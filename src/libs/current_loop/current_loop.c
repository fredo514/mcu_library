#include "current_loop.h"
#include "assert.h"

struct CURRENT_LOOP_CTX {
    CURRENT_LOOP_STATUS_t status;
    uint32_t (*read_v)(void);
    float shunt_resistor_value_ohms;     ///< Precision resistor value for 4-20mA (e.g., 250Ω)
    float min_physical;
    float max_physical;
    float last_reading;
};

CURRENT_LOOP_h CurrentLoop_Create(void) {
    struct CURRENT_LOOP_CTX* loop = calloc(1, sizeof(struct CURRENT_LOOP_CTX));
    loop->status = CURRENT_LOOP_STATUS_RESET;

    return loop;
}

ERROR_CODE_t CurrentLoop_Init(CURRENT_LOOP_h loop, CURRENT_LOOP_CONFIG_t const* const config) {
    ASSERT(loop);
    ASSERT(config);

    loop->read_v = config->read_v;
    loop->shunt_resistor_value_ohms = config->shunt_resistor_value_ohms;
    loop->min_physical = config->min_physical;
    loop->max_physical = config->max_physical;

    return SUCCESS;
}

ERROR_CODE_t CurrentLoop_Read(CURRENT_LOOP_h loop, float* reading_percent) {
    ASSERT(loop);
    ASSERT(loop->status != CURRENT_LOOP_STATUS_RESET);

    if (CURRENT_LOOP_STATUS_BUSY == loop->status) {
        return ERROR_BUSY;
    }

    uint32_t raw_reading = loop->read_v();
    float current_ma = (float)reading_v / (loop->shunt_resistor_value_ohms * 1000);

    if ((current_ma < 4.0f) || (current_ma > 20.0f)) {
        return ERROR_OUT_OF_RANGE;
    }

    *reading_percent = loop->min_physical + ((current_ma - 4.0f) / (20.0f - 4.0f)) * (loop->max_physical - loop->min_physical);;
    loop->last_reading = reading_percent;

    return SUCCESS;
}

CURRENT_LOOP_STATUS_t CurrentLoop_GetStatus(CURRENT_LOOP_h loop) {
    ASSERT(loop);

    return loop->status;
}

float CurrentLoop_GetLastReading(CURRENT_LOOP_h loop) {
    ASSERT(loop);

    return loop->last_reading;
}
