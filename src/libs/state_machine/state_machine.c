#include "state_machine.h"
#include "assert.h"

typedef struct { 
    void (*state_func)(SM_h sm, void * event); 
    bool (*guard_func)(SM_h sm, void * event); 
    void (*entry_func)(SM_h sm, void * event); 
    void (*exit_func)(SM_h sm); 
} SM_STATE_STRUCT_t; 

typedef struct { 
    SM_STATE_t next_state; 
    SM_STATE_t current_state; 
    void * event_queue; 
    const SM_STATE_t * state_map; 
} SM_CTX_t; 

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