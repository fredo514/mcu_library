#ifndef DELAY_H 
#define DELAY_H 

#include "stdint.h"
#include "stdbool.h"
#include "core.h"

bool Delay_Is_Elapsed(uint32_t time_then, uint32_t period);
uint32_t Delay_Time_Since(uint32_t time_then);

#endif DELAY_H
