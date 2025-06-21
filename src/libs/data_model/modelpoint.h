#ifndef MODELPOINT_H
#define MODELPOINT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
   //    DataPointTypeId typeId;
   char const *name;
   void *pData;
   size_t dataSize;
   bool isValid;
   bool isLocked;
   uint32_t updateCount;
   //    MP_SubscriberCallback subs[MP_MAX_SUBSCRIBERS];
   //    void *subCtx[MP_MAX_SUBSCRIBERS];
   //    uint8_t subCount;
} modelpoint_t;

typedef struct {
   char const *name;
   void *dataPtr;
   size_t dataSize_bytes;
   bool isValidOnInit;
   void *initValPtr;
} modelpoint_config_t;

void Modelpoint_Init(modelpoint_t *const mp, modelpoint_config_t *const pConfig);

char *Modelpoint_Name_Get(modelpoint_t const *const mp);
uint32_t Modelpoint_UpdateCount_Get(modelpoint_t const *const mp);

bool Modelpoint_IsLocked(modelpoint_t const *const mp);
bool Modelpoint_Lock(modelpoint_t *const mp);
bool Modelpoint_Unlock(modelpoint_t *const mp);

bool Modelpoint_IsValid(modelpoint_t const *const mp);

// Modelpoint_Subscribe()

bool Modelpoint_Set(modelpoint_t *const mp, void const *const pNewValue);
bool Modelpoint_Get(modelpoint_t const *const mp, void *const pDest);

// Modelpoint_ToJSON()
// Modelpoint_FromJSON()

#endif MODELPOINT_H