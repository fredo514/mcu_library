#ifndef GPIO_DEF_H
#define GPIO_DEF_H

typedef enum {
	GPIO_PIN_0 = 0,
	GPIO_PIN_1 = 1,
	GPIO_PIN_2 = 2,
	GPIO_PIN_3 = 3,
	GPIO_PIN_4 = 4,
	GPIO_PIN_5 = 5,
	GPIO_PIN_6 = 6,
	GPIO_PIN_7 = 7,
	GPIO_PIN_8 = 8,
	GPIO_PIN_9 = 9,
	GPIO_PIN_10 = 10,
	GPIO_PIN_11 = 11,
	GPIO_PIN_12 = 12,
	GPIO_PIN_13 = 13,
	GPIO_PIN_14 = 14,
	GPIO_PIN_15 = 15,
	GPIO_PIN_MAX
} GPIO_PIN_t;

// resgiters settings are encoded in the enum value to simplify the init function
// bits: | 11 8 | 7    4 | 3   0 |
//              | otyper | moder |
typedef enum {
	GPIO_INPUT = GPIO_MODER_INPUT,

	GPIO_OUTPUT_OD = (GPIO_MODER_OUTPUT | (GPIO_OTYPER_OD << GPIO_MODE_TYPE_OFFSET)),
	GPIO_OUTPUT_PP = (GPIO_MODER_OUTPUT | (GPIO_OTYPER_PP << GPIO_MODE_TYPE_OFFSET)),

	GPIO_MODE_ALT_OD = (GPIO_MODER_ALT | GPIO_OTYPER_OD << GPIO_MODE_TYPE_OFFSET)),
	GPIO_MODE_ALT_PP = (GPIO_MODER_ALT | GPIO_OTYPER_PP << GPIO_MODE_TYPE_OFFSET)),

	GPIO_MODE_ANALOG = GPIO_MODER_ANALOG,

	// Interrupt both send signals and trigger interrupt
	GPIO_MODE_IT_RISE = (GPIO_MODE_INPUT | (GPIO_MODE_EXTI_IT << GPIO_MODE_EXTI_OFFSET) | (GPIO_MODE_TRIG_RISE << GPIO_MODE_TRIG_EDGE_OFFSET)),     
	GPIO_MODE_IT_FALL = (GPIO_MODE_INPUT | (GPIO_MODE_EXTI_IT << GPIO_MODE_EXTI_OFFSET) | (GPIO_MODE_TRIG_FALL << GPIO_MODE_TRIG_EDGE_OFFSET)),   
	GPIO_MODE_IT_RISE_FALL = (GPIO_MODE_INPUT | (GPIO_MODE_EXTI_IT << GPIO_MODE_EXTI_OFFSET) | ((GPIO_MODE_TRIG_RISE | GPIO_MODE_TRIG_FALL) << GPIO_MODE_TRIG_EDGE_OFFSET)),
	
    // Events send signals but don't trigger interrupt
	GPIO_MODE_EVT_RISE = (GPIO_MODE_INPUT | (GPIO_MODE_EXTI_EVT << GPIO_MODE_EXTI_OFFSET) | (GPIO_MODE_TRIG_RISE << GPIO_MODE_TRIG_EDGE_OFFSET)),     
	GPIO_MODE_EVT_FALL = (GPIO_MODE_INPUT | (GPIO_MODE_EXTI_EVT << GPIO_MODE_EXTI_OFFSET) | (GPIO_MODE_TRIG_FALL << GPIO_MODE_TRIG_EDGE_OFFSET)),   
	GPIO_MODE_EVT_RISE_FALL = (GPIO_MODE_INPUT | (GPIO_MODE_EXTI_EVT << GPIO_MODE_EXTI_OFFSET) | ((GPIO_MODE_TRIG_RISE | GPIO_MODE_TRIG_FALL) << GPIO_MODE_TRIG_EDGE_OFFSET)),
	GPIO_MODE_MAX
} GPIO_MODE_t;

typedef enum {
	GPIO_SPEED_SLOW = 0x2,
	GPIO_SPEED_MODERATE = 0x1,
	GPIO_SPEED_FAST = 0x3,
	GPIO_DRIVE_MAX
} GPIO_SPEED_t;

typedef enum {
	GPIO_NO_PULL,
	GPIO_PULL_UP,
	GPIO_PULL_DOWN,
	GPIO_PULL_MAX
} GPIO_PULL_t;

