#ifndef HSM_H
#define HSM_H

#include "error.h"
#include "stdbool.h"

#ifndef HSM_MAX_DEPTH
#define HSM_MAX_DEPTH 16
#endif

#define HSM_TRAN(target_state_ptr)           \
   ({                                        \
      sm->target_state = (target_state_ptr); \
      HSM_STATUS_TRAN;                       \
   })

typedef enum {
   HSM_SIG_EMPTY = 0,
   HSM_SIG_INIT,
   HSM_SIG_ENTRY,
   HSM_SIG_EXIT,
   HSM_SIG_USER_DEFINED_START,
} hsm_sig_t;

typedef enum {
   HSM_STATUS_UNHANDLED,  // Event unhandled due to guard condition
   HSM_STATUS_HANDLED,    // Event processed (internal transition)
   HSM_STATUS_IGNORED,    // Event ignored by top level state
   HSM_STATUS_TRAN,       // regular transition
   HSM_STATUS_MAX
} hsm_status_t;

typedef struct {
   hsm_sig_t signal;
   void* param;
} hsm_event_t;

typedef struct hsm_ctx hsm_t;

typedef hsm_status_t (*hsm_statehandler_t)(hsm_t* const sm, hsm_sig_t const signal);

typedef struct hsm_state {
   struct hsm_state* const parent;    // NULL for top
   hsm_statehandler_t const handler;  // state handler
} hsm_state_t;

struct hsm_ctx {
   hsm_state_t* curr_state;   /* current active leaf state */
   hsm_state_t* target_state; /* transition target (set by handler via macro) */
};

extern hsm_state_t hsm_top_state;

void Hsm_Init(hsm_t* const sm, hsm_state_t const* const initial_state);

// void Hsm_Dispatch(hsm_t* const sm, hsm_event_t const * const event);
void Hsm_Dispatch(hsm_t* const sm, hsm_sig_t const signal);

hsm_state_t* Hsm_State_Get(hsm_t const* const sm);
void Hsm_State_Set(hsm_t* const sm, hsm_state_t* const state);

#endif  // HSM_H