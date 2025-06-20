#ifndef DP_REGISTRY_CONFIG_H
#define DP_REGISTRY_CONFIG_H

// X-Macro list of all datapoints
#define DATAPOINT_LIST \
    DATAPOINT(temperature, float, 25.0f) \
    DATAPOINT(motorSpeed, int32_t, 0) \
    DATAPOINT(MyBanana, banana_t, (banana_t){.fleshWeight_g = 100.0f, .peel = BANANA_PEEL_RIPE})

#endif DP_REGISTRY_CONFIG_H