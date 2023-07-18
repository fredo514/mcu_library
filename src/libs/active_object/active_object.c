#include "active_object.h"
#include "state_machine.h"
#include "ringfifo.h"

typedef void (*Dispatch_Handler)(AO_h ao, AO_EVENT_t * const evt);

struct AO_CTX {
    RTOS_THREAD_h thread;
    RINGFIFO_h evt_queue;       // queue of pointers to events
    Dispatch_Handler Dispatch;
    // active obkect data added by inheritance in subclasses
}

struct AO_TIMEEVENT_CTX {
    AO_EVENT_t parent;
    AO_h requester;
    uint32_t timeout;
    uint32_t period;
}

static AO_TIMEEVENT_h time_events[10];
uint_fast8_t time_events_num;

AO_h Ao_Create(uint8_t queue_len) {
    AO_h inst = calloc(1, sizeof(struct AO_CTX));
    inst.evt_queue = Ringfifo_Create(queue_len);

    return inst;
}

ERROR_CODE_t Ao_Init(AO_h ao, Dispatch_Handler dispatch) {
    ao->Dispatch = dispatch;
}

static void Event_Loop(void * params) {
    AO_h ao = (AO_h)params;
    assert(ao);

    static AO_EVENT_t const init_event = {AO_SIG_INIT}

    (*ao->Dispatch)(ao, &init_event);

    while (1) {
        if (!Ringfifo_Is_Empty(ao->evt_queue)) {
            AO_EVENT_t * evt = Ringfifo_Pop(ao->evt_queue);
            (*ao->Dispatch)(ao, evt);
        }
    }
}

ERROR_CODE_t Ao_Run(AO_h ao) {
    ao->thread = Rtos_Thread_Create(.func = &Event_Loop);
    Rtos_Thread_Start(ao->thread);
}

ERROR_CODE_t Ao_Post_Event(AO_h ao, AO_EVENT_t * evt) {
    ERROR_CODE_t ret = Ringfifo_Push(ao->evt_queue, (void *)&evt);
    assert(ret == NO_ERROR);
}

ERROR_CODE_t Ao_Timeevent_Init(AO_TIMEEVENT_h time_event, AO_SIGNAL_t sig, AO_h ao) {
    time_event->parent.signal = sig;
    time_event->ao = ao;
    time_event->timeout = 0;
    time_event->period = 0;

    assert(time_events_num < sizeof(time_events/sizeof(time_events[0])));

    time_events[time_events_num++] = time_event;
}

ERROR_CODE_t Ao_Timeevent_Arm(AO_TIMEEVENT_h time_event, uint32_t timeout, uint32_t period) {
    CRITICAL_SECTION(IRQ_ALL) {
        time_event->timeout = timeout;
        time_event->period = period;
    }
}

ERROR_CODE_t Ao_Timeevent_Stop(AO_TIMEEVENT_h time_event) {
    CRITICAL_SECTION(IRQ_ALL) {
        time_event->timeout = 0;
    }
}

void Ao_TimeEvent_Service(void) {
    for (uint_fast8_t i = 0; i < time_events_num; ++i) {
        AO_TIMEEVENT_h const evt = time_events[i];
        assert(evt);

        if (evt->timeout > 0) {
            if (--(evt->timeout) == 0) {
                Ao_Post(evt->ao, &evt->parent);
                evt->timeout = evt->period;
            }
        }
    }
}
