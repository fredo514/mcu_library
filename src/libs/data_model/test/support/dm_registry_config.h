#ifndef DM_REGISTRY_CONFIG_H
#define DM_REGISTRY_CONFIG_H

typedef enum {
   BANANA_PEEL_GREEN,
   BANANA_PEEL_YELLOW,
   BANANA_PEEL_YELLOW_WITH_BROWN_DOTS,
   BANANA_PEEL_BROWN,
   BANANA_PEEL_MAX_COUNT
} banana_peel_t;

typedef struct {
   float fleshWeight;
   banana_peel_t peel;
} banana_t;

// X-Macro list of all modelpoints
#define MODELPOINT_LIST                                  \
   MODELPOINT(BATH_TEMPERATURE, float, ((float){25.0f})) \
   MODELPOINT(MOTORSPEED, int32_t, ((int32_t){0}))       \
   MODELPOINT(MYBANANA, banana_t, ((banana_t){.fleshWeight = 100.0f, .peel = BANANA_PEEL_YELLOW}))

#endif DM_REGISTRY_CONFIG_H