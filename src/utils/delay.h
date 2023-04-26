#ifndef DELAY_H 
#define DELAY_H 

void Delay_Cycle(uint32_t duration);
void Delay_us(uint32_t duration);
void Delay_ms(uint32_t duration);

bool Delay_Is_ms_Elapsed(SYSTICK_TIME_t time_then, uint32_t period);
SYSTICK_TIME_t Delay_Elapsed_ms(SYSTICK_TIME_t time_then);

#endif DELAY_H
