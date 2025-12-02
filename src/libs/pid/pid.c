#include "pid.h"
#include "assert.h"

static pid_data_t Simple_Error(pid_data_t setpoint, pid_data_t input);
static inline pid_data_t Clamp(pid_t * const pid, pid_data_t const val);

void Pid_Init(pid_t * const pid,  pid_config_t const * const config) {
    assert(pid);
    assert(config->kp >= 0);
    assert(config->ki >= 0);
    assert(config->kd >= 0);
    assert(config->min_output < config->max_output); 
    if (config->kd > 0) {
        assert(config->deriv_filter);
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

void Pid_Gain_Set(pid_t * const pid, pid_data_t const kp, pid_data_t const ki, pid_data_t const kd, pid_data_t const alpha, float const p_on_m_weight) {
    assert(pid);
    assert(kp >= 0);
    assert(ki >= 0);
    assert(kd >= 0);
    assert(p_on_m_weight >= 0);
    assert(p_on_m_weight <= 1);

    pid->p_gain = kp;
    pid->i_gain = ki;
    pid->d_gain = kd;
    pid->alpha = alpha;

    pid->p_on_m_weight = p_on_m_weight;
}

error_t Pid_Error_Callback_Register(pid_t * const pid, pid_error_calc_cb_t const error_calc_cb) {
    assert(pid);

    if (error_calc_cb) {
        pid->error_calc_cb = error_calc_cb;
    }
    else {
        pid->error_calc_cb = &Simple_Error;
    }

    return NO_ERROR;
}

pid_data_t Pid_Update(pid_t * const pid, pid_data_t const input) {
    assert(pid);

    pid_data_t output = 0;
    
    if (pid->mode == PID_MODE_ACTIVE) {
        pid_data_t p_on_m_term = 0;
        
        // TODO: add optional setpoint 1st-order filter
        pid_data_t error = pid->error_calc_cb(pid->setpoint, input);
        pid_data_t input_deriv = input - pid->last_input;

        if (pid->action == PID_ACTION_REVERSE) {
            input_deriv = -input_deriv;
            error = -error;
        }

        // Calculate proportional terms
        pid_data_t p_term = 0;
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
        pid_data_t d_term = pid->deriv_filter(pid->d_gain * input_deriv, pid->last_d_term);
        // TODO: add 1st-order filter with filter time = Td/10 to derivative term calculation
        // pid->last_deriv = pid->last_deriv + (pid->alpha_deriv * ((pid->d_gain * input_deriv) - pid->last_deriv)); // TODO: use callback to provide own filter?
        // could also use a small FIR filter such as: (TODO, replace with Fir_Filt instance)
        // pid->last_deriv[0] = pid->d_gain * (input_deriv - last_deriv[2] + 3*(last_deriv[0] - last_deriv[1])) / 6;

        // Calculate integral term
        pid_data_t i_term_clamped = Clamp(pid->i_term, pid->min_output - (p_term + d_term), pid->max_output - (p_term + d_term));

        // Calculate desired output
        // d_term is negative due to using derivative on measurement
        pid_data_t output_desired = p_term + i_term_clamped - d_term;

        // Clamp to controller limits
        pid_data_t output_saturated = Clamp(output_desired, pid->min_output, pid->max_output);
        // CO filter
        // Normally not needed, but could be rate-limiter, actuator limits, lowpass, etc
        if (pid->output_filter) {
            output_saturated = pid->output_filter(output_saturated, pid->last_output);
            //output = pid->last_output + ((sampling_period / (pid->alpha_co * pid->Kd / pid->Kp)) * (output - pid->last_output)); // TODO: use callback to provide own filter?
        }
        output = output_saturated;

        // Update intergral sum at the end for faster response time
        // output is saturated and integral wants to grow
        if ((output_saturated != output_desired) && ((error * output_desired) >= 0)) {
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

pid_data_t Pid_Output_Get(pid_t * const pid) {
    assert(pid);

    return pid->last_output;
}

pid_data_t Pid_Override(pid_t * const pid) {
    assert(pid);

    pid->mode = PID_MODE_OVERRIDE;
    pid->last_output = output;

    return pid->last_output;
}

void Pid_Resume(pid_t * const pid) {
    assert(pid);
    
    // need to preload intergral term to avoid bump when exiting override mode
    if (pid->mode == PID_MODE_OVERRIDE) {
        pid->i_term = pid->last_output;
        pid->i_term = Clamp(pid, pid->i_term);
    }
    
    pid->mode = PID_MODE_ACTIVE;
}

pid_mode_t Pid_Mode_Get(pid_t * const pid) {
    assert(pid);
    
    return pid->mode;
}

void Pid_Action_Set(pid_t * const pid, pid_action_t const action) {
    assert(pid);
    assert(action < PID_ACTION_MAX);
    
    pid->action = action;
}

error_t Pid_Output_Limits_Set(pid_t * const pid, pid_data_t const min_output, pid_data_t const max_output) {
    assert(pid);
}

error_t Pid_Setpoint_Set(pid_t * const pid, pid_data_t const setpoint){
    assert(pid);
}

static pid_data_t Simple_Error(pid_data_t setpoint, pid_data_t input) {
    return setpoint - input;
}

static inline pid_data_t Clamp(pid_t * const pid, pid_data_t const val) {
    assert(pid);
    
    if (val > pid->max_output) {
        return pid->max_output;
    }
    else if (val < pid->min_output) {
        return pid->min_output;
    } 

    return val;
}