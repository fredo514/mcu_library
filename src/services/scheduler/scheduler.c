#include "scheduler.h"
#include "active_object.h"

ERROR_CODE_t Sched_Run(void) {
    for (int i = 0, i<num_tasks; ++i) {
        Ao_Run(task_array[i]);
    }
}