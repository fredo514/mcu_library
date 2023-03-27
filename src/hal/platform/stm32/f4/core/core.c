#include "core.h"
#include "core_cm3.h"


bool Core_Save_And_Disable_Interrupts(CORE_IRQ_t const IRQn) {
    bool state;
    if (IRQ_ALL == IRQn) {
        state = __get_PRIMASK();
        __disable_irq();
    }
    else {
        state = __NVIC_GetEnableIRQ(IRQn);
        __NVIC_DisableIRQ(IRQn)
    }

    return state;
}

void Core_Restore_Interrupts(CORE_IRQ_t const IRQn, bool const wasEnabled) {
    if (wasEnabled) {
        if (IRQ_ALL == IRQn) {
            __enable_irq();
        }
        else {
            __NVIC_EnableIRQ(IRQn)
        } 
    }
}