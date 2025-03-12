// for 4-20mA conversion

#ifndef CURRENT_LOOP_H
#define CURRENT_LOOP_H

#include "error.h"
#include "adc.h"

#include <stdint.h>

typedef enum {
    CURRENT_LOOP_STATE_RESET,
    CURRENT_LOOP_STATE_IDLE,
    CURRENT_LOOP_STATE_BUSY,
    CURRENT_LOOP_STATE_FAULT,
    CURRENT_LOOP_STATE_MAX
} CURRENT_LOOP_STATE_t;

typedef enum {
    uint32_t (*read_v)(void);
    float shunt_resistor_value_ohms;     ///< Precision resistor value for 4-20mA (e.g., 250Ω)
    float min_physical;
    float max_physical;
} CURRENT_LOOP_CONFIG_t;

typedef struct CURRENT_LOOP_CTX const* const CURRENT_LOOP_h;

CURRENT_LOOP_h CurrentLoop_Create(void);
ERROR_CODE_t CurrentLoop_Init(CURRENT_LOOP_h loop, CURRENT_LOOP_CONFIG_t const* const config);
CURRENT_LOOP_STATE_t CurrentLoop_State_Get(CURRENT_LOOP_h loop);

ERROR_CODE_t CurrentLoop_Read(CURRENT_LOOP_h loop, float* reading_percent);
float CurrentLoop_LastRead_Get(CURRENT_LOOP_h loop);

#endif // CURRENT_LOOP_H