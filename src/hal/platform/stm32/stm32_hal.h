// use preprocessor defines to include the correct register map file

#define REG_SIZE uint32_t

// declare const pointers to structures of volatile uint32_t
// typedef {
//     uint32_t volatile BSRR;
// } GPIO_REGS;

// GPIO_REGS * const PORTA = (GPIO_REGS*)0xCAFECAFE;

uint32_t Sysclk_Get_Hz(void);