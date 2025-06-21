// use preprocessor defines to include the correct platform hal file
#ifdef STM32F415xx
#include "stm32f415xx.h"
#else
#error Unknown platform
#endif

// for each platform, define:
// processor data bus size
// endianess
// aligned access (for casting)

ERROR_CODE_t Platform_Init(void);