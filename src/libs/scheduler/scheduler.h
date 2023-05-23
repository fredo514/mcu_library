// simple scheduler for non-blocking active objects

#ifndef SCHEDULER_H
#define SCHEDULER_H

void Sched_Run(void);
void Sched_Task_Start(void (*handler)(ctx, event), priority, SCHED_EVT_t * evt_queue, uint8_t nelem);
void Sched_TimeEvent_Arm(timeevent, timeout, repetitionrate);
void Sched_Event_Post(activeobject, event);

#endif // SCHEDULER_H