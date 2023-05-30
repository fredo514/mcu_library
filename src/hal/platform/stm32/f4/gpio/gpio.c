#include "gpio.h"
#include "stm32_hal.h"
#include "assert.h"

typedef struct GPIO_CTX {
    GPIO_REGS_t const * const port;
    GPIO_PIN_t const pin;
};

GPIO_h Gpio_Create(GPIO_PORT_t const * const port, GPIO_PIN_t const pin) {
    GPIO_h inst = calloc(1, sizeof(struct GPIO_CTX));
    inst->port = port;
    inst->pin = pin;

    return inst;
}

ERROR_CODE_t Gpio_Init(GPIO_h gpio, GPIO_CONFIG_t const * const config) {
    ASSERT(config);   // config exists
    ASSERT(gpio);   // gpio exists
    ASSERT((gpio->pin >= 0) && (gpio->pin < GPIO_PIN_MAX));   // only 16 pins per port

    // Find the clock enable bit for the gpio port
    // NOTE: Cannot be tested with mock register structure besause case will not evaluate to constant.
    uint8_t port_index = 0;
    switch ((int)gpio->port) {
        case (int)GPIO_PORTA:
            port_index = 0;
        break;

        case (int)GPIO_PORTB:
            port_index = 1;
        break; 

        case (int)GPIO_PORTC:
            port_index = 2;
        break; 

        case (int)GPIO_PORTD:
            port_index = 3;
        break; 

        case (int)GPIO_PORTF:
            port_index = 5;
        break; 

        default:
            return ERROR_NO_DEV;
    }

    // enable clock for the gpio port
    Core_Reg_Write(RCC->AHBENR, SET_MASK(RCC->AHBENR, 1 << port_index));

    // configure gpio registers
    if (((config->mode & GPIO_MODE_MASK) == GPIO_MODE_OUTPUT) || ((config->mode & GPIO_MODE_MASK) == GPIO_MODE_ALT)) {
        // output speed option is only available for output or alternate mode
        Gpio_Reg_Write(gpio->port->OSPEEDR, CLEAR_MASK(gpio->port->OSPEEDR, GPIO_OSPEEDR_MASK << (gpio->pin * 2)));       // clear the bits first
        Gpio_Reg_Write(gpio->port->OSPEEDR, SET_MASK(gpio->port->OSPEEDR, config->speed << (gpio->pin * 2)));

        // output type option is only available for output or alternate mode
        Gpio_Reg_Write(gpio->port->OTYPER, SET_MASK(gpio->port->OTYPER, ((config->mode >> GPIO_MODE_TYPE_OFFSET) & GPIO_MODE_MASK) << gpio->pin));
    }

    if (config->mode != GPIO_MODER_ANALOG) {
        // pull-up/down is not available for analog input mode
        Gpio_Reg_Write(gpio->port->PUPDR, CLEAR_MASK(gpio->port->PUPDR, GPIO_PUPDR_MASK << (gpio->pin * 2)));       // clear the bits first
        Gpio_Reg_Write(gpio->port->PUPDR, SET_MASK(gpio->port->PUPDR, config->pull << (gpio->pin * 2)));
    }

    if ((config->mode & GPIO_MODE_MASK) == GPIO_MODE_ALT) {
        // configure alternate function
        Gpio_Reg_Write(gpio->port->AFR[gpio->pin >> 3], \
                        CLEAR_MASK(gpio->port->AFR[gpio->pin >> 3], GPIO_AFR_MASK << ((gpio->pin & 0x7) * 4)));       // clear the bits first
        Gpio_Reg_Write(gpio->port->AFR[gpio->pin >> 3], \
                        SET_MASK(gpio->port->AFR[gpio->pin >> 3], config->alt_function << ((gpio->pin & 0x7) * 4)));
    }

    // set the gpio mode
    Gpio_Reg_Write(gpio->port->MODER, CLEAR_MASK(gpio->port->MODER, GPIO_MODER_MASK << (gpio->pin * 2)));       // clear the bits first
    Gpio_Reg_Write(gpio->port->MODER, SET_MASK(gpio->port->MODER, config->mode & GPIO_MODE_MASK << (gpio->pin * 2)));

    if ((config->mode >> GPIO_MODE_EXTI_OFFSET) & GPIO_MODE_MASK) {
        // configure external interrupt or event
        // enable syscfg clock
        Gpio_Reg_Write(RCC->APB2ENR, SET_MASK(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN));

        // attach exti to gpio port
        Gpio_Reg_Write(SYSCFG->EXTICR[gpio->pin >> 2], CLEAR_MASK(SYSCFG->EXTICR[gpio->pin >> 2], SYSCFG_EXTICR_MASK << ((gpio->pin & 0x7) * 4)));       // clear the bits first
        Gpio_Reg_Write(SYSCFG->EXTICR[gpio->pin >> 2], SET_MASK(SYSCFG->EXTICR[gpio->pin >> 2], port_index << ((gpio->pin & 0x7) * 4)));

        // set the rising/falling trigger configutation
        if (((config->mode >> GPIO_MODE_TRIG_EDGE_OFFSET) & GPIO_MODE_TRIG_RISE) == GPIO_MODE_TRIG_RISE) {
            Gpio_Reg_Write(EXTI->RTSR, SET_MASK(EXTI->RTSR, 1 << gpio->pin));
        }

        if (((config->mode >> GPIO_MODE_TRIG_EDGE_OFFSET) & GPIO_MODE_TRIG_FALL) == GPIO_MODE_TRIG_FALL) {
            Gpio_Reg_Write(EXTI->FTSR, SET_MASK(EXTI->FTSR, 1 << gpio->pin));
        }

        if (((config->mode >> GPIO_MODE_EXTI_OFFSET) & GPIO_MODE_MASK) == GPIO_MODE_EXTI_EVT) {
            Gpio_Reg_Write(EXTI->EMR, SET_MASK(EXTI->EMR, 1 << gpio->pin));
        }

        if (((config->mode >> GPIO_MODE_EXTI_OFFSET) & GPIO_MODE_MASK) == GPIO_MODE_EXTI_IT) {
            Gpio_Reg_Write(EXTI->IMR, SET_MASK(EXTI->IMR, 1 << gpio->pin));
        }
    }

    return SUCCESS;
}

