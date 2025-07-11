#ifndef UART_H
#define UART_H

#include "stdint.h"
#include <stdbool.h>
#include "error.h"
#include "uart_def.h"	// contains platform-specific defines

typedef enum {
    UART_BAUD_9600 = 9600,
    UART_BAUD_57600 = 57600,
    UART_BAUD_115200 = 115200,
    UART_BAUD_MAX
} UART_BAUD_RATE_t;

typedef enum {
    UART_7BIT_WORD,
    UART_8BIT_WORD,
    UART_WORD_MAX
} UART_WORD_LEN_t;

typedef enum {
    UART_NO_PARITY,
    UART_ODD_PARITY,
    UART_EVEN_PARITY,
    UART_MARK_PARITY,
    UART_SPACE_PARITY,
    UART_PARITY_MAX
} UART_STOP_BITS_t;

typedef enum {
    UART_1_STOPBIT,
    UART_2_STOPBIT,
    UART_STOPBIT_MAX
} UART_PARITY_t;

typedef enum {
    UART_MODE_FULL_DUPLEX,
    UART_MODE_HALF_DUPLEX,
    UART_MODE_MAX
} UART_MODE_t;

typedef enum {
    UART_TX_HALFDONE_CALLBACK,
    UART_TX_DONE_CALLBACK,
    UART_RX_HALFDONE_CALLBACK,
    UART_RX_DONE_CALLBACK,
    UART_ERROR_CALLBACK,
    UART_ABORT_CALLBACK,
} UART_CALLBACK_t;

typedef UART_CTX_t * const UART_h;

typedef struct {
    UART_BAUD_RATE_t baud_rate;
    UART_WORD_LEN_t word_length;
    UART_STOP_BITS_t stop_bits;
    UART_PARITY_t parity;
    UART_MODE_t mode;
} UART_CONFIG_t;

UART_h Uart_Create(UART_REGS_t const * const regs);
ERROR_CODE_t Uart_Init(UART_h uart, UART_CONFIG_t const * const config);
ERROR_CODE_t Uart_Callback_Register(UART_h uart, UART_CALLBACK_t const function, void (*cb)(UART_h uart));  // really need this?
ERROR_CODE_t Uart_Enable(UART_h uart);
ERROR_CODE_t Uart_Disable(UART_h uart);
ERROR_STATUS_t Uart_Status_Get(UART_h uart);
IRQn_Type Uart_Irqn_Get(UART_h uart);

ERROR_CODE_t Uart_Char_Write(UART_h uart, uint8_t const ch);
ERROR_CODE_t Uart_Buffer_Write(UART_h uart, uint8_t const * const buf, size_t len);
ERROR_CODE_t Uart_Break(UART_h uart);
uint8_t Uart_Char_Read(UART_h uart);
ERROR_CODE_t Uart_Buffer_Read(UART_h uart, uint8_t * const buf, size_t len);
ERROR_CODE_t Uart_Flush_All(UART_h uart);
ERROR_CODE_t Uart_Rx_Flush(UART_h uart);
ERROR_CODE_t Uart_Tx_Flush(UART_h uart);

bool Uart_Is_Char_Available(UART_h uart);
bool Uart_Is_Tx_Full(UART_h uart);
bool Uart_Is_Tx_Done(UART_h uart);

// Use these SPARINGLY to extend the API
ERROR_CODE_t Uart_Reg_Write(REG_SIZE_t * const address, uint32_t const val);
REG_SIZE_t Uart_Reg_Read(REG_SIZE_t * const address);

#endif // UART_H