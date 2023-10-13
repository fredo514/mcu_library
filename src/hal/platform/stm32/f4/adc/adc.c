#include "adc.h"
#include "assert.h"
#include "macros.h"

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
    adc->regs->CR |= ADC_CR_ADSTP;
    // wait until cleared
    while (adc->regs->CR & ADC_CR_ADSTP) {
        // TODO: add 2ms timeout
    }

	// disable adc if enabled
    if (adc->regs->CR & ADC_CR_ADEN) {
        adc->regs->CR |= ADC_CR_ADDIS;
        // wait until cleared
        while (adc->regs->CR & ADC_CR_ADDIS) {
            // TODO: add 2ms timeout
        }
    }

	adc->regs->CFGR1 = 0;

	// enable adc clock
	if (adc->regs == ADC1) {
		SET_MASK(RCC->APB2ENR, RCC_APB2ENR_ADC1EN);
	}
	else {
		// TODO: use assert
		return ERROR_NO_DEV;
	}
   
	// set clock source
    switch (config->clk_source) {
        case ADC_CLK_SRC_HSI:
			// Disable ADC control of the Internal High Speed oscillator
			SET_MASK(RCC->CR2, RCC_CR2_HSI14DIS);

			// Enable the Internal High Speed oscillator
			SET_MASK(RCC->CR2, RCC_CR2_HSI14ON);

			// Wait till HSI is ready
			while (!(RCC->CR2 & RCC_CR2_HSI14RDY)) {
				// TODO: add 2ms timeout
			}

			// Adjust the Internal High Speed oscillator calibration value, hardcode to 16 for now
			MODIFY_MASK(RCC->CR2, RCC_CR2_HSI14TRIM, 16 << RCC_CR2_HSI14TRIM_Pos);
        break;

        case ADC_CLK_SRC_PCLK:
        break;

        default:
            // TODO: use assert
            return ERROR_INVALID_PARAM;
    }

	// set clock prescaler (nothing else in CFGR2)
	adc->regs->CFGR2 = config->clk_div;

    // configure acquisition, first clear all options
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

    //set resolution
	MODIFY_MASK(adc->regs->CFGR1, ADC_CFGR1_RES_Msk, config->resolution);

	// set sampling time
	MODIFY_MASK(adc->regs->SMPR, ADC_SMPR_SMP_Msk, config->sampling_time);

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
    adc->regs->CR |= ADC_CR_ADCAL;

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
    CLEAR_MASK(adc->regs->ISR, (ADC_ISR_EOC | ADC_ISR_EOS | ADC_ISR_OVR));
    
    // start conversion
    adc->regs->CR |= ADC_CR_ADSTART;

    adc->state = ADC_STATE_BUSY;
}

bool Adc_Is_Conversion_Done(ADC_h adc) {
    // check if single conversion mode
    if (adc->regs->CR & ADC_CFGR1_DISCEN) {
        return ((adc->regs->ISR & ADC_ISR_EOC) && (adc->regs->ISR & ADC_ISR_EOS));
    }
    else {
        return (adc->regs->ISR & ADC_ISR_EOS);
    }
}

ADC_SAMPLE_t Adc_Reading_Get(ADC_h adc) {
    return adc->regs->DR;
}

ADC_SAMPLE_t Adc_Channel_Read(ADC_h adc, ADC_CHANNEL_t channel, uint32_t timeout_ms) {
    assert(!(adc->regs->CR & ADC_CR_ADSTART); // make sure no conversion already ongoing

    // select only desired channel
    adc->regs->CHSELR = 1 << channel;
    
    Adc_Conversion_Start(adc);

    while (!Adc_Is_Conversion_Done(adc)) {
        // TODO: add timeout
    }

    return Adc_Reading_Get(adc);
}

void ADC1_IRQHandler(void) {
    Irq_Handler(adc1);
}