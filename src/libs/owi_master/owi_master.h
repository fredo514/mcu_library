#ifndef OWI_MASTER_H
#define OWI_MASTER_H

#include <stdbool.h>
#include <stdint.h>
#include "uart.h"
 
typedef enum
{
	OWI_MASTER_STATE_RESET,
	OWI_MASTER_STATE_IDLE,
	OWI_MASTER_STATE_BUSY_RESET,
	OWI_MASTER_STATE_BUSY_WRITE,
	OWI_MASTER_STATE_BUSY_READ,
	OWI_MASTER_STATE_FAIL,
	OWI_MASTER_STATE_MAX
} OWI_MASTER_STATE_t;

// TODO: register virtual functions instead of a uart instance to allow different interfaces.
typedef struct
{
	UART_h uart;
} OWI_MASTER_CONFIG_t;

void Owi_Master_Init(OWI_MASTER_CONFIG_t const * config);
void Owi_Master_Abort(void);
OWI_MASTER_STATE_t Owi_Master_State_Get(void);

void Owi_Master_Byte_Write(uint8_t data);
void Owi_Master_Byte_Read(void);
uint8_t Owi_Master_Byte_Get(void);

void Owi_Master_Reset_Pulse(void);
bool Owi_Master_Presence_Get(void);

// TODO: strong pulse interface
// void Owi_Master_Strong_Pullup(bool state);

#endif // OWI_MASTER_H
