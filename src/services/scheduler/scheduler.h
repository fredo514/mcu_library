// simple scheduler for non-blocking active objects

#ifndef SCHEDULER_H
#define SCHEDULER_H

ERROR_CODE_t Sched_Init();
ERROR_CODE_t Sched_Task_Create( uint8_t queue_len);

ERROR_CODE_t Sched_Run(void);
ERROR_CODE_t Sched_TimeEvent_Arm(timeevent, timeout, repetitionrate);

#endif // SCHEDULER_H