// use preprocessor defines to include the correct register map file

#define REG_SIZE_t uint32_t

// typedef struct {
//     REG_SIZE_t volatile IDR;
//     REG_SIZE_t volatile DIR;
//     REG_SIZE_t volatile ODR;
//     REG_SIZE_t volatile BSRR;
// } GPIO_PORT_t;

// GPIO_PORT_t * const PORTA = (GPIO_PORT_t*)0xCAFECAFE;

#define I2C_DUAL_ADDRESS
#define I2C1 ((I2C_TypeDef *) I2C1_BASE)
#define I2C2 ((I2C_TypeDef *) I2C2_BASE)

uint32_t Sysclk_Get_Hz(void);