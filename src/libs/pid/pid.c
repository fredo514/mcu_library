#include "pid.h"

typedef struct {
    PID_OUTPUT_t last_output;
    PID_GAIN_t p_gain;
    PID_GAIN_t i_gain;
    PID_GAIN_t d_gain;

    // for integral
    PID_OUTPUT_t integral_sum;
    PID_OUTPUT_t max_integral;
    PID_OUTPUT_t min_integral;

    // for derivative
    PID_INPUT_t last_input;

    // for output clamping
    PID_OUTPUT_t max_output;
    PID_OUTPUT_t min_output;
} PID_CTX_t;

ERROR_t Pid_Init(PID_h pid) {

}

ERROR_t Pid_Gain_Set(PID_h pid, PID_GAIN_t kp, PID_GAIN_t ki, PID_GAIN_t kd) {
    pid->p_gain = kp;
    pid->i_gain = ki;
    pid->d_gain = kd;
    
    return NO_ERROR;
}

PID_OUTPUT_t Pid_Update(PID_h pid, PID_ERROR_t error, PID_INPUT_t input) {
    // Calculate proportional term
    PID_OUTPUT_t p_term = pid->p_gain * error;

    // Calculate intergral term
    pid->integral_sum += error;
    if (pid->integral_sum > pid->max_integral) {
        pid->integral_sum = pid->max_integral;
    }
    else if (pid->integral_sum < pid->min_integral) {
        pid->integral_sum = pid->min_integral;
    }

    PID_OUTPUT_t i_term = pid->i_gain * pid->integral_sum;

    // Calculate derivative term
    PID_OUTPUT_t d_term = pid->d_gain * (input - pid->last_input);
    pid->last_input = input;

    // Calculate new output
    pid->last_output = p_term + i_term + d_term;
    if (pid->last_output > pid->max_output) {
        pid->last_output = pid->max_output;
    }
    else if (pid->last_output < pid->max_output) {
        pid->last_output = pid->max_output;
    }   

    return pid->last_output;
}

PID_OUTPUT_t Pid_Output_Get(PID_h pid) {
    return pid->last_output;
}