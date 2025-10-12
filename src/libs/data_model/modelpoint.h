#ifndef MODELPOINT_H
#define MODELPOINT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MP_MAX_SUBSCRIBERS 16

typedef enum { MP_STATE_NOSUBS, MP_STATE_IDLE, MP_STATE_PENDING, MP_STATE_MAX } mp_state_t;

typedef enum {
   MP_SUB_TYPE_ALWAYS,
   MP_SUB_TYPE_ONCHANGE,
   MP_SUB_TYPE_MAX
} mp_sub_type_t;

typedef void (*mp_subscriberCb_t)(void);

typedef struct {
   char const *name;
   void *pData;
   size_t dataSize;
   bool isValid;
   bool isLocked;
   uint32_t updateCount;
   mp_subscriberCb_t subscribers[MP_MAX_SUBSCRIBERS];
   uint8_t subCount;
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

bool Modelpoint_Subscribe(modelpoint_t *const mp, mp_subscriberCb_t const cb, mp_sub_type_t sub_type);
// Modelpoint_Unsubscribe()
void Modelpoint_Touch(modelpoint_t const *const mp);
// Modelpoint_CopyFrom()

bool Modelpoint_Set(modelpoint_t *const mp, void const *const pNewValue);
bool Modelpoint_Get(modelpoint_t const *const mp, void *const pDest);

// Modelpoint_ToJSON()
// Modelpoint_FromJSON()

#endif MODELPOINT_H