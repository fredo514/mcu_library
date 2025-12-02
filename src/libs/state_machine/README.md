# Hierarchical State Machine (HSM)

This module implements a lightweight **Hierarchical State Machine (HSM)** suitable for embedded firmware on microcontrollers (bare-metal or RTOS).  
It is inspired by classical event-driven hierarchical designs (e.g., Miro Samek), but simplified for clarity and portability.

The framework provides:

- Hierarchical state nesting (each state has a parent)
- Entry / Exit / Init semantics with deterministic ordering
- Transitions between any two states (siblings, ancestors, descendants)
- Nested initial transitions
- Strict separation between framework code and application-specific state machines

---

## Overview

An HSM consists of:

- A **state-machine instance** (`hsm_t`)
- A set of **states** (`hsm_state_t`), each with:
  - A `handler` function
  - A `parent` pointer (ending at `hsm_top_state`)
- A set of **event signals** (`hsm_sig_t`)
- A **dispatch** routine that routes events to the current state and up the ancestry chain

User code implements its own states and signals; the framework handles the mechanics of hierarchical dispatch, entry/exit/init propagation, and transitions.

---

## Minimal Usage Pattern

### 1. Define your signals

Place them in a header local to your state-machine:

```c
typedef enum {
    MY_SIG_A = HSM_SIG_USER_DEFINED_START,
    MY_SIG_B,
    /* ... */
} my_sig_t;
```

### 2. Declare your state handlers
Handlers must match:

```c
hsm_status_t My_State_Handler(hsm_t *sm, hsm_sig_t sig);
```

### 3. Define your state hierarchy
Each state needs:
* A parent (`&hsm_top_state` for top-level)
* A handler function

Example skeleton (THIS IS A TEMPLATE — adjust names, do not copy blindly):
```c
static hsm_state_t my_parent_state = {
    .parent  = &hsm_top_state,
    .handler = &My_Parent_Handler,
};

static hsm_state_t my_child_state = {
    .parent  = &my_parent_state,
    .handler = &My_Child_Handler,
};
```

### 4. Implement state handlers
Each handler typically:
* Responds to `HSM_SIG_ENTRY`, `HSM_SIG_EXIT`, `HSM_SIG_INIT` if needed
* Responds to user signals
* Returns `HSM_STATUS_TRAN` when requesting a transition
* Returns `HSM_STATUS_UNHANDLED` to let the parent try

Template:
```c
hsm_status_t My_Child_Handler(hsm_t *sm, hsm_sig_t sig)
{
    switch (sig) {
        case HSM_SIG_ENTRY:
            /* entry actions */
            return HSM_STATUS_HANDLED;

        case HSM_SIG_EXIT:
            /* exit actions */
            return HSM_STATUS_HANDLED;

        case HSM_SIG_INIT:
            /* optional nested initial transition */
            /* return HSM_TRAN(sm, &some_nested_state); */
            return HSM_STATUS_UNHANDLED;

        case MY_SIG_A:
            /* example transition */
            /* return HSM_TRAN(sm, &my_parent_state); */
            return HSM_STATUS_HANDLED;

        default:
            break;
    }

    return HSM_STATUS_UNHANDLED;
}
```
> **_NOTE:_** The actual `HSM_TRAN` macro internally sets the transition target. Use it only from within handlers.

### 5. Create and initialize an instance
```c
hsm_t *sm = Hsm_Create();     // or use static memory if desired
Hsm_Init(sm, &my_child_state);
```

### 6. Dispatch events
```c
Hsm_Dispatch(sm, MY_SIG_A);
```

The framework will:
1. Try the current state, then its parents
2. On HSM_STATUS_TRAN, perform:
    * Exit from source up to the Least Common Ancestor
    * Entry from LCA down to the target
    * Execute nested init transitions if requested

---

### Handler Semantics Summary
#### Reserved signals
* `HSM_SIG_ENTRY`
* `HSM_SIG_EXIT`
* `HSM_SIG_INIT`

#### Rules enforced by the framework
* No transitions are allowed from ENTRY or EXIT handlers (asserted in debug builds)
* An INIT handler may:
    * Do nothing
    * Request a transition to a descendant state (nested initialization)

### Normal signals
Every user-defined event starts at the current state and bubbles up until a handler returns:
* `HSM_STATUS_HANDLED`, or
* `HSM_STATUS_TRAN`, or
* It reaches `hsm_top_state`, which returns `HSM_STATUS_IGNORED`

---

### Recommended Project Structure
```
your_feature/
    your_sm.h        <-- public API for your state machine
    your_sm.c        <-- state definitions + handlers
libs/state_machine/
    hsm.h            <-- framework public API
    hsm.c            <-- framework implementation
    hsm_priv.h       <-- internal details
```
User state-machine modules remain clean and self-contained.

---

### Concurrency Guidance
The core HSM is **not re-entrant** today.

Recommended design pattern for embedded systems:
* **ISRs should NOT call Hsm_Dispatch directly**
* ISRs should **post events to a queue**
* A **single task or main loop** calls `Hsm_Dispatch`

This avoids race conditions on curr_state and target_state.

---

### Testing
This module includes a full Ceedling/Unity test suite exercising:
* Entry / Exit / Init propagation
* Nested initial transitions
* Transitions to ancestors, siblings, descendants
* Unhandled-event bubbling
* Transition correctness and ordering

---

## TODO (Planned Enhancements)
These features are intentionally not implemented yet but are planned:

### 1. Make the event processor fully re-entrant
* Allow safe use from multiple contexts (ISR + task, or multiple tasks)
* Requires guarding `curr_state` and `target_state` via:
    * Critical sections, OR
    * Atomic operations, OR
    * Redesign using an event queue

### 2. Support events with arbitrary parameters
* The `hsm_event_t` struct already exists but is unused
* API addition planned:
    * `Hsm_Dispatch_Event(hsm_t *sm, const hsm_event_t *evt)`
* Requires:
    * Handler signature updates OR dual API path
    * Clear ownership of parameter memory

### 3. Add tracing support
* Hook into entry/exit/transition/events:
    * Optional callback or macro-based tracer
    * Useful for debugging complex behaviors on MCUs
* Should be configurable at compile time to avoid overhead in release builds

---

### Final Notes
* The HSM is intentionally minimalistic and stable.
* Examples shown here are templates, not literal implementations, to avoid future drift.
* The full behavior—including nested transitions, LCA logic, and all edge cases—is documented in the accompanying unit tests.