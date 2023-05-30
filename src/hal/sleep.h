#ifndef SLEEP_H 
#define SLEEP_H 

#include "stdint.h"

void Sleep_Cycle(uint32_t duration);
void Sleep_us(uint32_t duration);
void Sleep_ms(uint32_t duration);

#endif SLEEP_H