#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "ring_fifo.h"
#include "stdbool.h"

typedef enum {
    SM_STATE_INIT = 0,
    SM_STATE_BASE_MAX
} SM_STATE_BASE_t;

typedef enum {
    SM_EVENT_INVALID = 0,
    SM_EVENT_RESET,
    SM_EVENT_ENTRY,
    SM_EVENT_EXIT,
    SM_EVENT_BASE_MAX
} SM_EVENT_BASE_t;

typedef SM_STATE_STRUCT_t SM_STATE_t;
typedef SM_CTX_t const * const SM_h;

void Sm_Init (SM_h sm, RING_FIFO_t eventQueue, SM_STATE_t * pState_map); 

bool Sm_Dispatch(SM_h sm); 
ERROR_t Sm_Post_Event(SM_h sm, void * event);
SM_STATE_t Sm_State_Get(SM_h sm)

#endif // STATE_MACHINE_H