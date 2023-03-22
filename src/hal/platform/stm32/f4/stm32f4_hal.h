// use preprocessor defines to include the correct register map file

#define REG_SIZE_t uint32_t

// typedef struct {
//     REG_SIZE_t volatile IDR;
//     REG_SIZE_t volatile DIR;
//     REG_SIZE_t volatile ODR;
//     REG_SIZE_t volatile BSRR;
// } GPIO_PORT_t;

// GPIO_PORT_t * const PORTA = (GPIO_PORT_t*)0xCAFECAFE;

uint32_t Sysclk_Get_Hz(void);