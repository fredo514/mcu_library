#ifndef TIMER_H
#define TIMER_H

#include "stdint.h"
#include <stdbool.h>
#include "error.h"
#include "timer_def.h"	// contains platform-specific defines

typedef TIMER_CTX * const TIMER_h;

#define DISABLE false
#define ENABLE true

typedef struct {
    TIMER_h timer;
    bool state;
    TIMER_CNT_MODE counter_mode;
    TIMER_CLK_SRC clock_source;
    TIMER_CLK_MODE clock_mode;
    bool isr_enable;
    uint32_t period_us;      // or prescaler, or frequency?
    bool autoreload;
} TIMER_CONFIG;

ERROR_CODE Timer_Init(TIMER_CONFIG const * const config);
ERROR_CODE Timer_Control(TIMER_h timer, bool const state);
ERROR_CODE Timer_Period_Set(TIMER_h timer, uint32_t const period_us);

uint32_t Timer_Count_Get(TIMER_h timer);
ERROR_CODE Timer_Count_Set(TIMER_h timer, uint32_t const count);

// Use these SPARINGLY to extend the API
ERROR_CODE Gpio_Reg_Write (REG_SIZE * const address, uint32_t const val);
REG_SIZE Gpio_Reg_Read (REG_SIZE * const address);

#endif // TIMER_H