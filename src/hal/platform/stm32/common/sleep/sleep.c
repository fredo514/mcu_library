#include "sleep.h"
#include "core.h"

void Delay_Cycle(uint32_t duration) {
    for (uint32_t i; i < duration; ++i) {
        Core_Nop();
    }
}

void Delay_us(uint32_t duration) {
    for (uint32_t i; i < duration; ++i) {
        Delay_Cycle(CORE_CYCLES_PER_US);
    }
}

void Delay_ms(uint32_t duration) {
    CORE_SYSTICK_TIME_t start = Core_SysTicks_Get();

    while(!Delay_Is_Elapsed_ms(start, duration)) {
        // busy wait
    }
}