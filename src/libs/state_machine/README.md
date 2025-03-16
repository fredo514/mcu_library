# Properties
* Hierarchical state nesting
* Entry/exit action execution on state transition
* Nested initial transitions
* Re-entrant event processor
* Events with arbitrary parameters
* Tracing support



The state machine module must be extended for each state machine needed in order to implement the specific events and states map.
E.g.:

sm1_state_map.h 
```
#include "hsm.h"

typedef enum {
    state_0 = HSM_STATE_USER_DEFINED_START,
    ...
    SM1_MAX_NUM_STATES
} SM1_STATE_t;

typedef enum {
    SM1_SIG_0 = HSM_SIG_USER_DEFINED_START,
    ...
} SM1_SIG_t;

typedef {
    HSM_EVT_t base_event;
    uint8_t event_data;
} SM1_EVENT1_t;

void Sm1_Init_Entry (SM_h sm, void * event);
void Sm1_Init_State (SM_h sm, void * event);
void Sm1_Init_Guard (SM_h sm, void * event);
void Sm1_Init_Exit (SM_h sm, void * event);

void Sm1_State0_Entry (SM_h sm, void * event); 
void Sm1_State0_State (SM_h sm, void * event); 
void Sm1_State0_Guard (SM_h sm, void * event); 
void Sm1_State0_Exit (SM_h sm, void * event); 

... 

const HSM_STATE_t sm1_state_map[SM1_MAX_NUM_STATES] = { \ 
\// | State func                     | Guard func                      | Entry func                        | Exit func                   |
    {.state_func = &Sm1_Init_State,   .guard_func = &Sm1_Init_Guard,    .entry_func = &Sm1_Init_Entry,      .exit_func = &Sm1_Init_Exit}, \     // init 
    {.state_func = &Sm1_State0_State, .guard_func = &Sm1_State0_Guard,  .entry_func = &Sm1_State0_Entry,    .exit_func = &Sm1_State0_Exit} \    // state0  
    ... 
}; 
```

sm1_state_map.c
```
void Sm1_State0_Entry(SM_h sm, void * event) { 
    // do some stuff 
} 

void Sm1_State0_State(SM_h sm, void * event) { 
    switch (event) { 
        case SM1_EVENT_0: 
            // do some stuff 
        break; 

        default: 
        break; 
    } 
} 

void Sm1_State0_Guard(SM_h sm, void * event) { 
    switch (event) { 
        case SM1_EVENT_0: 
            // change state 
        break; 

        default: 
        vreak; 
    } 
} 

void Sm1_State0_Exit(SM_h sm, void * event) { 
    // do some stuff 
} 
```