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

void owi_master_init(OWI_MASTER_CONFIG_t const * config);
void owi_master_abort(void);
OWI_MASTER_STATE_t owi_master_state_get(void);

void owi_master_byte_write(uint8_t data);
void owi_master_byte_read(void);
uint8_t owi_master_byte_get(void);

void owi_master_reset_pulse(void);
bool owi_master_presence_get(void);

// TODO: strong pulse interface
// void owi_master_strong_pullup_enable(void);
// void owi_master_strong_pullup_disable(void);

#endif // OWI_MASTER_H
