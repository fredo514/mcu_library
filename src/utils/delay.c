#include "delay.h"
#include "sleep.h"

CORE_SYSTICK_TIME_t Delay_Elapsed_ms(CORE_SYSTICK_TIME_t time_then) { 
    CORE_SYSTICK_TIME_t now = SysTicks_Get(); 

    // Has rollover occurred?
    if (now <= time_then) { 
        // rollover has occurred 
        return (now - time_then); 
    } 

    return (now + (CORE_SYSTICK_TIME_MAX - time_then)); 
}

bool Delay_Is_Elapsed_ms(CORE_SYSTICK_TIME_t time_then, uint32_t period) { 
    return (Delay_Elapsed_ms(time_then) >= period); 
} 

