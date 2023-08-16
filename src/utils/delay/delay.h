#ifndef DELAY_H 
#define DELAY_H 

#include "stdint.h"
#include "stdbool.h"

uint32_t Delay_Ticks_now(void);
bool Delay_Is_Elapsed(uint32_t ticks_then, uint32_t duration);
uint32_t Delay_Time_Since(uint32_t ticks_then);

#endif DELAY_H
