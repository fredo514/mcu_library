# Open Mcu Library

## Instructions
### HAL
In a module that needs to use a HAL library, simply `#include` the desired HAL header file (e.g. gpio.h). 

If you need access to platform-specific functionalities that is not covered by the generic HAL API, then also `#include` the <hal_module>_ext.h header from your specific platform folder (e.g. src/hal/platform/stm32/f4/timer_ext.h).

Make sure that your compilation script (e.g. makefile) includes all the necessary directories to include all the used files. At a minimum, these are needed:
* src/utils
* src/hal
* src/hal/platform
* src/hal/platform/<path_to_specific_platform>/common
* src/hal/platform/<path_to_specific_platform>/<family>
