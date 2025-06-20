#include "dp_registry.h"

typedef struct {
    bool (*set)     (dp_t* dp, const void* pNewData);
    bool (*get)     (dp_t* dp, void* pOutData);
    bool (*toJSON)  (dp_t* dp, char* dst, size_t dstSize, bool pretty);
    bool (*fromJSON)(dp_t* dp, const char* json);
    const char*     typeName;
} DataPointType;

#define DATAPOINT(name, type, init) \
  extern bool dp_##type_set(dp_t * mp, const type* pNewValue); \
  extern bool dp_##type_get(dp_t* mp, type* out);
DATAPOINT_LIST
#undef DATAPOINT

const DataPointType DataPointTypeVTable[DPT_COUNT];

// Declare static storage variables
#define DATAPOINT(name, type, init) 
    static type name##Storage = init; \
DATAPOINT_LIST
#undef DATAPOINT

dp_t dpRegistry_datapoints[DPREGISTRY_DPID_MAX_NUM] = {
#define DATAPOINT(name, type, init)
    {
        .typeId = DP_TYPE_##name,
    },
DATAPOINT_LIST
#undef DATAPOINT
}