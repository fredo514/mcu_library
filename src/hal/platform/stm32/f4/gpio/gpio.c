#include "gpio.h"
#include "stm32_hal.h"
#include "assert.h"

typedef struct
{
    GPIO_PORT_t * const port;
    GPIO_PIN_t const pin;
} GPIO_CTX_t;

ERROR_CODE_t Gpio_Init(GPIO_h gpio, GPIO_CONFIG_t const * const config) {
    ASSERT(config != 0);   // config exists
    ASSERT(gpio != 0);   // gpio exists
    ASSERT((gpio->pin >= 0) && (gpio->pin < GPIO_PIN_MAX));   // only 16 pins per port

    REG_SIZE_t mask = 0;

    // Find the clock enable bit for the gpio port
    // NOTE: Cannot be tested with mock register structure besause case will not evaluate to constant.
    switch ((int)gpio->port) {
        case (int)GPIO_PORTA:
            mask = RCC_AHBENR_GPIOAEN;
        break;

        case (int)GPIO_PORTB:
            mask = RCC_AHBENR_GPIOBEN;
        break; 

        case (int)GPIO_PORTC:
            mask = RCC_AHBENR_GPIOCEN;
        break; 

        case (int)GPIO_PORTD:
            mask = RCC_AHBENR_GPIODEN;
        break; 

        case (int)GPIO_PORTF:
            mask = RCC_AHBENR_GPIOFEN;
        break; 

        default:
            return ERROR_NO_DEV;
    }

    // enable clock for the gpio port
    Gpio_Reg_Write(RCC->AHBENR, CLEAR_MASK(RCC->AHBENR, mask));       // clear the bits first
    Gpio_Reg_Write(RCC->AHBENR, SET_MASK(RCC->AHBENR, mask));

    // find bit pattern for desired mode
    mask = 0;
    switch ((int)config->mode) {
        case (int)GPIO_OUTPUT:
            mask = GPIO_MODER_OUTPUT_MASK;
        break;

        case (int)GPIO_INPUT:
            mask = GPIO_MODER_INPUT_MASK;
        break;

        case (int)GPIO_MODE_ALT_FUNC:
            mask = GPIO_MODER_ALT_MASK;
        break;

        case (int)GPIO_MODE_ANALOG:
            mask = GPIO_MODER_AN_MASK;
        break;

        default:
            return ERROR_INVALID_PARAM;
    }

    // set the gpio mode
    Gpio_Reg_Write(gpio->port->MODER, CLEAR_MASK(gpio->port->MODER, GPIO_MODER_MASK << (config->gpio->pin * 2)));       // clear the bits first
    Gpio_Reg_Write(gpio->port->MODER, SET_MASK(gpio->port->MODER, mask << (config->gpio->pin * 2)));

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

ERROR_CODE_t Gpio_Reg_Write (REG_SIZE_t const address, uint32_t const val) {
    *((REG_SIZE_t * const)address) = val;
}

REG_SIZE_t Gpio_Reg_Read (REG_SIZE_t const address) {
    return *((REG_SIZE_t * const)address);
}