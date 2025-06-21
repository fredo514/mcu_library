#include "i2c.h"
#include "stdint.h"

typedef union {
    void (*Address_Match_Cb)(I2C_h i2c, I2C_XFER_DIR_t operation, uint16_t address);
    bool (*Slave_Rx_Ack_Cb)(I2C_h i2c, uint8_t byte);
    void (*Slave_Done_Cb)(I2C_h i2c, I2C_XFER_DIR_t operation);
    void (*Generic_Cb)(I2C_h i2c);
} CALLBACK_t;

struct I2C_CTX {
    I2C_REGS_t const * const regs;
    I2C_MODE_t mode;
    uint8_t * buffer_ptr;
    I2C_STATE_t state;
    uint8_t index;
    CALLBACK_t callbacks[I2C_CB_ID_MAX];
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

    // reset the peripheral
    SET_MASK(i2c->regs->CR1, I2C_CR1_SWRST);
    CLEAR_MASK(i2c->regs->CR1, I2C_CR1_SWRST);

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

    switch (config->mode) {
        case I2C_MODE_MASTER:
            return ERROR_INTERNAL;
        break;

        case I2C_MODE_SLAVE:
            // Setup slave address 
            uint16_t address_mask;
            if (I2C_ADDRESS_MODE_7BIT == config->address_mode) {
                address_mask = 0x7F;
            }
            else {
                address_mask = 0x3FF;
            }

            i2c->regs->OAR1 = (I2C_OAR1_OA1EN | ((config->address & address_mask) << 1));

#ifdef I2C_DUAL_ADDRESS
            // Setup slave address 2 if enabled 
            if (config->slave_address2 != 0)
            {
                i2c->regs->OAR2 = (I2C_OAR2_OA2EN | ((config->slave_address2 & address_mask) << 1) | (I2C_OA2_NOMASK << 8));
            }
#endif

            // General call disabled
            // Stretch mode enabled
            // Reload mode enabled
            // Slave byte control mode disabled for now
            // No noise filter
            // all interrupts disabled
            i2c->regs->CR1 = 0;

            // AUTOEND enabled
            // Reload mode enabled
            // Respond NACK by default
            i2c->regs->CR2 = I2C_CR2_AUTOEND | I2C_CR2_RELOAD | I2C_CR2_NACK;

            // Configure baudrate
            i2c->regs->TIMINGR = config->speed;

            // cleanup dangling address match interrupts
            i2c->regs->ISR &= ~I2C_ISR_ADDR;
        break;

        default:
            return ERROR_INVALID_PARAM;
    }
	
    // clean callbacks
    // Attach dummy function to callbacks to avoid error if interrupt is called before app attaches its own
	i2c->callbacks[I2C_ADDRESS_MATCH_CALLBACK].Address_Match_Cb = &Dummy_Addr_Callback;	
	i2c->callbacks[I2C_SLAVE_RX_ACK_CALLBACK].Slave_Rx_Ack_Cb = &Dummy_Rx_Ack;
	// Attach dummy buffer for if app forgets to attach its own
	i2c_buffer_attach(me, &dummy_buffer, 1);
	me->index = 0;

    // Need to be enabled separately
	me->state = I2C_STATE_INACTIVE;

    // enable i2c peripheral
    SET_MASK(i2c->regs->CR1, I2C_CR1_PE);
}

ERROR_CODE_t I2c_Slave_Enable(I2C_h i2c) {
    // ready to accept transactions
	me->state = I2C_STATE_READY;

    // enable all interrupts for listen mode
    i2c->regs->CR1 |= I2C_CR1_ERRIE | I2C_CR1_STOPIE | I2C_CR1_ADDRIE | I2C_CR1_NACKIE;

    return NO_ERROR;
}

