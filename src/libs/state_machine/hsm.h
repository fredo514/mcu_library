#ifndef HSM_H
#define HSM_H

#include "ring_fifo.h"
#include "stdbool.h"

typedef enum {
    SM_STATE_INIT = 0,
    SM_STATE_BASE_MAX
} HSM_STATE_t;

typedef enum {
    SM_SIG_INVALID = 0,
    SM_SIG_RESET,
    SM_SIG_ENTRY,
    SM_SIG_EXIT,
    SM_SIG_BASE_MAX
} HSM_SIG_t;

typedef enum {
    // unhandled and bubbled up
    HSM_STATUS_SUPER,       // event passed to parent
    HSM_STATUS_SUPER_SUB,   // ??
    HSM_STATUS_UNHANDLED,   // Event unhandled due to guard condition

    // handled
    HSM_STATUS_HANDLED,     // Event processed (internal transition)
    HSM_STATUS_IGNORED,     // Event ignored by top level state

    // entry/exit
    HSM_STATUS_ENTRY,       // State entry action executed
    HSM_STATUS_EXIT,        // State exit action executed

    HSM_STATUS_NULL,        // ??

    // need to execute transition action
    HSM_STATUS_TRAN,        // regular transition
    HSM_STATUS_TRAN_INIT,   // initial transition
    HSM_STATUS_TRAN_ENTRY,  // entry transition
    HSM_STATUS_TRAN_HIST,   // ??
    HSM_STATUS_TRAN_EXIT,   // exit transition
    HSM_STATUS_MAX
} HSM_STATUS_t;

typedef struct {
    HSM_SIG_t sig;
} HSM_EVT_t;

typedef HSM_STATE_INTERNAL_t HSM_STATE_t;
typedef HSM_CTX_t const * const HSM_h;

void Hsm_Init (HSM_h sm, RING_FIFO_t eventQueue, HSM_STATE_t * pState_map); 

bool Hsm_Dispatch(HSM_h sm); 
ERROR_t Hsm_Post_Event(HSM_h sm, void * event);
HSM_STATE_t Sm_State_Get(HSM_h sm)

#endif // HSM_H