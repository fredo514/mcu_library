#include "core.h"
#include "core_cm3.h"

bool Core_Save_And_Disable_Interrupt(CORE_IRQ_t const IRQn) {
    bool state;
    if (IRQ_ALL == IRQn) {
        state = __get_PRIMASK();
        __disable_irq();
    }
    else {
        state = (bool)__NVIC_GetEnableIRQ(IRQn);
        __NVIC_DisableIRQ(IRQn);
    }

    return state;
}

void Core_Restore_Interrupt(CORE_IRQ_t const IRQn, bool const wasEnabled) {
    if (wasEnabled) {
        if (IRQ_ALL == IRQn) {
            __enable_irq();
        }
        else {
            __NVIC_EnableIRQ(IRQn);
        } 
    }
}

uint32_t SysTicks_Get(void) { 
    // disable timer interrupt and save state 
    bool int_state = Core_Save_And_Disable_Interrupt(); 

    // get ticks 
    uint32_t ticks = sysTickTimerCnt_get(); 

    // restore timer interrupt 
    Core_Restore_Interrupt(int_state); 

    return ticks; 
} 

void Core_Breakpoint(void) {
#ifdef __GNUC__
    __asm__("BKPT");
#elif __clang__
    __asm("BKPT");
#endif
}