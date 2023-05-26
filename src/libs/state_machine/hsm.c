#include "hsm.h"
#include "assert.h"

typedef HSM_STATUS_t (*state_handler_funptr)(void * const sm, HSM_EVT_t const * const event);
typedef HSM_STATUS_t (*action_handler_funptr)(void * const sm);

typedef struct HSM_STATE_INTERNAL { 
    HSM_STATE_t const * parent;
    state_handler_funptr const state_handler;
    state_handler_funptr const guard_func;
    action_handler_funptr const entry_action;
    action_handler_funptr const exit_action;
    action_handler_funptr const init_action;
} HSM_STATE_INTERNAL_t; 

typedef struct { 
    HSM_STATE_t const target_state;
    action_handler_funptr const * action_table;
} HSM_TRAN_ACTION_TABLE_t; 

typedef union {
    state_handler_funptr state_handler;
    action_handler_funptr action_handler;
    HSM_TRAN_ACTION_TABLE_t const * tran_action_table;
    HSM_STATE_t const * state;
} HSM_MEMBER_UNION_t;

typedef struct { 
    void (*init)(HSM_h sm, void const * const event);
    void (*dispatch)(HSM_h sm, void const * const event);
    HSM_MEMBER_UNION_t state;
    
    HSM_STATE_t next_state; 
    HSM_STATE_t current_state; 
    const HSM_STATE_t * state_map; 
} HSM_CTX_t; 

void Hsm_Init (HSM_h sm, SM_STATE_t * pState_map) { 
    sm->state_map = pState_map; 
    sm->current_state = HSM_STATE_INIT; 
} 

bool Hsm_dispatch (HSM_h sm, HSM_EVT_t * event) { 
    bool guard_result = true; 

    // check that we have a valid state 
    //assert(state < MAX_NUM_STATES);

    // TODO: pass event to parent if not handled

    void (*state)(HSM_h sm, void * event) = sm->state_map[sm->next_state].state_func; 
    bool (*guard)(HSM_h sm, void * event) = sm->state_map[sm->next_state].guard_func; 
    void (*entry)(HSM_h sm, void * event) = sm->state_map[sm->next_state].entry_func; 
    void (*exit)(HSM_h sm, void * event) = sm->state_map[sm->next_state].exit_func; 
 
    if (*event != SM_EVENT_INVALID) { 
        if (guard != NULL) { 
            guard_result = guard(sm, event); 
        } 

        if (guard_result) { 
            if (sm->next_state != sm->current_state) { 
                if (exit != NULL) { 
                    exit(sm, event); 
                } 

                if (entry != NULL) { 
                    entry(sm, event); 
                } 
            } 

            sm->current_state = sm->next_state; 
            state(sm, event); 
        } 
    } 

    return true;
} 