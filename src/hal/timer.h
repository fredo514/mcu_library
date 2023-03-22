#ifndef TIMER_H
#define TIMER_H

#include "stdint.h"
#include <stdbool.h>
#include "error.h"
#include "timer_def.h"	// contains platform-specific defines

typedef TIMER_CTX_t * const TIMER_h;

#define DISABLE false
#define ENABLE true

typedef struct {
    bool state;
    TIMER_CNT_MODE_t counter_mode;
    TIMER_CLK_SRC_t clock_source;
    TIMER_CLK_MODE_t clock_mode;
    bool isr_enable;
    uint32_t period_us;      // or prescaler, or frequency?
    bool autoreload;
} TIMER_CONFIG_t;

ERROR_CODE_t Timer_Init(TIMER_h timer, TIMER_CONFIG_t const * const config);
ERROR_CODE_t Timer_Callback_Register(TIMER_h timer, TIMER_CALLBACK_t const function, void (*cb)(void));  // really need this?

ERROR_CODE_t Timer_Control(TIMER_h timer, bool const state);
ERROR_CODE_t Timer_Period_Set(TIMER_h timer, uint32_t const period_us);

uint32_t Timer_Count_Get(TIMER_h timer);
ERROR_CODE_t Timer_Count_Set(TIMER_h timer, uint32_t const count);

// Use these SPARINGLY to extend the API
ERROR_CODE_t Timer_Reg_Write (REG_SIZE_t * const address, uint32_t const val);
REG_SIZE_t Timer_Reg_Read (REG_SIZE_t * const address);

#endif // TIMER_H