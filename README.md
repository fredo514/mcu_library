# Open Mcu Library

## Instructions
### HAL
In a module that needs to use a HAL library, simply `#include` the desired HAL header file (e.g. gpio.h). 

If you need access to platform-specific functionalities that is not covered by the generic HAL API, then also `#include` the <hal_module>_ext.h header from your specific platform folder (e.g. src/hal/platform/stm32/f4/timer_ext.h).

