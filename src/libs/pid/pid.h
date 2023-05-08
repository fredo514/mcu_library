#ifndef PID_H
#define PID_H

#include "error.h"

// TODO: make all same type?
typedef PID_OUTPUT_t float;
typedef PID_INPUT_t float;
typedef PID_ERROR_t float;
typedef PID_GAIN_t float;

typedef PID_CTX_t const * const PID_h;

ERROR_t Pid_Init(PID_h pid);
ERROR_t Pid_Gain_Set(PID_h pid, PID_GAIN_t kp, PID_GAIN_t ki, PID_GAIN_t kd);

PID_OUTPUT_t Pid_Update(PID_h pid, PID_ERROR_t error, PID_INPUT_t input);
PID_OUTPUT_t Pid_Output_Get(PID_h pid);

#endif // PID_H