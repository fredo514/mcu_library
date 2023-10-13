# Open Mcu Library

**THIS PROJECT IS STILL A WORK IN PROGRESS**
Please feel free to submit pull requests to improve it!

## General
This library was created with the intent to provide a platform-independent and lean API, much like the Arduino framework but in plain C. Such libraries already exist for individual platforms, such as Arm Cortex (e.g. libopencm3) or Microchip PIC (Harmony), but I couldn't find a good one that covers multiple platforms.

The Open MCU Library currently supports:
* Microchip AVR
* Microchip PIC18
* Microchip SAM
* Silicon Labs EFM8
* NXP LPC
* Nordic NRF
* STMicroelectronics STM32

It is split in groups:
* **HAL**: Hardware abstraction layer for the different peripherals for each platform
* **Utils**: Platform-independant utility modules
* **Libs**: Common application level libraries
* **Drivers**: Common external devices drivers

Although not directly the goal of this project, drivers for external devices or common stacks and general libraries using the Open MCU Library HAL are also included (TODO: split into a sepatate repo?).

## How To Use This Library
### HAL
In a module that needs to use a HAL library, simply `#include` the desired HAL header file (e.g. gpio.h). 

If you need access to platform-specific functionalities that is not covered by the generic HAL API, then also `#include` the <hal_module>_ext.h header from your specific platform folder (e.g. src/hal/platform/stm32/f4/timer_ext.h), or use the direct register access API from the base HAL to implement the functionality manually at the application level.

Make sure that your compilation script (e.g. makefile) includes all the necessary directories to include all the used files. At a minimum, these are needed:
* src/utils
* src/hal
* src/hal/platform
* src/hal/platform/<path_to_specific_platform>/common
* src/hal/platform/<path_to_specific_platform>/<family>/<used_module>