ERROR_CODE_t I2c_Slave_Disable(I2C_h i2c) {
    // disable all interrupts
    i2c->regs->CR1 &= ~(I2C_CR1_ERRIE | I2C_CR1_STOPIE | I2C_CR1_ADDRIE | I2C_CR1_NACKIE | I2C_CR1_TXIE | I2C_CR1_RXIE | I2C_CR1_TCIE);

	me->state = I2C_STATE_INACTIVE;

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

void I2c_Callback_Register(I2C_h i2c, I2C_CALLBACK_ID_t const callback_id, void * cb) {
    Assert(callback_id < I2C_CB_ID_MAX);
    
    i2c->callbacks[callback_id].Generic_Cb = cb
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
            // Clear dangling interrupts
		    i2c->regs->ICR |= I2C_ICR_STOPCF | I2C_ICR_NACKCF;

            // Disable interrupts
            i2c->regs->CR1 &= ~(I2C_ISR_RXNE | I2C_CR1_TCIE | I2C_CR1_TXIE);

            if(i2c->state == I2C_STATUS_BUSY_TX) {
                i2c->Callbacks[I2C_SLAVE_TX_DONE_CALLBACK].Generic_Cb(i2c);
            }
            else if(me->status == I2C_STATUS_BUSY_RX) {
                i2c->Callbacks[I2C_SLAVE_RX_DONE_CALLBACK].Generic_Cb(i2c);
            }
            else if () {
                // other ones
            }

            i2c->regs->TXDR = 0x00;
            CLEAR_MASK(i2c->regs->ISR, I2C_ISR_TXE);

            // Reset state
            i2c->status = I2C_STATUS_READY;
        }

        // else check if address match hit
        else if (i2c->regs->ISR & I2C_ISR_ADDR) {
            // Clear ADDR Interrupt
 		    i2c->regs->ICR |= I2C_ICR_ADDRCF;
            
            // reset buffer index
            i2c->index = 0;
            
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
            i2c->Callbacks[I2C_ADDRESS_MATCH_CALLBACK].Address_Match_Cb(i2c, dir, addr);
            
            // check direction
            if (dir == I2C_XFER_DIR_WRITE) {
                // write
                // setup slave sequential receive
                i2c->state = I2C_STATUS_BUSY_RX;
			    i2c->regs->CR1 |= I2C_ISR_RXIE | I2C_CR1_TCIE;

                // flush the receive register
                volatile uint32_t tmp = i2c->regs->RXDR;

                // setup reload for individual bytes ACK control
                i2c->regs->CR1 |= I2C_CR1_SBC;
                i2c->regs->CR2 &= ~I2C_CR2_NBYTES_Msk;
                i2c->regs->CR2 |= (1 << I2C_CR2_NBYTES_Pos);

                // enable slave receive interrupts	
                i2c->regs->CR1 |= I2C_CR1_RXIE | I2C_CR1_TCIE;
            }
            else {
                // read
                // setup first byte for slave transmit
                i2c->status = I2C_STATUS_BUSY_TX;

                // clean any dangling interrupts
                i2c->regs->ISR |= I2C_ISR_TXE | I2C_ISR_TXIS;
                i2c->regs->ICR |= I2C_ICR_STOPCF;
            
                // setup first byte for slave transmit
                i2c->regs->TXDR = i2c->buffer_ptr[0];
                i2c->index = 1;
                    
                // disable individual bytes ACK control when transmitting
                i2c->regs->CR1 &= ~I2C_CR1_SBC;
                
                // Enable slave transmit interrupts
                i2c->regs->CR1 |= I2C_CR1_TXIE;	
            }
        }

        // else check if NACK
        else if (i2c->regs->ISR & I2C_ISR_NACK) {
            // Clear NACK
		    i2c->regs->ICR |= I2C_ICR_NACKCF;
        }

        // else check if master reads from us
        else if (i2c->regs->ISR & I2C_ISR_TXIS) {
            // check if buffer is empty
            if (i2c->index < i2c->buf_len) {
                // place next data from buffer in DR
		        i2c->regs->TXDR = i2c->buffer_ptr[index];
                i2c->index++;
            }
            else {
                // send 0xFF if buffer empty
                i2c->regs->TXDR = 0xFF;
            }
        }

        // else check if master is sending data
        else if ((i2c->regs->ISR & I2C_ISR_RXNE) || (i2c->regs->ISR & I2C_ISR_TCR)) {
            // clear interrupt
		    i2c->regs->ISR &= ~I2C_ISR_TCR;
            
            // check if buffer is full
            if (i2c->index < i2c->buf_len) {
                // buffer is not full
                // store receive data register in buffer
                i2c->buffer_ptr[index] = i2c->regs->RXDR;
                i2c->index++;

                // check with app to decide what ACK to answer
                if (i2c->Callbacks[I2C_SLAVE_RX_ACK_CALLBACK].Slave_Rx_Ack_Cb((uint8_t)i2c->regs->RXDR)) {
                    // true, answer ACK
                    i2c->regs->CR2 &= ~I2C_CR2_NACK;
                }
                else {
                    // false, answer NACK
                    i2c->regs->CR2 |= I2C_CR2_NACK;
                }
            }
            else {
                // buffer is full
                // drop this byte to avoid receive register overflow error
                volatile uint32_t tmp = i2c->regs->RXDR;
                
                // answer NACK
                i2c->regs->CR2 |= I2C_CR2_NACK;
            }
            
            // release clock by setting up reload for next byte ACK control
		    i2c->regs->CR2 |= (1 << I2C_CR2_NBYTES_Pos);
        }

        else {
            // nothing to do
        }
    }
}

static void Irq_Er_Handler(I2C_h i2c) {
    bool error = false;
    
    if (i2c->regs->CR1 & I2C_CR1_ERRIE) {
        if (i2c->regs->ISR & I2C_ISR_BERR) {
            // report BERR error
            error = true;

            // clear error
            i2c->regs->ICR |= I2C_ICR_BERR;
        }

        if (i2c->regs->ISR & I2C_ISR_OVR) {
            // report Over-Run/Under-Run error
            error = true;

            // clear error
            i2c->regs->ICR |= I2C_ICR_OVR;
        }

        if (i2c->regs->ISR & I2C_ISR_ARLO) {
            // report Arbitration Loss error
            error = true;

            // clear error
            i2c->regs->ICR |= I2C_ICR_ARLO;
        }
    }
    
    if (error) {
        // handle error
    }
}

static void Dummy_Callback(I2C_h i2c) {
    // do nothing?
}

static void Dummy_Addr_Callback(I2C_h i2c, I2C_XFER_DIR_t operation, uint16_t address) {
    // do nothing or at least attach a buffer?
}

static bool Dummy_Rx_Ack_Callback(I2C_h i2c, uint8_t byte) {
    // do nothing or at least attach a buffer?
    return false;
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