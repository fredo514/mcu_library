#include "pid.h"

typedef struct {
    PID_DATA_t last_output;
    PID_DATA_t setpoint;
    
    PID_DATA_t p_gain;
    PID_DATA_t i_gain;
    PID_DATA_t d_gain; 
    PID_ACTION_t action;
    PID_MODE_t mode;
    float p_on_m_weight;

    // for integral
    PID_DATA_t i_term;

    // for derivative
    PID_DATA_t last_input;

    // for windup Clamping
    PID_DATA_t max_output;
    PID_DATA_t min_output;

    PID_DATA_t (*error_calc_cb)(PID_DATA_t setpoint, PID_DATA_t input);
} PID_CTX_t;

static PID_DATA_t Simple_Error(PID_DATA_t setpoint, PID_DATA_t input);
static PID_DATA_t Clamp(PID_h pid, PID_DATA_t val);

ERROR_t Pid_Init(PID_h pid, PID_CONFIG_t const * const config) {
    ASSERT(pid);
    ASSERT(config->kp >= 0);
    ASSERT(config->ki >= 0);
    ASSERT(config->kd >= 0);
    ASSERT(config->min_output < config->max_output);  

    pid->action = config->action;

    pid->p_gain = config->kp;
    pid->i_gain = config->ki;
    pid->d_gain = config->kd;

    pid->mode = config->mode;
    pid->p_on_m_weight = config->p_on_m_weight;
    
    if (config->error_calc_cb) {
        pid->error_calc_cb = config->error_calc_cb;
    }
    else {
        pid->error_calc_cb = &Simple_Error;
    }
}

ERROR_t Pid_Gain_Set(PID_h pid, PID_DATA_t kp, PID_DATA_t ki, PID_DATA_t kd, float p_on_m_weight) {
    ASSERT(kp >= 0);
    ASSERT(ki >= 0);
    ASSERT(kd >= 0);
    ASSERT(p_on_m_weight >= 0);
    ASSERT(p_on_m_weight <= 1);

    pid->p_gain = kp;
    pid->i_gain = ki;
    pid->d_gain = kd;

    pid->p_on_m_weight = p_on_m_weight;
    
    return NO_ERROR;
}

ERROR_CODE_t Pid_Error_Callback_Register(PID_h pid, PID_DATA_t (*error_calc_cb)(PID_DATA_t setpoint, PID_DATA_t input)) {
    ASSERT(pid);

    if (error_calc_cb) {
        pid->error_calc_cb = error_calc_cb;
    }
    else {
        pid->error_calc_cb = &Simple_Error;
    }

    return NO_ERROR;
}

PID_DATA_t Pid_Update(PID_h pid, PID_DATA_t input) {
    PID_DATA_t output = 0;
    
    if (pid->mode == PID_MODE_ACTIVE) {
        PID_DATA_t p_on_m_term = 0;
        
        PID_DATA_t error = pid->error_calc_cb(pid->setpoint, input);
        PID_DATA_t input_deriv = input - pid->last_input;

        if (pid->action == PID_ACTION_REVERSE) {
            input_deriv = -input_deriv;
            error = -error;
        }

        // Calculate proportional terms
        if (pid->p_on_m_weight > 0) {
            // In proportional-on-measurement mode, the proportional gain resists change
            p_on_m_term = pid->p_on_m_weight * pid->p_gain * input_deriv;
            output -= p_on_m_term;

            // should i_term be updated here for p_on_m instead?
        }
        
        if (pid->p_on_m_weight < 1) {
            // In proportional-on-error mode, the proportional gain reacts to error
            output += (1 - pid->p_on_m_weight) * pid->p_gain * error;
        }
        
        // add in integral sum
        output += pid->i_term;

        // Calculate derivative term using derivative on measurement to avoid derivative kick
        // d_term is negative due to using derivative on measurement
        output -= pid->d_gain * input_deriv;

        // Clamp to avoid windup and store
        output = Clamp(pid, output);
        pid->last_output = output;

        // Update intergral sum at the end for faster response time
        // Clamp to avoid windup
        // Store computed term to avoid bump when changing the integral gain
        pid->i_term += pid->i_gain * error;
        pid->i_term -= p_on_m_term;
        pid->i_term = Clamp(pid, pid->i_term);
    }    

    pid->last_input = input;

    return output;
}

PID_DATA_t Pid_Output_Get(PID_h pid) {
    return pid->last_output;
}

ERROR_CODE_t Pid_Override(PID_h pid, PID_DATA_t output) {
    pid->mode = PID_MODE_OVERRIDE;
    pid->last_output = output;

    return NO_ERROR;
}

ERROR_CODE_t Pid_Resume(PID_h pid) {
    // need to preload intergral term to avoid bump when exiting override mode
    if (pid->mode == PID_MODE_OVERRIDE) {
        pid->i_term = pid->last_output;
        pid->i_term = Clamp(pid, pid->i_term);
    }
    
    pid->mode = PID_MODE_ACTIVE;

    return NO_ERROR;
}

PID_MODE_t Pid_Mode_Get(PID_h pid) {
    return pid->mode;
}

ERROR_CODE_t Pid_Action_Set(PID_h pid, PID_ACTION_t action) {
    pid->action = action;

    return NO_ERROR;
}

static PID_DATA_t Simple_Error(PID_DATA_t setpoint, PID_DATA_t input) {
    return setpoint - input;
}

static inline PID_DATA_t Clamp(PID_h pid, PID_DATA_t const val) {
    if (val > pid->max_output) {
        return pid->max_output;
    }
    else if (val < pid->min_output) {
        return pid->min_output;
    } 

    return val;
}