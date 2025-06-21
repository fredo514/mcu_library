#ifndef DM_REGISTRY_CONFIG_H
#define DM_REGISTRY_CONFIG_H

// X-Macro list of all modelpoints
#define MODELPOINT_LIST                       \
   MODELPOINT(BATH_TEMPERATURE, float, 25.0f) \
   MODELPOINT(MOTORSPEED, int32_t, 0)         \
   MODELPOINT(MYBANANA, banana_t, (banana_t){.fleshWeight_g = 100.0f, .peel = BANANA_PEEL_RIPE})

#endif DM_REGISTRY_CONFIG_H