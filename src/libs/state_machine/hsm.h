/**
 * @file   hsm.h
 *
 * @brief   Hierarchical State-machines Library.
 * @details
 * This module implements a lightweight hierarchical state machine (HSM)
 * framework suitable for embedded firmware. It allows application code to
 * model complex behavior using a tree of states where each state can handle
 * events or delegate them to its parent.
 *
 * Key features:
 * - Each state is represented by a handler function receiving events.
 * - Unhandled events are automatically propagated up the parent chain.
 * - Entry, exit, and transition actions are explicitly managed by the framework.
 * - Compatible with cooperative or preemptive scheduling models.
 *
 * Typical usage:
 * 1. Define states, their ancestry (if applicable), their handler function.
 * 2. Initialize an instance with `Hsm_Init()`, providing the initial state.
 * 3. Dispatch events using `Hsm_Dispatch()`, the framework manages traversal.
 *
 * This module is inspired by event-driven patterns described by Miro Samek and
 * adapted for simplification.
 */

#ifndef HSM_H
#define HSM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "error.h"
#include "stdbool.h"

// -----------------------------------------------------------------------------
// Public Constants
// -----------------------------------------------------------------------------

#ifndef HSM_MAX_DEPTH
#define HSM_MAX_DEPTH 16
#endif

// -----------------------------------------------------------------------------
// Public Types
// -----------------------------------------------------------------------------

/**
 * @brief Macro for returning a transition status.
 */
#define HSM_TRAN(target_state_ptr)           \
   ({                                        \
      sm->target_state = (target_state_ptr); \
      HSM_STATUS_TRAN;                       \
   })

/**
 * @brief Reserved event signals.
 */
typedef enum {
   HSM_SIG_EMPTY = 0,
   HSM_SIG_INIT,
   HSM_SIG_ENTRY,
   HSM_SIG_EXIT,
   HSM_SIG_USER_DEFINED_START,  // set the first user defined event of concrete state machines to this
} hsm_sig_t;

/**
 * @brief Event processing return codes.
 */
typedef enum {
   HSM_STATUS_UNHANDLED,  // Event unhandled
   HSM_STATUS_HANDLED,    // Event processed
   HSM_STATUS_IGNORED,    // Event ignored (sinked)
   HSM_STATUS_TRAN,       // Transition
   HSM_STATUS_MAX
} hsm_status_t;

/**
 * @brief Event structure.
 */
typedef struct {
   hsm_sig_t signal;  // Event signal
   void* param;       // Event parameters (typecast to concrete parameters)
} hsm_event_t;

/**
 * @brief Opaque state-machine Pointer.
 */
typedef struct _hsm_ctx hsm_t;

/**
 * @brief State handler function prototype.
 */
typedef hsm_status_t (*hsm_statehandler_t)(hsm_t* const sm, hsm_sig_t const signal);

/**
 * @brief State structure.
 */
typedef struct hsm_state {
   struct hsm_state* const parent;    // NULL for top
   hsm_statehandler_t const handler;  // state handler
} hsm_state_t;

// needs to keep this here as HSM_TRAN accesses guts
#include "hsm_priv.h"

/**
 * @brief All state machines must have the top state as their ultimate ancestor.
 */
extern hsm_state_t hsm_top_state;

// -----------------------------------------------------------------------------
// Public Functions
// -----------------------------------------------------------------------------

hsm_t* Hsm_Create(void);
void Hsm_Init(hsm_t* const sm, hsm_state_t const* const initial_state);

// void Hsm_Dispatch(hsm_t* const sm, hsm_event_t const * const event);
void Hsm_Dispatch(hsm_t* const sm, hsm_sig_t const signal);

hsm_state_t* Hsm_State_Get(hsm_t const* const sm);
#ifdef TEST
void Hsm_State_Set(hsm_t* const sm, hsm_state_t* const state);
#endif

#ifdef __cplusplus
}
#endif

#endif  // HSM_H