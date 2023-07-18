bsp.h
```
typedef enum {
    BUTTON_PRESSED_SIG = AO_SIG_USER_DEFINED_START,
    BUTTON_RELEASED_SIG
} BLINKY_SIG;

Bsp_Init(void);
Bsp_Start(void);

extern AO_h blinky_ao;
```

bsp.c
```
#include "bsp.h"

static void Debounce(void) {
    if (button becomes pressed) {
        static AO_EVENT_t const button_pressed_event = {.signal = BLINKY_PRESSED_SIG};
        Ao_Post(blinky_ao, &button_pressed_event);
    }
    else if (button becomes released) {
        static AO_EVENT_t const button_released_event = {.signal = BLINKY_RELEASED_SIG};
        Ao_Post(blinky_ao, &button_released_event);
    }
}

static void Systick_Handler(void) {
   Debounce();
   Ao_TimeEvent_Service();
}

Bsp_Init(void) {
    Core_Init();
    Core_Interrupt_Callback_Attach(SYSTICK_IRQ, &Systick_Handler);
    Core_Systick_Start();
}
```

main.c
```
#include "bsp.h"

typedef struct {
    struct AO_CTX parent;
    AO_TIMEEVENT_h time_evt;
    bool is_led_on;  // could save this by using state machine
} BLINKY_AO;

static void Blinky_Dispatch(BLINKY_AO me, AO_EVENT_t const * const evt){
    switch (evt->signal) {
        case INIT_SIG:
            Led1_Off();
            // intentional fall through
        case TIMEOUT_SIG:
            if (me->is_led_on) {
                Led2_Off();
                me->is_led_on = false;
                Ao_timeevent_Arm(me->time_evt, 800*TICKS_PER_MS, 0);
            }
            else {
                Led2_On();
                me->is_led_on = true;
                Ao_timeevent_Arm(me->time_evt, 200*TICKS_PER_MS, 0);
            }
        break;

        case BUTTON_PRESSED_SIG:
            Led1_On();
        break;

        case BUTTON_RELEASED_SIG:
            Led1_Off();
        break;
        
        default:
        break;
    }
}

void Blinky_Init(BLINKY_AO * const me) {
    Ao_Init(&me->parent, (AO_DISPATCH_HANDLER)&Blinky_Dispatch);
    me->time_evt = Ao_Timeevent_Create();
    Ao_timeevent_Init(me->time_evt, TIMEOUT_SIG, &me->parent);
    me->is_led_on = false;
}

static AO_EVENT_t * blinky_queue[10];
static BLINKY_AO blinky;
AO_h blinky_ao = &blinky.super;

int main(void) {
    Bsp_Init();

    Blinky_Init(&blinky);
    Ao_Run(blinky_ao);

    Rtos_Run();
}
```
