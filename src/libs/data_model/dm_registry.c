#include "dm_registry.h"

#define MODELPOINT(name, type, init)                                                \
   static modelpoint_t mp_##name;                                                   \
   static type mp_##name_storage;                                                   \
   static modelpoint_config_t mp_##name_config = {.name = "##name",                 \
                                                  .dataPtr = mp_##name_storage,     \
                                                  .dataSize_bytes = sizeof(##type), \
                                                  .isValidOnInit = true,            \
                                                  .initValPtr = &(init)};           \
   MODELPOINT_LIST
#undef MODELPOINT

modelpoint_t *dmRegistry_modelpoints[DMREGISTRY_POINTS_MAX_NUM] = {
#define MODELPOINT(name, type, init) &mp_##name, MODELPOINT_LIST
#undef MODELPOINT
};

void DmRegistry_Points_InitAll(void) {
#define MODELPOINT(name, type, init)                                                    \
   Modelpoint_Init(dmRegistry_modelpoints[DMREGISTRY_POINT_##name], &mp_##name_config); \
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