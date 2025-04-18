#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include "pid_config.h"
#include "error.h"

// TODO: Implement additional features such as feed-forward, reset tieback, integer math,
// alternative forms, velocity control, and setpoint ramping for extended use cases.

/** Enum for defining PID controller mode */
typedef enum {
    PID_MODE_ACTIVE = 0,
    PID_MODE_OVERRIDE,
} PID_MODE_t;

/** Enum for defining action direction */
typedef enum {
    PID_ACTION_DIRECT = 0,
    PID_ACTION_REVERSE,
} PID_ACTION_t;

/** PID Controller Configuration */
typedef struct {
    PID_DATA_t kp;               ///< Proportional gain
    PID_DATA_t ki;               ///< Integral gain
    PID_DATA_t kd;               ///< Derivative gain
    PID_DATA_t alpha_deriv;      ///< Filter coefficient for derivative term
    PID_DATA_t kaw               ///< Anti-windup gain
    PID_DATA_t max_output;       ///< Maximum output value for saturation
    PID_DATA_t min_output;       ///< Minimum output value for saturation
    PID_ACTION_t action;         ///< Control action (direct or reverse)
    float proportional_on_measurement_weight; ///< Weighting for P action on measurement
    PID_MODE_t mode;             ///< Current mode of PID (active/override)
    
    /** Optional callback for error calculation */
    PID_DATA_t (*calculate_error_cb)(PID_DATA_t setpoint, PID_DATA_t input);
} PID_CONFIG_t;

/** Handle for opaque PID context */
typedef struct PID_CONTEXT const * const PID_HANDLE_t;

/** 
 * @brief Creates a new PID controller instance.
 * @return A handle to the PID instance, or NULL on failure.
 */
PID_HANDLE_t PID_Create(void);

/** 
 * @brief Initializes the PID controller with configuration parameters.
 * @param pid Handle to the PID instance.
 * @param config Configuration parameters for PID.
 * @return Error code indicating success or failure.
 */
ERROR_CODE_t PID_Init(PID_HANDLE_t pid, const PID_CONFIG_t* config);

/** 
 * @brief Registers a custom error calculation callback.
 * @param pid Handle to the PID instance.
 * @param error_calc_cb Pointer to the error calculation function.
 * @return Error code indicating success or failure.
 */
ERROR_CODE_t PID_RegisterErrorCalculation(PID_HANDLE_t pid, PID_DATA_t (*error_calc_cb)(PID_DATA_t setpoint, PID_DATA_t input));

/** 
 * @brief Sets the PID gains and proportional-on-measurement weighting.
 * @param pid Handle to the PID instance.
 * @param kp Proportional gain.
 * @param ki Integral gain.
 * @param kd Derivative gain.
 * @param p_on_m Weighting for proportional action on measurement.
 * @return Error code indicating success or failure.
 */
ERROR_CODE_t PID_SetGains(PID_HANDLE_t pid, PID_DATA_t kp, PID_DATA_t ki, PID_DATA_t kd, float p_on_m_weight);

/** 
 * @brief Sets the PID setpoint.
 * @param pid Handle to the PID instance.
 * @param setpoint Desired setpoint value.
 * @return Error code indicating success or failure.
 */
ERROR_CODE_t PID_SetSetpoint(PID_HANDLE_t pid, PID_DATA_t setpoint);

/** 
 * @brief Overrides the PID output to a specific value.
 * @param pid Handle to the PID instance.
 * @param output The override output value.
 * @return Error code indicating success or failure.
 */
ERROR_CODE_t PID_OverrideOutput(PID_HANDLE_t pid, PID_DATA_t output);

/** 
 * @brief Resumes the PID controller from override mode.
 * @param pid Handle to the PID instance.
 * @return Error code indicating success or failure.
 */
ERROR_CODE_t PID_Resume(PID_HANDLE_t pid);

/** 
 * @brief Gets the current mode of the PID controller.
 * @param pid Handle to the PID instance.
 * @return The current PID mode.
 */
PID_MODE_t PID_GetMode(PID_HANDLE_t pid);

/** 
 * @brief Sets the control action (direct or reverse).
 * @param pid Handle to the PID instance.
 * @param action Control action setting.
 * @return Error code indicating success or failure.
 */
ERROR_CODE_t PID_SetAction(PID_HANDLE_t pid, PID_ACTION_t action);

/** 
 * @brief Updates the PID output based on the input value.
 * @param pid Handle to the PID instance.
 * @param input Current input value for PID calculation.
 * @return The computed PID output.
 */
PID_DATA_t PID_Update(PID_HANDLE_t pid, PID_DATA_t input);

/** 
 * @brief Retrieves the current output of the PID controller.
 * @param pid Handle to the PID instance.
 * @return The current output value.
 */
PID_DATA_t PID_GetOutput(PID_HANDLE_t pid);

#endif // PID_CONTROLLER_H
