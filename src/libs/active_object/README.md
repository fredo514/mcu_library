// bsp.h
typedef enum {
    BLINKY_PRESSED_SIG = HSM_SIG_USER_DEFINED_START,
    BLINKY_RELEASED_SIG
}

Bsp_Init(void);
Bsp_Start(void);

extern AO_h button_ao;


// bsp.c
#include "bsp.h"

static void Debounce(void) {
    if (button becomes pressed) {
        static Ao_Event_t const button_pressed_event = {.signal = BLINKY_PRESSED_SIG};
        Ao_Post(button_ao, &button_pressed_event);
    }
    else if (button becomes released) {
        static Ao_Event_t const button_released_event = {.signal = BLINKY_RELEASED_SIG};
        Ao_Post(button_ao, &button_released_event);
    }
}

static void Systick_Handler(void) {
   Debounce();
   TimeEvent_Service();
}

Bsp_Init(void) {
    Core_Init();
    Core_Interrupt_Callback_Attach(SYSTICK_IRQ, &Systick_Handler);
}

Bsp_Start(void) {
    Core_Systick_Start();
}


// main.c
#include "bsp.h"

typedef struct {
    struct AO_CTX parent;
    // no private data
} BUTTON_AO;

static void Button_Dispatch(BLINKY_AO me, AO_EVENT_t const * const evt){
    switch (evt->signal) {
        case BLINKY_PRESSED_SIG:
            Led1_On();
        break;

        case BLINKY_RELEASED_SIG:
            Led1_Off();
        break;
        
        default:
        break;
    }
}

void Button_Init(BUTTON_AO * const me) {
    Ao_Init(&me->parent, (AO_DISPATCH_HANDLER)&Button_Dispatch);
}

static AO_EVENT_t * button_queue[10];
static BUTTON_AO button;
AO_h button_ao = &button.super;

typedef struct {
    struct AO_CTX parent;
    AO_TIMEEVENT_h time_evt;
} BLINKY_AO;

static void Blinky_Dispatch(BLINKY_AO me, AO_EVENT_t const * const evt){
    switch (evt->signal) {
        case TIMEOUT_SIG:
            Led2_Toggle();
        break;
        
        default:
        break;
    }
}

void Blinky_Init(BLINKY_AO * const me) {
    Ao_Init(&me->parent, (AO_DISPATCH_HANDLER)&Blinky_Dispatch);
    me->time_evt = Ao_Timeevent_Create();
    Ao_timeevent_Init(me->time_evt, TIMEOUT_SIG, &me->parent);
}

static AO_EVENT_t * blinky_queue[10];
static BLINKY_AO blinky;
AO_h blinky_ao = &blinky.super;

int main(void) {
    Bsp_Init();
    Bsp_Start();

    Button_Init(&button);
    Ao_Run(button_ao);

    Blinky_Init(&blinky);
    Ao_Run(blinky_ao);

    Rtos_Run();
}