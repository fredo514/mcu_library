#include "owi_master.h"
#include "stdbool.h"

static OWI_MASTER_STATE_t state = OWI_MASTER_STATE_RESET;   // initial value ensure init function must be called
static UART_h uart = 0;
static bool presence = false;
static uint8_t read_data = 0;
static uint8_t write_data = 0;
static uint8_t bits_done = 0;

static void tx_done_callback(void);
static uint8_t bit_to_byte(bool b);

void owi_master_init(OWI_MASTER_CONFIG_t const * config) {
    // abort pending transactions
    owi_master_abort();

    // store uart instance to use
    uart = config->uart;

    // Set baudrate to 9600baud (assuming 48MHz clock), since the 1st request will likely be a reset pulse
    // must be done with peripheral disabled
    uart->instance->CR1 &= ~(USART_CR1_UE);
    uart->instance->BRR = 0x00001388;
    uart->instance->CR1 |= USART_CR1_UE;  // restore to previous state instead of blindly enabling it?

    uart_cb_register(uart, UART_CB_TX_DONE, &tx_done_callback);

    read_data = 0;
}

OWI_MASTER_STATE_t owi_master_state_get(void) {
    return state;
}

void owi_master_abort(void) {
    uart_flush_tx(uart);
    state = OWI_MASTER_STATE_IDLE;
    presence = false;
}

bool owi_master_presence_get(void) {
    return presence;
}

void owi_master_reset_pulse(void) {
    // ignore request if already busy
    if ((state != OWI_MASTER_STATE_IDLE) && (state != OWI_MASTER_STATE_FAIL)) {
        return;
    }
    
    state = OWI_MASTER_STATE_BUSY_RESET;
    presence = false;

    uart_flush_rx(uart);

    // Set baudrate to 9600baud (assuming 48MHz clock), must be done with peripheral disabled
    uart->instance->CR1 &= ~(USART_CR1_UE);
    uart->instance->BRR = 0x00001388;
    uart->instance->CR1 |= USART_CR1_UE;    // TODO: restore to previous state instead of blindly enabling it?

    // 0xF0 at 9600baud is close enough to the reset pulse waveform spec
    uart_char_send(uart, 0xF0);
}

void owi_master_byte_write(uint8_t data) {
    // ignore request if already busy
    if ((state != OWI_MASTER_STATE_IDLE) && (state != OWI_MASTER_STATE_FAIL)) {
        return;
    }
    
    state = OWI_MASTER_STATE_BUSY_WRITE;

    uart_flush_rx(uart);

    // Set baudrate to 115200baud (assuming 48MHz clock), must be done with peripheral disabled
    uart->instance->CR1 &= ~(USART_CR1_UE);
    uart->instance->BRR = 0x000001A1;
    uart->instance->CR1 |= USART_CR1_UE;    // TODO: restore to previous state instead of blindly enabling it?

    // send first bit (0xFF if 1, 0x00 if 0)
    uart_char_send(uart, bit_to_byte((bool)(data & 0x01)));
    write_data = data;
    bits_done = 1;
}

void owi_master_byte_read(void) {
    // ignore request if already busy
    if ((state != OWI_MASTER_STATE_IDLE) && (state != OWI_MASTER_STATE_FAIL)) {
        return;
    }
    
    state = OWI_MASTER_STATE_BUSY_READ;

    uart_flush_rx(uart);

    // Set baudrate to 115200baud (assuming 48MHz clock), must be done with peripheral disabled
    uart->instance->CR1 &= ~(USART_CR1_UE);
    uart->instance->BRR = 0x000001A1;
    uart->instance->CR1 |= USART_CR1_UE;    // TODO: restore to previous state instead of blindly enabling it?

    // 0xFF at 115200baud is close enough to the read slot waveform spec
    uart_char_send(uart, 0xFF);
}

uint8_t owi_master_byte_get(void) {
    // return 0 if a byte is in process of being read
    if (OWI_MASTER_STATE_BUSY_READ == state) {
        return 0;
    }
    else {
        // otherwise return the last read byte
        return read_data;
    }
}

static void tx_done_callback(void) {
    // Make sure a char was received first
    if (!uart_is_char_available(uart)) {
        state = OWI_MASTER_STATE_FAIL;
    }
    else {
        switch (state) {
            case OWI_MASTER_STATE_BUSY_RESET:
                // receiving anything but 0xF0 during the reset pulse means a slave is present
                if (uart_char_get(uart) != 0xF0) {
                    presence = true;
                }

                state = OWI_MASTER_STATE_IDLE;
            break;

            case OWI_MASTER_STATE_BUSY_WRITE:
                // check if received expected value
                if ((write_data & 0x01) && (uart_char_get(uart) != 0xFF)) {
                    // sent 1 should have received 0xFF
                    state = OWI_MASTER_STATE_FAIL;
                }
                else if (!(write_data & 0x01) && (uart_char_get(uart) != 0x00)) {
                    // sent 0 should have received 0x00
                    state = OWI_MASTER_STATE_FAIL;
                }

                // send next bit if not done
                if (bits_done >= 8) {
                    state = OWI_MASTER_STATE_IDLE;
                }
                else {
                    write_data >>= 1;
                    uart_char_send(uart, bit_to_byte((bool)(write_data & 0x01)));
                    bits_done++;
                }
            break;

            case OWI_MASTER_STATE_BUSY_READ:
                // shift received bit in buffer
                read_data >>= 1;
                // receiving anything else than 0xFF is 0
                if (uart_char_get(uart) == 0xFF) {
                    read_data |= 0x80;
                }

                // send next read slot if not done
                if (bits_done >= 8) {
                    state = OWI_MASTER_STATE_IDLE;
                }
                else {
                    uart_char_send(uart, 0xFF);
                    bits_done++;
                }
            break;

            default:
            break;
        }
    }
}

static uint8_t bit_to_byte(bool b) {
    if (b) {
        // 0xFF at 115200baud is close enough to the 1 write slot waveform spec
        return 0xFF;
    }
    else {
        // 0x00 at 115200baud is close enough to the 0 write slot waveform spec
        return 0x00;
    }
}