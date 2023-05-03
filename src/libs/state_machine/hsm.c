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
    HSM_MEMBER_UNION_t temp;
    
    HSM_STATE_t next_state; 
    HSM_STATE_t current_state; 
    HSM_EVT_t * event_queue; 
    const HSM_STATE_t * state_map; 
} HSM_CTX_t; 

static bool Is_Event_Available(SM_h sm) { 
    return !Ringfifo_IsEmpty(sm->event_queue); 
} 

static EVENT_t Pop_Event(SM_h sm) { 
    return Ringfifo_Get(sm->event_queue); 

}

void Sm_Init (SM_h sm, RINGFIFO_h event_queue, SM_STATE_t * pState_map) { 
    sm->state_map = pState_map; 
    sm->current_state = SM_STATE_INIT; 
    sm->event_queue = event_queue; 
    Ringfifo_Init(event_queue); 
} 

bool Sm_dispatch (SM_h sm) { 
    bool guard_result = true; 

    if (is_event_available(sm)) { 
        // check that we have a valid state 
        //assert(state < MAX_NUM_STATES);

        void (*state)(SM_h sm, void * event) = sm->state_map[sm->next_state].state_func; 
        bool (*guard)(SM_h sm, void * event) = sm->state_map[sm->next_state].guard_func; 
        void (*entry)(SM_h sm, void * event) = sm->state_map[sm->next_state].entry_func; 
        void (*exit)(SM_h sm, void * event) = sm->state_map[sm->next_state].exit_func; 

        void * event = pop_event(sm); 
        if (event != SM_EVENT_INVALID) { 
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

    return false; 
} 