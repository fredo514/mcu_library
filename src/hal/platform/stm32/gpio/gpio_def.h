#ifndef GPIO_DEF_H
#define GPIO_DEF_H

typedef enum {
	GPIO_OUTPUT,
	GPIO_INPUT,
	GPIO_MODE_ALT_FUNC,
	GPIO_MODE_ANALOG,
	GPIO_MODE_MAX
} GPIO_MODE;

typedef enum {
	GPIO_PUSH_PULL,
	GPIO_OPEN_DRAIN,
	GPIO_DRIVE_MAX
} GPIO_DRIVE;

typedef enum {
	GPIO_NO_PULL,
	GPIO_PULL_UP,
	GPIO_PULL_DOWN,
	GPIO_PULL_MAX
} GPIO_PULL;

typedef enum {
	// see datasheet for part and STM32CUBE hal
	GPIO_ALT_FUNC_MAX
} GPIO_ALT_FUNCTION;

typedef enum {
	// Interrupt both send signals and trigger interrupt
	GPIO_INT_RISING          = (0x0UL << 0U | (0x1UL << 16U) | (0x1UL << 20U)),               // Rising edge trigger External Interrupt      
	GPIO_INT_FALLING         = (0x0UL << 0U | (0x1UL << 16U) | (0x2UL << 20U)),               // Falling edge trigger External Interrupt    
	GPIO_INT_RISING_FALLING  = (0x0UL << 0U | (0x1UL << 16U) | (0x1UL << 20U) | (0x2UL << 20U)),   // Rising/Falling edge trigger External Interrupt
	
    // Events send signals but don't trigger interrupt
	GPIO_EVENT_RISING         = (0x0UL << 0U | (0x2UL << 16U) | (0x1UL << 20U)),                    // Rising edge trigger External Event Mode
	GPIO_EVENT_FALLING        = (0x0UL << 0U | (0x2UL << 16U) | (0x2UL << 20U)),                    // Falling edge trigger External Event Mode
	GPIO_EVENT_RISING_FALLING = (0x0UL << 0U | (0x2UL << 16U) | (0x1UL << 20U) | (0x2UL << 20U))    // Rising/Falling edge trigger External Event Mode
} GPIO_INT_MODE;

#endif // GPIO_DEF_H