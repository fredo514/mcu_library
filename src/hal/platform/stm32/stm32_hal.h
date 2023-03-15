// use preprocessor defines to include the correct register map file

#define REG_SIZE uint32_t

// typedef {
//     REG_SIZE volatile IDR;
//     REG_SIZE volatile DIR;
//     REG_SIZE volatile ODR;
//     REG_SIZE volatile BSRR;
// } GPIO_PORT;

// GPIO_PORT * const PORTA = (GPIO_PORT*)0xCAFECAFE;

uint32_t Sysclk_Get_Hz(void);