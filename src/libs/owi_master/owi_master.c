#include "owi_master.h"
#include "stdbool.h"

static OWI_MASTER_STATE_t state = OWI_MASTER_STATE_RESET;   // initial value ensure init function must be called
static UART_h uart = 0;
static bool presence = false;
static uint8_t read_data = 0;
static uint8_t write_data = 0;
static uint8_t bits_done = 0;

static void Tx_Done_Callback(void);
static uint8_t Bit_To_Byte(bool b);

void Owi_Master_Init(OWI_MASTER_CONFIG_t const * config) {
    // abort pending transactions
    Owi_Master_Abort();

    // store uart instance to use
    uart = config->uart;

    // Set baudrate to 9600baud (assuming 48MHz clock), since the 1st request will likely be a reset pulse
    // must be done with peripheral disabled
    uart->instance->CR1 &= ~(USART_CR1_UE);
    uart->instance->BRR = 0x00001388;
    uart->instance->CR1 |= USART_CR1_UE;  // restore to previous state instead of blindly enabling it?

    Uart_Callback_Register(uart, UART_Tx_Done_Callback, &Tx_Done_Callback);

    read_data = 0;
}

OWI_MASTER_STATE_t Owi_Master_State_Get(void) {
    return state;
}

void Owi_Master_Abort(void) {
    Uart_Flush_Tx(uart);
    state = OWI_MASTER_STATE_IDLE;
    presence = false;
}

bool Owi_Master_Presence_Get(void) {
    return presence;
}

void Owi_Master_Reset_Pulse(void) {
    // ignore request if already busy
    if ((state != OWI_MASTER_STATE_IDLE) && (state != OWI_MASTER_STATE_FAIL)) {
        return;
    }
    
    state = OWI_MASTER_STATE_BUSY_RESET;
    presence = false;

    Uart_Flush_Rx(uart);

    // Set baudrate to 9600baud (assuming 48MHz clock), must be done with peripheral disabled
    uart->instance->CR1 &= ~(USART_CR1_UE);
    uart->instance->BRR = 0x00001388;
    uart->instance->CR1 |= USART_CR1_UE;    // TODO: restore to previous state instead of blindly enabling it?

    // 0xF0 at 9600baud is close enough to the reset pulse waveform spec
    Uart_Char_Send(uart, 0xF0);
}

void Owi_Master_Byte_Write(uint8_t data) {
    // ignore request if already busy
    if ((state != OWI_MASTER_STATE_IDLE) && (state != OWI_MASTER_STATE_FAIL)) {
        return;
    }
    
    state = OWI_MASTER_STATE_BUSY_WRITE;

    Uart_Flush_Rx(uart);

    // Set baudrate to 115200baud (assuming 48MHz clock), must be done with peripheral disabled
    uart->instance->CR1 &= ~(USART_CR1_UE);
    uart->instance->BRR = 0x000001A1;
    uart->instance->CR1 |= USART_CR1_UE;    // TODO: restore to previous state instead of blindly enabling it?

    // send first bit (0xFF if 1, 0x00 if 0)
    Uart_Char_Send(uart, Bit_To_Byte((bool)(data & 0x01)));
    write_data = data;
    bits_done = 1;
}

void Owi_Master_Byte_Read(void) {
    // ignore request if already busy
    if ((state != OWI_MASTER_STATE_IDLE) && (state != OWI_MASTER_STATE_FAIL)) {
        return;
    }
    
    state = OWI_MASTER_STATE_BUSY_READ;

    Uart_Flush_Rx(uart);

    // Set baudrate to 115200baud (assuming 48MHz clock), must be done with peripheral disabled
    uart->instance->CR1 &= ~(USART_CR1_UE);
    uart->instance->BRR = 0x000001A1;
    uart->instance->CR1 |= USART_CR1_UE;    // TODO: restore to previous state instead of blindly enabling it?

    // 0xFF at 115200baud is close enough to the read slot waveform spec
    Uart_Char_Send(uart, 0xFF);
}

uint8_t Owi_Master_Byte_Get(void) {
    // return 0 if a byte is in process of being read
    if (OWI_MASTER_STATE_BUSY_READ == state) {
        return 0;
    }
    else {
        // otherwise return the last read byte
        return read_data;
    }
}

static void Tx_Done_Callback(void) {
    // Make sure a char was received first
    if (!Uart_Is_Char_Available(uart)) {
        state = OWI_MASTER_STATE_FAIL;
    }
    else {
        switch (state) {
            case OWI_MASTER_STATE_BUSY_RESET:
                // receiving anything but 0xF0 during the reset pulse means a slave is present
                if (Uart_Char_Get(uart) != 0xF0) {
                    presence = true;
                }

                state = OWI_MASTER_STATE_IDLE;
            break;

            case OWI_MASTER_STATE_BUSY_WRITE:
                // check if received expected value
                if ((write_data & 0x01) && (Uart_Char_Get(uart) != 0xFF)) {
                    // sent 1 should have received 0xFF
                    state = OWI_MASTER_STATE_FAIL;
                }
                else if (!(write_data & 0x01) && (Uart_Char_Get(uart) != 0x00)) {
                    // sent 0 should have received 0x00
                    state = OWI_MASTER_STATE_FAIL;
                }

                // send next bit if not done
                if (bits_done >= 8) {
                    state = OWI_MASTER_STATE_IDLE;
                }
                else {
                    write_data >>= 1;
                    Uart_Char_Send(uart, Bit_To_Byte((bool)(write_data & 0x01)));
                    bits_done++;
                }
            break;

            case OWI_MASTER_STATE_BUSY_READ:
                // shift received bit in buffer
                read_data >>= 1;
                // receiving anything else than 0xFF is 0
                if (Uart_Char_Get(uart) == 0xFF) {
                    read_data |= 0x80;
                }

                // send next read slot if not done
                if (bits_done >= 8) {
                    state = OWI_MASTER_STATE_IDLE;
                }
                else {
                    Uart_Char_Send(uart, 0xFF);
                    bits_done++;
                }
            break;

            default:
            break;
        }
    }
}

static uint8_t Bit_To_Byte(bool b) {
    if (b) {
        // 0xFF at 115200baud is close enough to the 1 write slot waveform spec
        return 0xFF;
    }
    else {
        // 0x00 at 115200baud is close enough to the 0 write slot waveform spec
        return 0x00;
    }
}