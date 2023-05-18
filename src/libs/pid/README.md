# General Usage
Sample at 1/10 to 1/100 of the process settling time.
* Use faster sampling rate if you have a process difficult to control, high Kd gain or need high accuracy (up to 1/1000 settling time if not using derivative).
* Sample rate should be stable (<1-5% deviation over 10 samples, >1% all the time if using derivative).

The error is computed `setpoint-input` by default (`error_callback = NULL`). If fancier error processing (e.g. filtering) is required, a custom callback can be registered to be used by `Pid_Compute()`.

For integrating processes (that always overshoot no matter the tuning), proportional-on-measurement mode can be used. Use 0 <= setpoint weigth <= 1 to use a little bit of both proportional-on-error and proportional-on-measurement for processes that are not entirely integrating.

Use `min_output` config parameter to set an offset to compensate an actuator deadband.