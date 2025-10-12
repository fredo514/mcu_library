#ifndef HSM_H
#define HSM_H

#include "error.h"
#include "stdbool.h"

#ifndef HSM_MAX_DEPTH
#define HSM_MAX_DEPTH 16
#endif

typedef enum {
   HSM_SIG_EMPTY = 0,
   HSM_SIG_INIT,
   HSM_SIG_ENTRY,
   HSM_SIG_EXIT,
   HSM_SIG_USER_DEFINED_START,
} hsm_sig_t;

typedef enum {
   HSM_STATUS_SUPER,      // event passed to parent
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

typedef hsm_status_t (*hsm_statehandler_t)(hsm_t* const sm, hsm_event_t const* const e);

typedef struct hsm_state {
   const struct hsm_state* const parent;   // NULL for top)
   const struct hsm_state* const initial;  // initial child; NULL if none (leaf)
   hsm_statehandler_t handler;             // state handler
} hsm_state_t;

hsm_t* Hsm_Create(void);
void Hsm_Init(hsm_t* const sm, hsm_state_t* const top_state);

void Hsm_Dispatch(hsm_t* const sm, hsm_event_t* const event);

hsm_state_t* Hsm_State_Get(hsm_t* const sm);
void Hsm_State_Set(hsm_t* const sm, hsm_state_t* const state);

#endif  // HSM_H