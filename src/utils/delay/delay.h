#ifndef DELAY_H 
#define DELAY_H 

#include "stdint.h"
#include "stdbool.h"
#include "error.h"

typedef struct {
  uint32_t (*time_now_cb)(void);
} delay_config_t;

error_t Delay_Init(delay_config_t const* const config);

uint32_t Delay_Ticks_now(void);
bool Delay_Is_Elapsed(uint32_t ticks_then, uint32_t duration);
uint32_t Delay_Ticks_Since(uint32_t ticks_then);

#endif DELAY_H
