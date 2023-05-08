# General Usage
Sample at 1/10 to 1/100 of the process settling time.
* Use faster sampling rate if you have a process difficult to control, high Kd gain or need high accuracy (up to 1/1000 settling time if not using derivative).
* Sample rate should be stable (<1-5% deviation over 10 samples, >1% all the time if using derivative).

Error must be pre-computed and fed to `Pid_Update()`. It is generally `command-input`. This implementation allows for fancier error processing (e.g. filtering) if required.