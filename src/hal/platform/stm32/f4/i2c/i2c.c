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
    // configure i2c interrupts
    // enable clock for pins and i2c peripheral
    // set speed
    // set addresses
    // configure CR1 and CR2

    for (int i =0; i<I2C_CB_ID_MAX - 1; ++i) {
        i2c->Callbacks[i] = &Dummy_Callback;
    }
    i2c->Address_Match_Cb = &Dummy_Addr_Callback;

    // enable i2c peripheral
}

ERROR_CODE_t I2c_Slave_Enable(I2C_h i2c) {
    // enable interrupts
}

ERROR_CODE_t I2c_Slave_Disable(I2C_h i2c) {
    // disable interrupts
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
        }
        // else check if address match hit
        else if (i2c->regs->ISR & I2C_ISR_ADDR) {
            // check direction
            I2C_XFER_DIR_t dir = i2c->regs->ISR & I2C_ISR_DIR ? I2C_XFER_DIR_WRITE : I2C_XFER_DIR_READ;

            // call address match hit callback
            i2c->Address_Match_Cb(dir, addr);
            
            // check direction
            if (dir == I2C_XFER_DIR_WRITE) {
                // write
                // setup slave sequential receive
            }
            else {
                // read
                // setup first byte for slave transmit
			    i2c->regs->TXDR = i2c->buffer_ptr[0];
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