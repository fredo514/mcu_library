#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include "pid_config.h"
#include "error.h"
#include "pid_priv.h"

// TODO: Implement additional features such as feed-forward, reset tieback, integer math,
// alternative forms, velocity control, and setpoint ramping for extended use cases.

/** Enum for defining PID controller mode */
typedef enum {
    PID_MODE_ACTIVE = 0,
    PID_MODE_OVERRIDE,
    PID_MODE_MAX
} pid_mode_t;

/** Enum for defining action direction */
typedef enum {
    PID_ACTION_DIRECT = 0,
    PID_ACTION_REVERSE,
    PID_ACTION_MAX
} pid_action_t;

/** PID Controller Configuration */
typedef struct {
    pid_data_t kp;               ///< Proportional gain
    pid_data_t ki;               ///< Integral gain
    pid_data_t kd;               ///< Derivative gain
    // pid_data_t K;      // continuous proportional gain
    // pid_data_t Ti;     // integral time (s)
    // pid_data_t Td;     // derivative time (s)
    // pid_data_t Ts;     // sampling period (s)
    pid_data_t alpha_deriv;      ///< Filter coefficient for derivative term
    pid_data_t kaw               ///< Anti-windup gain
    pid_data_t max_output;       ///< Maximum output value for saturation
    pid_data_t min_output;       ///< Minimum output value for saturation
    pid_action_t action;         ///< Control action (direct or reverse)
    float proportional_on_measurement_weight; ///< Weighting for P action on measurement
    pid_mode_t mode;             ///< Current mode of PID (active/override)
    
    /** Optional callback for error calculation */
    pid_data_t (*calculate_error_cb)(pid_data_t setpoint, pid_data_t input);
} pid_config_t;

/** Handle for opaque PID context */
typedef struct pid_ctx pid_t;

typedef pid_data_t (*pid_error_calc_cb_t)(pid_data_t setpoint, pid_data_t input)

/** 
 * @brief Initializes the PID controller with configuration parameters.
 * @param pid Handle to the PID instance.
 * @param config Configuration parameters for PID.
 * @return Error code indicating success or failure.
 */
void Pid_Init(pid_t * const pid,  pid_config_t const * const config);

/** 
 * @brief Registers a custom error calculation callback.
 * @param pid Handle to the PID instance.
 * @param error_calc_cb Pointer to the error calculation function.
 * @return Error code indicating success or failure.
 */
error_t Pid_Error_Callback_Register(pid_t * const pid, pid_error_calc_cb_t const error_calc_cb);

/** 
 * @brief Sets the PID gains and proportional-on-measurement weighting.
 * @param pid Handle to the PID instance.
 * @param kp Proportional gain.
 * @param ki Integral gain.
 * @param kd Derivative gain.
 * @param p_on_m Weighting for proportional action on measurement.
 * @return Error code indicating success or failure.
 */
void Pid_Gain_Set(pid_t * const pid, pid_data_t const kp, pid_data_t const ki, pid_data_t const kd, pid_data_t const alpha, float const p_on_m_weight);

/** 
 * @brief Sets the PID setpoint.
 * @param pid Handle to the PID instance.
 * @param setpoint Desired setpoint value.
 * @return Error code indicating success or failure.
 */
error_t Pid_Setpoint_Set(pid_t * const pid, pid_data_t const setpoint);

/** 
 * @brief Overrides the PID output to a specific value.
 * @param pid Handle to the PID instance.
 * @param output The override output value.
 * @return Error code indicating success or failure.
 */
pid_data_t Pid_Override(pid_t * const pid);

/** 
 * @brief Resumes the PID controller from override mode.
 * @param pid Handle to the PID instance.
 * @return Error code indicating success or failure.
 */
void Pid_Resume(pid_t * const pid);

/** 
 * @brief Gets the current mode of the PID controller.
 * @param pid Handle to the PID instance.
 * @return The current PID mode.
 */
pid_mode_t Pid_Mode_Get(pid_t * const pid);

/** 
 * @brief Sets the control action (direct or reverse).
 * @param pid Handle to the PID instance.
 * @param action Control action setting.
 * @return Error code indicating success or failure.
 */
error_t Pid_Action_Set(pid_t * const pid, pid_action_t const action);

/** 
 * @brief Sets the control output limits for saturation.
 * @param pid Handle to the PID instance.
 * @param min_output Minimum allowed control output.
 * @param max_output Maximum allowed control output.
 * @return Error code indicating success or failure.
 */
error_t Pid_Output_Limits_Set(pid_t * const pid, pid_data_t const min_output, pid_data_t const max_output);

/** 
 * @brief Updates the PID output based on the input value.
 * @param pid Handle to the PID instance.
 * @param input Current input value for PID calculation.
 * @return The computed PID output.
 */
pid_data_t Pid_Update(pid_t * const pid, pid_data_t const input);

/** 
 * @brief Retrieves the current output of the PID controller.
 * @param pid Handle to the PID instance.
 * @return The current output value.
 */
pid_data_t Pid_Output_Get(pid_t * const pid);

#endif // PID_CONTROLLER_H
