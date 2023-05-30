#include "i2c.h"
#include "stdint.h"

typedef struct I2C_CTX {
    I2C_REGS_t const * const regs;
    I2C_MODE_t mode;
    uint8_t * buffer_ptr;
    I2C_STATUS_t status;
    uint8_t receive_count;

    void (*Callbacks[I2C_CB_ID_MAX - 1])(I2C_h i2c);
    void (*Address_Match_Cb)(I2C_h i2c, I2C_XFER_DIR_t operation, uint16_t address);
};

static void Irq_Ev_Handler(I2C_h i2c);
static void Irq_Er_Handler(I2C_h i2c);

I2C_h I2c_Create(I2C_REGS_t const * const regs) {
    I2C_h inst = calloc(1, sizeof(struct I2C_CTX));
    inst->regs = regs;
    inst->status = I2C_STATUS_RESET;

    return inst;
}

ERROR_CODE_t I2c_Init(I2C_h i2c, I2C_CONFIG_t const * const config) {
    // Disable the selected I2C peripheral
    CLEAR_MASK(i2c->regs->CR1, I2C_CR1_PE);

    // enable clock for i2c peripheral
    switch (int)i2c->regs) {
        case (int)I2C1:
            Core_Reg_Write(RCC->AHB1ENR, SET_MASK(RCC->AHB1ENR, RCC_APB1ENR_I2C1EN));
        break;

        case (int)I2C2:
            Core_Reg_Write(RCC->AHB1ENR, SET_MASK(RCC->AHB1ENR, RCC_APB1ENR_I2C2EN));
        break;
        
        default:
            return ERROR_NO_DEV;
    }

    // set speed
    i2c->regs->TIMINGR = config->speed;

    // set addresses
    uint16_t address_mask;
    if (I2C_ADDRESS_MODE_7BIT == address_mode) {
        address_mask = 0x7F;
    }
    else {
        address_mask = 0x3FF;
    }

    i2c->regs->OAR1 = (I2C_OAR1_OA1EN | (config->address & address_mask));

#ifdef I2C_DUAL_ADDRESS
    i2c->regs->OAR2 = (I2C_OAR2_OA2EN | (config->address2 & address_mask) | (I2C_OA2_NOMASK << 8));
#endif

    // configure CR1 and CR2
    // No Generalcall and NoStretch mode
    i2c->regs->CR1 = I2C_GENERALCALL_DISABLE | I2C_CR1_NOSTRETCH;

    // Enable the AUTOEND by default
    i2c->regs->CR2 |= I2C_CR2_AUTOEND;

    // Only enable Address Acknowledge in master mode
    if (I2C_MODE_MASTER == config->mode) {
        i2c->regs->CR2 |= I2C_CR2_NACK;
    }
    else {
        i2c->regs->CR2 &= ~I2C_CR2_NACK;
    }
	
    // clean callbacks
    for (int i =0; i<I2C_CB_ID_MAX - 1; ++i) {
        i2c->Callbacks[i] = &Dummy_Callback;
    }
    i2c->Address_Match_Cb = &Dummy_Addr_Callback;

    // enable i2c peripheral
    SET_MASK(i2c->regs->CR1, I2C_CR1_PE);
}

ERROR_CODE_t I2c_Slave_Enable(I2C_h i2c) {
    // enable interrupts
    i2c->regs->CR1 |= I2C_CR1_ERRIE | I2C_CR1_TCIE | I2C_CR1_ADDRIE;

    return NO_ERROR;
}

ERROR_CODE_t I2c_Slave_Disable(I2C_h i2c) {
    // disable interrupts
    i2c->regs->CR1 &= ~(I2C_CR1_ERRIE | I2C_CR1_TCIE | I2C_CR1_ADDRIE);

    return NO_ERROR;
}

uint8_t I2c_Receive_Count_Get(I2C_h i2c) {
    return i2c->receive_count;
}

I2C_STATUS_t I2c_Buffer_Attach(I2C_h i2c, uint8_t const * const buffer_ptr, uint16_t len) {
    i2c->buffer = buffer_ptr;
    i2c->buf_len = len;

    return NO_ERROR;
}

ERROR_CODE_t I2c_Callback_Register(I2C_h i2c, I2C_CALLBACK_ID_t const callback_id, void (*cb)(I2C_h i2c)) {
    if (callback_id < I2C_ADDRESS_MATCH_CALLBACK) {
        i2c->Callbacks[callback_id] = cb;
        if (NULL == cb) {
            i2c->Callbacks[callback_id] = Dummy_Callback;
        }
        else {
            i2c->Callbacks[callback_id] = cb;
        }
    }
    else if (I2C_ADDRESS_MATCH_CALLBACK == callback_id) {
        if (NULL == cb) {
            i2c->Address_Match_Cb = &Dummy_Addr_Callback;
        }
        else {
            i2c->Address_Match_Cb = cb;
        }
    }
    else {
        return ERROR_INVALID_PARAM;
    }

    return SUCCESS;
}

