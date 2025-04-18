#include "current_loop.h"
#include "assert.h"

struct CURRENT_LOOP_CTX {
    CURRENT_LOOP_STATE_t state;
    uint32_t (*read_v)(void);
    float shunt_resistor_value_ohms;     ///< Precision resistor value for 4-20mA (e.g., 250Ω)
    float min_physical;
    float max_physical;
    float last_reading;
};

CURRENT_LOOP_h CurrentLoop_Create(void) {
    struct CURRENT_LOOP_CTX* loop = calloc(1, sizeof(struct CURRENT_LOOP_CTX));
    loop->state = CURRENT_LOOP_STATE_RESET;

    return loop;
}

ERROR_CODE_t CurrentLoop_Init(CURRENT_LOOP_h loop, CURRENT_LOOP_CONFIG_t const* const config) {
    ASSERT(loop);
    ASSERT(config);
    ASSERT(config->read_v);
    ASSERT(config->shunt_resistor_value_ohms > 0);

    loop->read_v = config->read_v;
    loop->shunt_resistor_value_ohms = config->shunt_resistor_value_ohms;
    loop->min_physical = config->min_physical;
    loop->max_physical = config->max_physical;

    return SUCCESS;
}

ERROR_CODE_t CurrentLoop_Read(CURRENT_LOOP_h loop, float* reading_percent) {
    ASSERT(loop);
    ASSERT(loop->status != CURRENT_LOOP_STATE_RESET);

    if (CURRENT_LOOP_STATE_BUSY == loop->status) {
        return ERROR_BUSY;
    }

    loop->status = CURRENT_LOOP_STATUS_BUSY;

    float reading_v = loop->read_v();
    float current_ma = reading_v / (loop->shunt_resistor_value_ohms * 1000);

    if ((current_ma < 4.0f) || (current_ma > 20.0f)) {
        loop->status = CURRENT_LOOP_STATE_FAULT;
        return ERROR_OUT_OF_RANGE;
    }

    *reading_percent = loop->min_physical + (((current_ma - 4.0f) / (20.0f - 4.0f)) * (loop->max_physical - loop->min_physical));
    loop->last_reading = reading_percent;

    loop->status = CURRENT_LOOP_STATUS_IDLE;

    return SUCCESS;
}

CURRENT_LOOP_STATE_t CurrentLoop_State_Get(CURRENT_LOOP_h loop) {
    ASSERT(loop);

    return loop->status;
}

float CurrentLoop_LastReading_Get(CURRENT_LOOP_h loop) {
    ASSERT(loop);

    return loop->last_reading;
}
