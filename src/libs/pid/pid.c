#include "pid.h"

struct PID_CTX {
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
    PID_DATA_t last_error;

    // for derivative
    PID_DATA_t last_input;

    // for windup Clamping
    PID_DATA_t max_output;
    PID_DATA_t min_output;

    PID_DATA_t (*error_calc_cb)(PID_DATA_t setpoint, PID_DATA_t input);
};

static PID_DATA_t Simple_Error(PID_DATA_t setpoint, PID_DATA_t input);
static PID_DATA_t Clamp(PID_h pid, PID_DATA_t val);

PID_h Pid_Create(void) {
    return calloc(1, sizeof(struct PID_CTX));
}

ERROR_t Pid_Init(PID_h pid, PID_CONFIG_t const * const config) {
    ASSERT(pid);
    ASSERT(config->kp >= 0);
    ASSERT(config->ki >= 0);
    ASSERT(config->kd >= 0);
    ASSERT(config->min_output < config->max_output); 
    if (config->kd > 0) {
        ASSERT(config->deriv_filter);
    }

    pid->action = config->action;
    pid->mode = config->mode;

    Pid_Gain_Set(pid, config->kp, config->ki, config->kd, config->alpha, config->p_on_m_weight);
    
    if (config->error_calc_cb) {
        pid->error_calc_cb = config->error_calc_cb;
    }
    else {
        pid->error_calc_cb = &Simple_Error;
    }
}

ERROR_t Pid_Gain_Set(PID_h pid, PID_DATA_t kp, PID_DATA_t ki, PID_DATA_t kd, PID_DATA_t alpha, float p_on_m_weight) {
    ASSERT(kp >= 0);
    ASSERT(ki >= 0);
    ASSERT(kd >= 0);
    ASSERT(p_on_m_weight >= 0);
    ASSERT(p_on_m_weight <= 1);

    pid->p_gain = kp;
    pid->i_gain = ki;
    pid->d_gain = kd;
    pid->alpha = alpha;

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
        
        // TODO: add optional setpoint 1st-order filter
        PID_DATA_t error = pid->error_calc_cb(pid->setpoint, input);
        PID_DATA_t input_deriv = input - pid->last_input;

        if (pid->action == PID_ACTION_REVERSE) {
            input_deriv = -input_deriv;
            error = -error;
        }

        // Calculate proportional terms
        PID_DATA_t p_term = 0;
        if (pid->p_on_m_weight > 0) {
            // In proportional-on-measurement mode, the proportional gain resists change
            p_on_m_term = pid->p_on_m_weight * pid->p_gain * input_deriv;
            p_term -= p_on_m_term;

            // should i_term be updated for p_on_m here instead?
        }
        
        if (pid->p_on_m_weight < 1) {
            // In proportional-on-error mode, the proportional gain reacts to error (normal mode)
            p_term += (1 - pid->p_on_m_weight) * pid->p_gain * error;
        }

        // Calculate derivative term 
        // using derivative on measurement to avoid derivative kick
        // low-pass filter is NOT optional. It is necessary to prevent output chatter due to input noise.
        PID_DATA_t d_term = pid->deriv_filter(pid->d_gain * input_deriv, pid->last_d_term);
        // TODO: add 1st-order filter with filter time = Td/10 to derivative term calculation
        // pid->last_deriv = pid->last_deriv + (pid->alpha_deriv * ((pid->d_gain * input_deriv) - pid->last_deriv)); // TODO: use callback to provide own filter?
        // could also use a small FIR filter such as: (TODO, replace with Fir_Filt instance)
        // pid->last_deriv[0] = pid->d_gain * (input_deriv - last_deriv[2] + 3*(last_deriv[0] - last_deriv[1])) / 6;

        // Calculate integral term
        PID_DATA_t i_term_clamped = Clamp(pid->i_term, pid->min_output - (p_term + d_term), pid->max_output - (p_term + d_term));

        // Calculate desired output
        // d_term is negative due to using derivative on measurement
        PID_DATA_t output_desired = p_term + i_term_clamped - d_term;

        // Clamp to controller limits
        PID_DATA_t output_saturated = Clamp(output_desired, pid->min_output, pid->max_output);
        // CO filter
        // Normally not needed, but could be rate-limiter, actuator limits, lowpass, etc
        if (pid->output_filter) {
            output_saturated = pid->output_filter(output_saturated, pid->last_output);
            //output = pid->last_output + ((sampling_period / (pid->alpha_co * pid->Kd / pid->Kp)) * (output - pid->last_output)); // TODO: use callback to provide own filter?
        }
        output = output_saturated;

        // Update intergral sum at the end for faster response time
        // output is saturated and integral wants to grow
        if ((output_saturated != output_desired) && ((error ^ output_desired) >= 0)) {
            // clamp integral
            integral_error = 0;
        }
        else {
            // Store computed term to avoid bump when changing the integral gain
            // Use transform to smooth sharp changes, bilinear (tustin) is acceptable (could also be standard, forward difference, backward difference, etc)
            integral_error = (error + pid->last_error) / 2;
        }

        pid->i_term += pid->i_gain * integral_error;
        // Compensate for proportional on measurement
        pid->i_term -= p_on_m_term;
        // Anti-windup
        pid->i_term += pid->kaw * (output_saturated - output_desired);

        // save states
        pid->last_error = error;
        pid->last_d_term = d_term;
        pid->last_output = output_saturated;
    } 
    else {
        output = pid->last_output;
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