GPIO_STATE_t Gpio_Read(GPIO_h gpio)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < GPIO_PIN_MAX));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as input?
    
    // if pin'th bit of the port's IDR register is set, then return SET. Otherwise, return RESET
    // Done using ternary operator because it generates the most efficient assembly
    return (gpio->port->IDR & (1 << gpio->pin)) ? GPIO_SET : GPIO_RESET;
}

ERROR_CODE_t Gpio_Set(GPIO_h gpio, GPIO_STATE_t const state)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < GPIO_PIN_MAX));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE_t ret = ERROR;

    // Access both the set and reset bits of the BSRR register at the same time
    // with the reset bit using the invert of the requested state
    // Done this way because it generates the most efficient assembly
    gpio->port->BSRR = ((REG_SIZE)state << gpio->pin) << 16) | (((REG_SIZE)!state) << gpio->pin);
    ret = SUCCESS;

    return ret;
}

ERROR_CODE_t Gpio_Set_High(GPIO_h gpio)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < GPIO_PIN_MAX));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE_t ret = ERROR;

    gpio->port->BSRR = 1 << gpio->pin;
    ret = SUCCESS;

    return ret;
}

ERROR_CODE_t Gpio_Set_Low(GPIO_h gpio)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < GPIO_PIN_MAX));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE_t ret = ERROR;

    gpio->port->BRR = 1 << gpio->pin;
    ret = SUCCESS;

    return ret;
}

ERROR_CODE_t Gpio_Toggle(GPIO_h gpio)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < GPIO_PIN_MAX));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE_t ret = ERROR;

    // Access both the set and reset bits of the BSRR register at the same time
    // with the reset bit using the invert of the ODR register current state
    // Done this way because it generates the most efficient assembly
	gpio->port->BSRR = ((gpio->port->ODR & (1 << gpio->pin)) << 16) | (~(gpio->port->ODR) & (1 << gpio->pin));
    ret = SUCCESS;

    return ret;
}

ERROR_CODE_t inline Gpio_Reg_Write (REG_SIZE_t const address, uint32_t const val) {
    *((REG_SIZE_t * const)address) = val;
}

REG_SIZE_t inline Gpio_Reg_Read (REG_SIZE_t const address) {
    return *((REG_SIZE_t * const)address);
}