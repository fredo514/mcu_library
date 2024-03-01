#include "bsp.h"

static gpio_init_t const gpio_init_arr[] = {
    {led_pin, {.initial_state = GPIO_RESET, .mode = GPIO_MODE_OUTPUT_PP, .pull = GPIO_NOPULL}},
};

error_t bsp_init(void) {
  // Initialize the GPIO pins with their initial configurations
  for (int i = 0; i < (sizeof(gpio_init_arr) / sizeof(gpio_init_arr[0])); i++) {
    Gpio_Init(gpio_init_arr[i].gpio, &gpio_init_arr[i].config);
  }

  return NO_ERROR;
}

inline void Hb_Led_Toggle(void) { 
    Gpio_Toggle(led_pin); 
}