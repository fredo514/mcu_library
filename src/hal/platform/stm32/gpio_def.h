#ifndef GPIO_DEF_H
#define GPIO_DEF_H

typedef enum {
	GPIO_MODE_INPUT              =  0x0UL << 0U,                                                  // Input Floating Mode
	GPIO_MODE_OUTPUT		     =  0x1UL,
	GPIO_MODE_OUTPUT_PP          = (0x1UL << 0U | (0x0UL << 4U)),                                 // Output Push Pull Mode
	GPIO_MODE_OUTPUT_OD          = (0x1UL << 0U | (0x1UL << 4U)),                                 // Output Open Drain Mode
	GPIO_MODE_AF_PP              = (0x2UL << 0U | (0x0UL << 4U)),                                 // Alternate Function Push Pull Mode
	GPIO_MODE_AF_OD              = (0x2UL << 0U | (0x1UL << 4U)),                                 // Alternate Function Open Drain Mode

	GPIO_MODE_ANALOG             = 0x3UL << 0U,                                                	 // Analog Mode
	
    // Interrupt both send signals and trigger interrupt
	GPIO_MODE_IT_RISING          = (0x0UL << 0U | (0x1UL << 16U) | (0x1UL << 20U)),               // Rising edge trigger External Interrupt      
	GPIO_MODE_IT_FALLING         = (0x0UL << 0U | (0x1UL << 16U) | (0x2UL << 20U)),               // Falling edge trigger External Interrupt    
	GPIO_MODE_IT_RISING_FALLING  = (0x0UL << 0U | (0x1UL << 16U) | (0x1UL << 20U) | (0x2UL << 20U)),   // Rising/Falling edge trigger External Interrupt
	
    // Events send signals but don't trigger interrupt
	GPIO_MODE_EVT_RISING         = (0x0UL << 0U | (0x2UL << 16U) | (0x1UL << 20U)),                    // Rising edge trigger External Event Mode
	GPIO_MODE_EVT_FALLING        = (0x0UL << 0U | (0x2UL << 16U) | (0x2UL << 20U)),                    // Falling edge trigger External Event Mode
	GPIO_MODE_EVT_RISING_FALLING = (0x0UL << 0U | (0x2UL << 16U) | (0x1UL << 20U) | (0x2UL << 20U))    // Rising/Falling edge trigger External Event Mode
} GPIO_MODE;

typedef enum {
	GPIO_NOPULL     = 0x0U,
	GPIO_PULLUP     = 0x1U,
	GPIO_PULLDOWN   = 0x2U
} GPIO_PULL;

typedef enum {
	// see datasheet for part and STM32CUBE hal
} GPIO_ALT_FUNCTION;

#endif // GPIO_DEF_H