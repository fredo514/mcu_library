#ifndef DELAY_H 
#define DELAY_H 

#include "stdint.h"
#include "stdbool.h"
#include "core.h"

bool Delay_Is_Elapsed_ms(CORE_SYSTICK_TIME_t time_then, uint32_t period);
CORE_SYSTICK_TIME_t Delay_Elapsed_ms(CORE_SYSTICK_TIME_t time_then);

#endif DELAY_H
