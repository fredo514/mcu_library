// model points types can't inherit from another type, they are all independent
// invalid after init

#ifndef DP_REGISTRY_H
#define DP_REGISTRY_H

#include "model_point.h"
#include "dp_registry_config.h"

typedef enum {
#define DATAPOINT(name, type, init) 
    DP_TYPE_##name,
DATAPOINT_LIST
#undef DATAPOINT
    DP_TYPE_MAX_NUM
} dp_types_t;

// Generate enum of datapoint IDs
typedef enum {
#define DATAPOINT(name, type, init) 
    DPREGISTRY_DPID_##name,
DATAPOINT_LIST
#undef DATAPOINT
    DPREGISTRY_DPID_MAX_NUM
} dpRegistry_dpId_t;

/* Array of all points */
extern dp_t dpRegistry_datapoints[DPREGISTRY_DPID_MAX_NUM];

void DpRegistry_Point_InitAll(void);

dp_t * DpRegistry_Point_GetById(dpRegistry_dpId_t const id);
dp_t * DpRegistry_Point_GetByName(char const * const name);

#define Dp_Set(dp, pNewData) (DataPointTypeVTable[(dp)->typeId].set((dp),(pNewData)))

#endif // DP_REGISTRY_H