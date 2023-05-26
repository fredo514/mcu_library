#include "active_object.h"
#include "state_machine.h"
#include "ringfifo.h"

ERROR_CODE_t Ao_Run(AO_h ao) {
    if (!Ringfifo_Is_Empty(ao->evt_queue)) {
        evt = Ringfifo_Pop(ao->evt_queue);
        Hsm_Dispatch(ao->hsm, evt);
    }
}