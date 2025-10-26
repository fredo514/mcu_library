#ifndef CORE_H
#define CORE_H

#include <stdbool.h>

#define CRITICAL_SECTION(irq) if (true)

typedef enum {
   CORE_IRQ_ALL,
} core_irq_t;

bool Core_Interrupts_SaveAndDisable(core_irq_t const IRQn);
void Core_Interrupts_Restore(core_irq_t const irq_id, bool const wasEnabled);

#endif  // CORE_H