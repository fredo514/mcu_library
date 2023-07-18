#include "delay.h"
#include "timer.h"

TIMER_h timer;

ERROR_CODE_t Delay_Init(TIMER_h delay_timer, uint32_t tick_period_us) {
    timer = delay_timer;
    
    TIMER_CONFIG_t config = {
        .period_us = tick_period_us,
        counter_mode = TIMER_UP_COUNT,
        clock_source = TIMER_PLL,
        clock_mode;
        autoreload = true
    };
    
    Timer_Init(timer, &config);
    Timer_Start(timer);
}

uint32_t Delay_Time_Since(uint32_t time_then) { 
    uint32_t now = Timer_Tick_Get(timer); 

    // Has rollover occurred?
    if (now <= time_then) { 
        // rollover has occurred 
        return (now - time_then); 
    } 

    return (now + (TIMER_TICK_MAX - time_then)); 
}

bool Delay_Is_Elapsed_ms(uint32_t time_then, uint32_t period) { 
    return (Delay_Time_Since(time_then) >= period); 
} 

