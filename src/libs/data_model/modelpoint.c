#include "modelpoint.h"

#include <string.h>

void Modelpoint_Init(modelpoint_t *const mp, modelpoint_config_t *const pConfig) {
   mp->name = pConfig->name;
   mp->pData = pConfig->dataPtr;
   mp->dataSize = pConfig->dataSize_bytes;

   Modelpoint_Unlock(mp);

   if (pConfig->isValidOnInit) {
      Modelpoint_Set(mp, pConfig->initValPtr);
   } else {
      memset(mp->pData, 0, mp->dataSize);
      mp->isValid = false;
   }

   mp->updateCount = 0;
}

char *Modelpoint_Name_Get(modelpoint_t const *const mp) {
   return mp->name;
}

bool Modelpoint_IsLocked(modelpoint_t const *const mp) {
   return mp->isLocked;
}

bool Modelpoint_Lock(modelpoint_t *const mp) {
   bool prevLockState = mp->isLocked;
   mp->isLocked = true;

   return prevLockState;
}

bool Modelpoint_Unlock(modelpoint_t *const mp) {
   bool prevLockState = mp->isLocked;
   mp->isLocked = false;

   return prevLockState;
}

bool Modelpoint_IsValid(modelpoint_t const *const mp) {
   return mp->isValid;
}

bool Modelpoint_Set(modelpoint_t *const mp, void const *const pNewValue) {
   if (!Modelpoint_IsLocked(mp)) {
      memcpy(mp->pData, pNewValue, mp->dataSize);
      mp->updateCount++;
      mp->isValid = true;

      return true;
   }

   return false;
}

bool Modelpoint_Get(modelpoint_t const *const mp, void *const pDest) {
   if (Modelpoint_IsValid(mp)) {
      memcpy(pDest, mp->pData, mp->dataSize);
      return true;
   }

   return false;
}

uint32_t Modelpoint_UpdateCount_Get(modelpoint_t const *const mp) {
   Modelpoint_Lock(mp);
   uint32_t count = mp->updateCount;
   Modelpoint_Unlock(mp);

   return count;
}