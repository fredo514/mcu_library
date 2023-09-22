#include "adc.h"
#include "assert.h"

struct I2C_CTX {
    ADC_REGS_t const * const regs;
    ADC_STATE_t state;
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

    // stop ongoing conversion
    adc->regs->CR |= ADC_CR_ADSTP
    // wait until cleared
    while (adc->regs->CR & ADC_CR_ADSTP) {
        // TODO: add timeout
    }

	// disable adc if enabled
    if (adc->regs->CR & ADC_CR_ADEN) {
        adc->regs->CR | ADC_CR_ADDIS;
        // wait until cleared
        while (adc->regs->CR & ADC_CR_ADDIS) {
            // TODO: add timeout
        }
    }
    
    // enable adc clock
    switch (config->clk_source) {
        case ADC_CLK_SRC_HSI:
        break;

        case ADC_CLK_SRC_AHB:
            // clock prescaler, only if clocked from perrpheral clock
            adc->regs->CFGR2 &= ~ADC_CFGR2_MODE_MSK;
            adc->regs->CFGR2 |= config->clk_prescaler;
        break;

        default:
            assert(0);
    }
    
    // configure acquisition
    // resolution
    adc->regs->CFGR1 &= ~ADC_CFGR1_RES_MSK;
    adc->regs->CFGR1 |= config->resolution;

    // other modes
    adc->regs->CFGR1 &= ~(  ADC_CFGR1_DISCEN  |
                            ADC_CFGR1_AUTOFF  |
                            ADC_CFGR1_AUTDLY  |
                            ADC_CFGR1_CONT    |
                            ADC_CFGR1_OVRMOD  |
                            ADC_CFGR1_EXTSEL  |
                            ADC_CFGR1_EXTEN   |
                            ADC_CFGR1_ALIGN   |
                            ADC_CFGR1_SCANDIR |
                            ADC_CFGR1_DMACFG   );
    adc->regs->CFGR1 |= config->auto_conv;
    adc->regs->CFGR1 |= config->auto_off;
    adc->regs->CFGR1 |= config->scan_dir;
    adc->regs->CFGR1 |= config->trig_mode;

    switch (config->conv_mode) {
        case ADC_CONV_MODE_ONESHOT_SINGLE_CHANNEL:
            adc->regs->CFGR1 |= ADC_CFGR1_DISCEN;
        break;

        case ADC_CONV_MODE_ONESHOT_ALL_CHANNELS:
        break;

        case ADC_CONV_MODE_CONTINUOUS:
            adc->regs->CFGR1 |= ADC_CFGR1_CONT;
        break;
        
        default:
            assert(0);
    }

    adc->regs->SMPR &= ~(ADC_SMPR_SMP);
    adc->regs->SMPR |= config->sampling_time;

    // configure channels

    // initialize callbacks

    // calibrate adc
    Adc_Calibrate(adc);

    // enable adc
    adc->regs->CR |= ADC_CR_ADEN;

    // do 1 reading and throw away

    // calibrate offset and gain
    // load offset calibration from nvm to calib register

    adc->ADC_STATUS_IDLE;

    return SUCCESS;
}

ERROR_CODE_t Adc_Calibrate(ADC_h adc) {
    assert(!(adc->regs->CR & ADC_CR_ADEN));

    // backup dma settings
    REG_SIZE_t dma_bak = adc->regs->CFGR1 & (ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG);
    adc->regs->CFGR1 &= ~(ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG);

    // start calibration
    adc->regs->CR &= ADC_CR_ADCAL;

    // wait until done
    while (adc->regs->CR & ADC_CR_ADCAL) {
        // TODO: add timeout
    }

    // restore dma settings
    adc->regs->CFGR1 |= dma_bak;

    return SUCCESS;
}

ERROR_CODE_t Adc_Conversion_Start(ADC_h adc) {
    assert(!(adc->regs->CR & ADC_CR_ADSTART); // make sure no conversion already ongoing

    // clear dangling errors
    adc->regs->ISR &= ~(ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR);
    
    // start conversion
    adc->regs->CR |= ADC_CR_ADSTART;

    adc->state = ADC_STATE_BUSY;
}

bool Adc_Is_Conversion_Done(ADC_h adc) {
    // check if single conversion mode
    if (adc->regs->CR & ADC_CFGR1_DISCEN) {
        return ((adc->regs->ISR & ADC_FLAG_EOC) && (adc->regs->ISR & ADC_FLAG_EOS));
    }
    else {
        return (adc->regs->ISR & ADC_FLAG_EOS);
    }
}

ADC_SAMPLE_t Adc_Reading_Get(ADC_h adc) {
    return adc->regs->DR;
}

ADC_SAMPLE_t Adc_Channel_Read(ADC_h adc, ADC_CHANNEL_t channel, uint32_t timeout_ms) {
    assert(!(adc->regs->CR & ADC_CR_ADSTART); // make sure no conversion already ongoing

    // select only desired channel
    adc->regs->CHSELR = channel
    
    Adc_Conversion_Start(adc);

    while (!Adc_Is_Conversion_Done(adc)) {
        // TODO: add timeout
    }

    return Adc_Reading_Get(adc);
}

void ADC1_IRQHandler(void) {
    Irq_Handler(adc1);
}