#ifndef CORE_H
#define CORE_H

#include "stdint.h"
#include <stdbool.h>
#include "error.h"
#include "macros.h"

// TODO: power modes?

typedef enum {
  IRQ_ALL = SIZE_MAX
} CORE_IRQ_t;

// Macro to hide critical sections background
// usage:
// CRITICAL_SECTION(UART5_IRq) { 
//     foo(); 
// } 
#define CRITICAL_SECTION(irq_id) DEFER(bool MACRO_VAR(_int_state_) = Core_Save_And_Disable_Interrupts(irq_id), \ 
  Core_Restore_Interrupts(irq_id, MACRO_VAR(_int_state_))) 

ERROR_CODE_t Core_Init(void);
ERROR_CODE_t Core_Interrupt_Callback_Attach(CORE_IRQ_t const IRQn, void cb(void));

bool Core_Save_And_Disable_Interrupt(CORE_IRQ_t const IRQn);
void Core_Restore_Interrupt(CORE_IRQ_t const IRQn, bool const wasEnabled);

uint32_t Core_SysTicks_Get(void);

void Core_Nop(void);

#endif // CORE_H