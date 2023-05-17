#ifndef PID_H
#define PID_H

#include "error.h"

// TODO: feed-forward, reset tieback, integer math, different form, velocity instead of position, setpoint ramping

typedef PID_DATA_t float;

typedef enum {
    PID_MODE_ACTIVE = 0,
    PID_MODE_OVERRIDE,
    PID_MODE_MAX
} PID_MODE_t;

typedef enum {
    PID_ACTION_DIRECT = 0,
    PID_ACTION_REVERSE,
    PID_ACTION_MAX
} PID_ACTION_t;

typedef struct {
    PID_DATA_t kp;
    PID_DATA_t ki;
    PID_DATA_t kd;
    PID_DATA_t max_output;
    PID_DATA_t min_output;
    PID_ACTION_t action;
    float p_on_m_weight;
    PID_MODE_t mode;

    PID_DATA_t (*error_calc_cb)(PID_DATA_t setpoint, PID_DATA_t input);
} PID_CONFIG_t;

typedef PID_CTX_t const * const PID_h;

PID_h Pid_Create(void)
ERROR_CODE_t Pid_Init(PID_h pid, PID_CONFIG_t const * const config);
ERROR_CODE_t Pid_Error_Calc_Callback_Register(PID_h pid, PID_DATA_t (*error_calc_cb)(PID_DATA_t setpoint, PID_DATA_t input));

ERROR_CODE_t Pid_Gain_Set(PID_h pid, PID_DATA_t kp, PID_DATA_t ki, PID_DATA_t kd, float p_on_m_weight);
ERROR_CODE_t Pid_Setpoint_Set(PID_h pid, PID_DATA_t setpoint);
ERROR_CODE_t Pid_Override(PID_h pid, PID_DATA_t output);
ERROR_CODE_t Pid_Resume(PID_h pid);
PID_MODE_t Pid_Mode_Get(PID_h pid);
ERROR_CODE_t Pid_Action_Set(PID_h pid, PID_ACTION_t action);

PID_DATA_t Pid_Update(PID_h pid, PID_DATA_t input);
PID_DATA_t Pid_Output_Get(PID_h pid);

#endif // PID_H