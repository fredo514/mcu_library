#include "error.h"
#include "gpio.h"

#define led_pin &pin_C13

error_t bsp_init(void);

void Hb_Led_Toggle(void);