static void Irq_Ev_Handler(I2C_h i2c) {
    if (i2c->mode == I2C_MODE_MASTER) {

    }
    // then slave mode
    else {  
        // check if error

        // check if stop
	    if (i2c->regs->ISR & I2C_ISR_STOPF) {
            // clear everything
            // Clear STOP Flag
		    i2c->regs->ICR |= I2C_ISR_STOPF;

            // Clear ack flag
            i2c->regs->ICR |= I2C_ISR_ADDR;

            // Disable rx & tx Interrupts
             i2c->regs->CR1 &= ~(I2C_CR1_TXIE | I2C_CR1_RXIE);

            if(i2c->status == I2C_STATUS_BUSY_TX) {
                i2c->Callbacks[I2C_MASTER_TX_DONE_CALLBACK](i2c);
            }
            else if(me->status == I2C_STATUS_BUSY_RX) {
                i2c->Callbacks[I2C_MASTER_RX_DONE_CALLBACK](i2c);
            }
            else {
                // Do nothing
            }

            // Enable Address Acknowledge
            i2c->regs->CR2 |= I2C_CR2_NACK;

            // Reset state
            me->status = I2C_STATUS_READY;
        }

        // else check if address match hit
        else if (i2c->regs->ISR & I2C_ISR_ADDR) {
            // Clear ADDR Interrupt
 		    i2c->regs->ICR |= I2C_ICR_ADDRCF;
            
            i2c->receive_count = 0;
            
            // get direction
            I2C_XFER_DIR_t dir;
            if (i2c->regs->ISR & I2C_ISR_DIR) {
                dir = I2C_XFER_DIR_WRITE;
            } 
            else {
                dir = I2C_XFER_DIR_READ;
            } 

            // get address
            uint16_t addr = (uint16_t)((i2c->regs->ISR & I2C_ISR_ADDR) >> I2C_ISR_ADDR_Pos);

            // call address match hit callback
            i2c->Address_Match_Cb(dir, addr);
            
            // check direction
            if (dir == I2C_XFER_DIR_WRITE) {
                // write
                // setup slave sequential receive
                i2c->status = I2C_STATUS_BUSY_RX;
			    i2c->regs->CR1 |= I2C_ISR_RXNE | I2C_CR1_NACKIE | I2C_CR1_ERRIE | I2C_CR1_TCIE;
            }
            else {
                // read
                // setup first byte for slave transmit
			    i2c->regs->TXDR = (uint32_t)(i2c->buffer_ptr[0]);
                i2c->status = I2C_STATUS_BUSY_TX;
                i2c->regs->CR1 |= I2C_CR1_TXIE | I2C_CR1_TCIE;	
            }
        }

        // else check if transmitting
        else if (i2c->regs->ISR & I2C_ISR_TXIS) {
            if (index < i2c->buf_len) {
                // place next data from buffer in DR
		        i2c->regs->TXDR = i2c->buffer_ptr[index];
                i2c->index++;
            }
            else {
                // send 0xFF if buffer empty
                i2c->regs->TXDR = 0xFF;
            }
        }

        // else then receiving
        else {
            // store DR in buffer
            if (index < i2c->buf_len) {
                i2c->buffer_ptr[index] = i2c->regs->RXDR;
                i2c->index++;
            }
            // drop data if buffer full
        }
    }
}

static void Irq_Er_Handler(I2C_h i2c) {

}

static void Dummy_Callback(I2C_h i2c) {
    // do nothing?
}

static void Dummy_Addr_Callback(I2C_h i2c, I2C_XFER_DIR_t operation, uint16_t address) {
    // do nothing or at least attach a buffer?
}

ERROR_CODE_t inline I2c_Reg_Write (REG_SIZE_t const address, uint32_t const val) {
    *((REG_SIZE_t * const)address) = val;
}

REG_SIZE_t inline I2c_Reg_Read (REG_SIZE_t const address) {
    return *((REG_SIZE_t * const)address);
}

void I2C1_EV_IRQHandler(void) {
    if (i2c1.regs->ISR & (I2C_ISR_BERR | I2C_ISR_ARLO | I2C_ISR_OVR)) {
		// handle error
	}
	else {
		Irq_Ev_Handler(i2c1);
	}
}

void I2C1_ER_IRQHandler(void) {
	Irq_Er_Handler(i2c1);
}

void I2C2_EV_IRQHandler(void) {
    if (i2c2.regs->ISR & (I2C_ISR_BERR | I2C_ISR_ARLO | I2C_ISR_OVR)) {
		// handle error
	}
	else {
		Irq_Ev_Handler(i2c2);
	}
}

void I2C2_ER_IRQHandler(void) {
    Irq_Er_Handler(i2c2);
}