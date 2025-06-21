#include "uart.h"
#include "ring_fifo.h"
#include "macros.h"

typedef struct UART_CTX {
    UART_REGS_t const * const regs;
    RING_FIFO_h fifo;
    UART_STATUS_t status;
};

UART_h Uart_Create(UART_REGS_t const * const regs, uint8_t buf_len) {
    UART_h inst = calloc(1, sizeof(struct UART_CTX));
    inst->regs = regs;
    inst->status = UART_STATUS_RESET;

    inst->tx_fifo = Ringfifo_Create(buf_len);
    Ringfifo_Init(inst->tx_fifo);
    inst->rx_fifo = Ringfifo_Create(buf_len);
    Ringfifo_Init(inst->rx_fifo);

    return inst;
}

ERROR_CODE_t Uart_Init(UART_h uart, UART_CONFIG_t const * const config) {
    Ringfifo_Init(uart->tx_fifo);
    Ringfifo_Init(uart->rx_fifo);

    // enable pins and peripheral clock
    // set baud rate
    // configure interrupts

    inst->status = UART_STATUS_READY;
}

ERROR_CODE_t Uart_Enable(UART_h uart) {
    // enable interrupts
}

ERROR_CODE_t Uart_Disable(UART_h uart){
    // disable interrupts
}

ERROR_CODE_t Uart_Char_Put(UART_h uart, uint8_t const ch) {
    bool irq_state = Core_Save_And_Disable_Interrupt(??); // TODO: how to find IRQ number from instance?
    Ringfifo_Push(uart->tx_fifo, ch);
    Core_Restore_Interrupt(irq_state);

    uart->regs->CR1 |= USART_CR1_TXEIE;
}

ERROR_CODE_t Uart_Break(UART_h uart) {
    uart->regs->CR1 |= USART_CR1_SBK;
}

uint8_t Uart_Char_Get(UART_h uart) {
    return Ringfifo_Pop(uart->rx_fifo);
}

ERROR_CODE_t Uart_Flush_All(UART_h uart) {
    Uart_Flush_Rx(uart);
    Uart_Flush_Tx(uart);
}

ERROR_CODE_t Uart_Flush_Rx(UART_h uart) {
    Ringfifo_Flush(uart->rx_fifo);
}

ERROR_CODE_t Uart_Flush_Tx(UART_h uart) {
    Ringfifo_Flush(uart->tx_fifo);
}

bool Uart_Is_Char_Available(UART_h uart) {
    return !Ringfifo_Is_Empty(uart->rx_fifo)
}

bool Uart_Is_Tx_Full(UART_h uart) {
    return Ringfifo_Is_Full(uart->tx_fifo);
}

static void Irq_Handler(UART_h uart) {
    // check if received something
    if (uart->regs->SR & USART_SR_RXNE) {
        if (!Ringfifo_Is_Full(uart->rx_fifo)) {
            Ringfifo_Push(uart->rx_fifo, uart->regs->DR);
        }
        else {
            // TODO: what to do if fifo already full?
        }
    }
    
    // check if ready to send next byte
    if (uart->regs->SR & USART_SR_TXE) {
        // check if still data to send
        if (!Ringfifo_Is_Empty(uart->tx_fifo)) {
            Io_Write(uart->regs->DR, Ringfifo_Pop(uart->tx_fifo));
        }
        else {
            // Done, disable transmit interrupt
            CLEAR_MASK(uart->regs->CR1, USART_CR1_TXEIE);
            //uart->regs->CR1 &= ~USART_CR1_TXEIE;
        }
    }
}

ERROR_CODE_t inline Uart_Reg_Write (REG_SIZE_t const address, uint32_t const val) {
    *((REG_SIZE_t * const)address) = val;
}

REG_SIZE_t inline Uart_Reg_Read (REG_SIZE_t const address) {
    return *((REG_SIZE_t * const)address);
}

void UART1_IRQHandler (void) {
    Irq_Handler(uart1);
}

void UART2_IRQHandler (void) {
    Irq_Handler(uart2);
}

void UART3_IRQHandler (void) {
    Irq_Handler(uart3);
}

void UART4_IRQHandler (void) {
    Irq_Handler(uart4);
}

void UART5_IRQHandler (void) {
    Irq_Handler(uart5);
}