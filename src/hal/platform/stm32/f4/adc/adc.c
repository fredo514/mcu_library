#include "adc.h"

struct I2C_CTX {
    ADC_REGS_t const * const regs;

};

#define ENABLE_TIMEOUT_MS       2
#define DISABLE_TIMEOUT_MS      2
#define STOP_CONV_TIMEOUT_MS    2

ADC_h Adc_Create(ADC_REGS_t const * const regs) {
    ADC_h inst = calloc(1, sizeof(struct I2C_CTX));
    inst->regs = regs;
    inst->status = ADC_STATUS_RESET;

    return inst;
}

ERROR_CODE_t Adc_Init(ADC_h adc, ADC_CONFIG_t const * const config) {
    assert(adc); // valid adc instance
    assert(adc->status == ADC_STATUS_RESET); // adc is not already initialized
    
    // enable HSI14 clock
    // enable adc clock 
    // load offset calibration from nvm to calib register
    // confugure acquisition
    // configure pins
    // enable adc
    // do 1 reading and throw away
    // calibrate offset and gain
}

void ADC_IRQHandler(void) {
    Irq_Handler(adc1);
}