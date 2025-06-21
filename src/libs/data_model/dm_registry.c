#include "dm_registry.h"

#define MODELPOINT(mp_name, mp_type, mp_initVal)                                        \
   static modelpoint_t mp_##mp_name;                                                    \
   static mp_type mp_storage_##mp_name;                                                 \
   static modelpoint_config_t mp_config_##mp_name = {.name = #mp_name,                  \
                                                     .dataPtr = &mp_storage_##mp_name,  \
                                                     .dataSize_bytes = sizeof(mp_type), \
                                                     .isValidOnInit = true,             \
                                                     .initValPtr = &mp_initVal};
MODELPOINT_LIST
#undef MODELPOINT

modelpoint_t *dmRegistry_modelpoints[DMREGISTRY_POINTS_MAX_NUM] = {
#define MODELPOINT(mp_name, mp_type, mp_initVal) &mp_##mp_name,
    MODELPOINT_LIST
#undef MODELPOINT
};

void DmRegistry_Points_InitAll(void) {
#define MODELPOINT(mp_name, mp_type, mp_initVal) \
   Modelpoint_Init(dmRegistry_modelpoints[DMREGISTRY_POINT_##mp_name], &mp_config_##mp_name);
   MODELPOINT_LIST
#undef MODELPOINT
}

// typedef struct {
//    bool (*toJSON)(dp_t* dp, char* dst, size_t dstSize, bool pretty);
//    bool (*fromJSON)(dp_t* dp, const char* json);
//    const char* typeName;
// } DataPointType;

// #define DATAPOINT(name, type, init)                            \
//    extern bool dp_##type_set(dp_t* mp, const type* pNewValue); \
//    extern bool dp_##type_get(dp_t* mp, type* out);
// DATAPOINT_LIST
// #undef DATAPOINT

// const DataPointType DataPointTypeVTable[DPT_COUNT];

// // Declare static storage variables
// #define DATAPOINT(name, type, init)
// static type name##Storage = init;
// DATAPOINT_LIST
// #undef DATAPOINT

// modelpoint_t dmRegistry_modelpoints[DMREGISTRY_MPID_MAX_NUM] = {
// #define DATAPOINT(name, type, init)
//     {
//         .typeId = DP_TYPE_##name,
//     },
//     DATAPOINT_LIST
// #undef DATAPOINT
// }