typedef enum {
	// see datasheet for part and STM32CUBE hal
	GPIO_ALT_0,
	GPIO_ALT_1 = 0x1,
	GPIO_ALT_2 = 0x2,
	GPIO_ALT_3 = 0x3,
	GPIO_ALT_4 = 0x4,
	GPIO_ALT_5 = 0x5,
	GPIO_ALT_6 = 0x6,
	GPIO_ALT_7 = 0x7,

	GPIO_ALT_EVENTOUT   = GPIO_ALT_0,  // AF0: EVENTOUT Alternate Function mapping  
	GPIO_ALT_MCO        = GPIO_ALT_0,  // AF0: MCO Alternate Function mapping       
	GPIO_ALT_SPI1       = GPIO_ALT_0,  // AF0: SPI1/I2S1 Alternate Function mapping 
	GPIO_ALT_SPI2       = GPIO_ALT_0,  // AF0: SPI2 Alternate Function mapping      
	GPIO_ALT_TIM15      = GPIO_ALT_0,  // AF0: TIM15 Alternate Function mapping     
	GPIO_ALT_TIM17      = GPIO_ALT_0,  // AF0: TIM17 Alternate Function mapping     
	GPIO_ALT_SWDIO      = GPIO_ALT_0,  // AF0: SWDIO Alternate Function mapping     
	GPIO_ALT_SWCLK      = GPIO_ALT_0,  // AF0: SWCLK Alternate Function mapping     
	GPIO_ALT_TIM14      = GPIO_ALT_0,  // AF0: TIM14 Alternate Function mapping     
	GPIO_ALT_USART1     = GPIO_ALT_0,  // AF0: USART1 Alternate Function mapping    
	GPIO_ALT_IR         = GPIO_ALT_0,  // AF0: IR Alternate Function mapping        
	GPIO_ALT_CEC        = GPIO_ALT_0,  // AF0: CEC Alternate Function mapping    

	GPIO_ALT_TIM3       = GPIO_ALT_1,  // AF1: TIM3 Alternate Function mapping      
	GPIO_ALT_TIM15      = GPIO_ALT_1,  // AF1: TIM15 Alternate Function mapping     
	GPIO_ALT_USART1     = GPIO_ALT_1,  // AF1: USART1 Alternate Function mapping    
	GPIO_ALT_USART2     = GPIO_ALT_1,  // AF1: USART2 Alternate Function mapping    
	GPIO_ALT_EVENTOUT   = GPIO_ALT_1,  // AF1: EVENTOUT Alternate Function mapping  
	GPIO_ALT_I2C1       = GPIO_ALT_1,  // AF1: I2C1 Alternate Function mapping      
	GPIO_ALT_I2C2       = GPIO_ALT_1,  // AF1: I2C2 Alternate Function mapping      
	GPIO_ALT_IR         = GPIO_ALT_1,  // AF1: IR Alternate Function mapping        
	GPIO_ALT_CEC        = GPIO_ALT_1,  // AF1: CEC Alternate Function mapping     

	GPIO_ALT_TIM1       = GPIO_ALT_2,  // AF2: TIM1 Alternate Function mapping      
	GPIO_ALT_TIM2       = GPIO_ALT_2,  // AF2: TIM2 Alternate Function mapping      
	GPIO_ALT_TIM16      = GPIO_ALT_2,  // AF2: TIM16 Alternate Function mapping     
	GPIO_ALT_TIM17      = GPIO_ALT_2,  // AF2: TIM17 Alternate Function mapping     
	GPIO_ALT_EVENTOUT   = GPIO_ALT_2,  // AF2: EVENTOUT Alternate Function mapping  

	GPIO_ALT_EVENTOUT   = GPIO_ALT_3,  // AF3: EVENTOUT Alternate Function mapping  
	GPIO_ALT_I2C1       = GPIO_ALT_3,  // AF3: I2C1 Alternate Function mapping      
	GPIO_ALT_TIM15      = GPIO_ALT_3,  // AF3: TIM15 Alternate Function mapping     
	GPIO_ALT_TSC        = GPIO_ALT_3,  // AF3: TSC Alternate Function mapping     

	GPIO_ALT_TIM14      = GPIO_ALT_4,  // AF4: TIM14 Alternate Function mapping     

	GPIO_ALT_TIM16      = GPIO_ALT_5,  // AF5: TIM16 Alternate Function mapping     
	GPIO_ALT_TIM17      = GPIO_ALT_5,  // AF5: TIM17 Alternate Function mapping     

	GPIO_ALT_EVENTOUT   = GPIO_ALT_6,  // AF6: EVENTOUT Alternate Function mapping  

	GPIO_ALT_COMP1      = GPIO_ALT_7,  // AF7: COMP1 Alternate Function mapping     
	GPIO_ALT_COMP2      = GPIO_ALT_7,   // AF7: COMP2 Alternate Function mapping       
	
	GPIO_ALT_FUNC_MAX
} GPIO_ALT_FUNCTION_t;

typedef struct {
  GPIO_TypeDef* port;
  gpio_pin_t pin;
} gpio_t;

typedef struct {
  gpio_state_t initial_state;
  gpio_mode_t mode;       // Specifies the operating mode for the selected pins
  gpio_pull_type_t pull;  // Specifies the Pull-up or Pull-Down activation for
                          // the selected pins
  gpio_speed_t speed;
  gpio_alt_function_t
      alt_function;  // Peripheral to be connected to the selected pins if
                     // alternate function mode is selected
} gpio_config_t;

#endif // GPIO_DEF_H