#include "delay.h"
#include "timer.h"

TIMER_h timer;

ERROR_CODE_t Delay_Init(TIMER_h delay_timer, uint32_t tick_period_us) {
    timer = delay_timer;
    
    TIMER_CONFIG_t config = {
        .period_us = tick_period_us,
        counter_mode = TIMER_UP_COUNT,
        clock_source = TIMER_PLL,
        clock_mode = ;
        autoreload = true;
    };
    
    Timer_Init(timer, &config);
    Timer_Start(timer);
}

uint32_t Delay_Ticks_now(void) {
    return Timer_Tick_Get(timer);
}

uint32_t Delay_Ticks_Since(uint32_t ticks_then) { 
    uint32_t now = delay_Ticks_now(); 

    // Has rollover occurred?
    if (now <= ticks_then) { 
        // rollover has occurred 
        return (now + (TIMER_TICK_MAX - ticks_then)); 
    } 

    return (now - ticks_then); 
}

bool Delay_Is_Elapsed(uint32_t ticks_then, uint32_t duration) { 
    return (Delay_Ticks_Since(ticks_then) >= duration); 
} 

