#ifndef ACTIVEOBJECT_H
#define	ACTIVEOBJECT_H

#include "error.h"
#include "state_machine.h"

typedef AO_CTX_t* AO_h;

AO_h Ao_Create(uint8_t queue_len);
ERROR_CODE_t Ao_Init(AO_h ao, bool (*dispatch)(HSM_h sm, HSM_EVT_t * event));

ERROR_CODE_t Ao_Post_Event(AO_h ao, AO_EVT_t * event);
ERROR_CODE_t Ao_Run(AO_h ao);

#endif // ACTIVEOBJECT_H