#ifndef ACTIVEOBJECT_H
#define	ACTIVEOBJECT_H

#include "error.h"
#include "stdint.h"
#include "stdbool.h"

typedef enum {
    AO_SIG_EMPTY = 0,
    AO_SIG_INIT,
    AO_SIG_USER_DEFINED_START
} AO_SIGNAL_t;

typedef struct {
    AO_SIG_t signal;
    // event parameters added by inheritance in subclasses
} AO_EVENT_t;

typedef bool (*AO_DISPATCH_HANDLER)(AO_h ao, AO_EVENT_t const * const evt);

typedef struct AO_TIMEEVENT_CTX * AO_TIMEEVENT_h;

typedef struct AO_CTX * AO_h;

AO_h Ao_Create(uint8_t queue_len);
ERROR_CODE_t Ao_Init(AO_h ao, AO_DISPATCH_HANDLER dispatch);

ERROR_CODE_t Ao_Post_Event(AO_h ao, AO_EVT_t * event);
ERROR_CODE_t Ao_Run(AO_h ao);

AO_TIMEEVENT_h Ao_Timeevent_Create(void);
ERROR_CODE_t Ao_Timeevent_Init(AO_TIMEEVENT_h time_event, AO_SIGNAL_t sig, AO_h ao);
ERROR_CODE_t Ao_Timeevent_Arm(AO_TIMEEVENT_h time_event, uint32_t timeout, uint32_t period);
ERROR_CODE_t Ao_Timeevent_Stop(AO_TIMEEVENT_h time_event);
void Ao_TimeEvent_Service(void);

#endif // ACTIVEOBJECT_H