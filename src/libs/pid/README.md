# Introduction
The algorithm implemented is the independant form PID with controller output filter.

## Hightlights
* Proportional on error + proportional on measurement blending
* Derivative on measurement to avoid derivative kick
* Mandatory derivative filtering (simple or callback) to prevent control output jitter due to input noise
* Support direct and reverse actuator action
* Trapezoidal integration
* Anti-windup using tunable back-calculation for faster saturation recovery
* Optional control output filter
* Override/resume mode with integral preloading for bumpless transfer

# Usage
Sample at 1/10 to 1/100 of the process settling time.
* Use faster sampling rate if you have a process difficult to control, high Kd gain or need high accuracy (up to 1/1000 settling time if not using derivative).
* Sample rate should be stable (<1-5% deviation over 10 samples, >1% all the time if using derivative). Using an periodic ADC interrupt to sample the process variable is a good starting point.

The error is computed `setpoint-input` by default (`error_callback = NULL`). If fancier error processing (e.g. filtering) is required, a custom callback can be registered to be used by `Pid_Compute()`.

For integrating processes (that always overshoot no matter the tuning), proportional-on-measurement mode can be used. Use 0 <= setpoint weigth <= 1 to use a little bit of both proportional-on-error and proportional-on-measurement for processes that are not entirely integrating.

Use `min_output` config parameter to inject a bias to compensate any constant offset (e.g. actuator deadband).

Ideally, call `Pid_Update()` from the ADC interrupt. The result should also be applied to the actuator immediately. 

All the measured variables have to be thoroughly filtered. They should go through a proper hardware anti-aliasing filter, with the cut-off frequency well below sampling frequency/2. If, for some reason, they cannot be filtered by an analog filter, they should be over sampled and filtered digitally.

The input needs to be sanitized before calling. E.g. out of range or nonsensical values due to sensor disconnect, ADC errors, etc.

# Tuning
PI is sufficient most of the time (Kd=0, a=0).
* Use Derivative for higher frequency damping (e.g. aggressive tuning overshoot) and  dead-time.
> Do **NOT** use derivative in a system where the input is inherently noisy!

CO filter is rarely needed. Only potential for benefit in loops with noise and/or delicate mechanical actuators.
> **NOTE** CO filter reponse will add to the controller frequency response. Carefully analyze the total combined loop response.

kp, ki, kd are already discretized (e.g. ki = K * Ts / Ti, kd = K * Td / Ts) and Ts is assumed constant.

## Derivative Filter Tuning
Carefully choose the derivative filter gain. Too high and it will lead to overshoot, and too low will be too weak to have the desired chatter-reducing effect.

## Anti-windup Tuning
A common and effective starting point is:
```math
K_{aw} = \frac{1}{K_{i}\cdot \tau}
```
Where:
* $K_{i}$ is the integral gain,
* $\tau$ is a time constant, usually selected based on the actuator or system time response (e.g., ~0.1 to 1.0 × the plant’s time constant).

1. Start with $\tau = T$, where $T$ is the controller's sampling time in seconds.
2. Test (or simulate) the response to a step input causing controller output saturation.
3. Adjust as needed:
    * If unwind is too slow → increase $K_{aw}$ (more aggressive correction).
    * If you get oscillation or instability → decrease $K_{aw}$ (smoother correction).

## Proportional on Measurement Tuning
Use Proportional-on-Measurement for lower frequency damping (e.g. integrating process overshooting).
* In an integrating process, the output needs to return to similar value after a setpoint change, i.e. integral is the primary steady-state contributor.
> **NOTE** Proportional on Measurement can introduce additional phase lead and reduce phase margin. Use only when necessary.

Propotionbal on Measurement weigth accepts values between 0 and 1. Set to 0 for proportional term to react only on error (default, standard PID).

# TODO (Planned Enhancements)
These features are intentionally not implemented yet but are planned:
* Add analysis (graphs) to prove correctness and robustness (override/resume, gain change, saturation, various plants: normal, dead-time, slow, fast, integrating, etc)
    * Discrete-time root locus / Bode of the closed loop
    * Monte-carlo varying factors (alpha_deriv, kaw, p_on_m_weight, etc)