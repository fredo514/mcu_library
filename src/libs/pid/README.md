# Introduction
The algorithm implemented is the independant form PID with controller output filter.

# Usage
Sample at 1/10 to 1/100 of the process settling time.
* Use faster sampling rate if you have a process difficult to control, high Kd gain or need high accuracy (up to 1/1000 settling time if not using derivative).
* Sample rate should be stable (<1-5% deviation over 10 samples, >1% all the time if using derivative). Using an periodic ADC interrupt to sample the process variable is a good starting point.

The error is computed `setpoint-input` by default (`error_callback = NULL`). If fancier error processing (e.g. filtering) is required, a custom callback can be registered to be used by `Pid_Compute()`.

For integrating processes (that always overshoot no matter the tuning), proportional-on-measurement mode can be used. Use 0 <= setpoint weigth <= 1 to use a little bit of both proportional-on-error and proportional-on-measurement for processes that are not entirely integrating.

Use `min_output` config parameter to inject a bias to compensate any constant offset (e.g. actuator deadband).

Ideally, call `Pid_Update()` from the ADC interrupt. The result should also be applied to the actuator immediately. 

All the measured variables have to be thoroughly filtered. They should go through a proper hardware anti-aliasing filter, with the cut-off frequency well below sampling frequency/2. If, for some reason, they cannot be filtered by an analog filter, they should be over sampled and filtered digitally.

# Tuning
PI is sufficient most of the time (Kd=0, a=0).
PID can sometimes help get marginally better performance when tuning aggressively.
CO filter is rarely needed. Only potential for benefit in loops with noise and/or delicate mechanical actuators.

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
Propotionbal on Measurement weigth accepts values between 0 and 1.

Set to 0 for proportional term to react only on error (default, standard PID).

Increase for proportional term to also react to input rate of change (resist change). This is useful for integrating processes. 
