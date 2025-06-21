// model points types can't inherit from another type, they are all independent
// invalid after init

#ifndef DM_REGISTRY_H
#define DM_REGISTRY_H

#include "dm_registry_config.h"
#include "modelpoint.h"

typedef enum {
#define MODELPOINT(name, type, init) DMREGISTRY_POINT_##name, MODELPOINT_LIST
#undef MODELPOINT
   DMREGISTRY_POINTS_MAX_NUM,
} dmRegistry_mpId_t;

extern modelpoint_t *dmRegistry_modelpoints[DMREGISTRY_POINTS_MAX_NUM];

void DmRegistry_Points_InitAll(void);

// modelpoint_t* DmRegistry_Point_GetById(dmRegistry_mpId_t const id);
// modelpoint_t* DmRegistry_Point_GetByName(char const* const name);

#endif  // DM_REGISTRY_H