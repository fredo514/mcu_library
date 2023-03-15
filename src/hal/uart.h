#ifndef UART_H
#define UART_H

#include "stdint.h"
#include <stdbool.h>
#include "error.h"
#include "uart_def.h"	// contains platform-specific defines

typedef UART_CTX * const UART_h;

typedef struct {
} UART_CONFIG;

ERROR_CODE Uart_Init(UART_h uart, UART_CONFIG const * const config);
ERROR_CODE Uart_Callback_Register(UART_h uart, UART_CALLBACK const function, void (*cb)(void));  // really need this?

ERROR_CODE Uart_Char_Put(UART_h uart, uint8_t const ch);
ERROR_CODE Uart_Break(UART_h uart);
uint8_t Uart_Char_Get(UART_h uart);
ERROR_CODE Uart_Flush_All(UART_h uart);
ERROR_CODE Uart_Flush_Rx(UART_h uart);
ERROR_CODE Uart_Flush_Tx(UART_h uart);
 
bool Uart_Is_Tx_Done(UART_h uart);
bool Uart_Is_Char_Available(UART_h uart);
bool Uart_Is_Tx_Full(UART_h uart);
bool Uart_Is_Rx_Full(UART_h uart);

// Use these SPARINGLY to extend the API
ERROR_CODE Uart_Reg_Write (REG_SIZE * const address, uint32_t const val);
REG_SIZE Uart_Reg_Read (REG_SIZE * const address);

#endif // UART_H