#include "i2c.h"
#include "stdint.h"

typedef struct I2C_CTX {
    I2C_REGS_t const * const regs;
    I2C_MODE_t mode;
    uint8_t * buffer_ptr;
    I2C_STATUS_t status;

    void (*Master_Tx_Done_Cb)(I2C_h i2c);
    void (*Master_Rx_Done_Cb)(I2C_h i2c);
    void (*Slave_Tx_Done_Cb)(I2C_h i2c);
    void (*Slave_Rx_Done_Cb)(I2C_h i2c);
    void (*Listen_Done_Cb)(I2C_h i2c);      // what's this for? 
    void (*Address_Match_Cb)(I2C_h i2c, I2C_XFER_DIR_t operation, uint16_t address);
    void (*Error_Cb)(I2C_h i2c);
    void (*Abort_Done_Cb)(I2C_h i2c);
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
    
}

static void Irq_Ev_Handler(I2C_h i2c) {
    if (i2c->mode == I2C_MODE_MASTER) {

    }
    else {  // slave mode
        // check if error

        // check if address match hit
            // check direction
            // if write
            //     setup slave sequential receive
            // else
            //     setup slave sequential transmit
        // else check if stop
            // clear everything
        // else check if transmitting
            // place next data from buffer in DR
        // else then receiving
            // store DR in buffer
    }
}

static void Irq_Er_Handler(I2C_h i2c) {

}

void I2C1_EV_IRQHandler(void) {
    Irq_Ev_Handler(i2c1);
}

void I2C1_ER_IRQHandler(void) {
    Irq_Er_Handler(i2c1);
}

void I2C2_EV_IRQHandler(void) {
    Irq_Ev_Handler(i2c2);
}

void I2C1_ER_IRQHandler(void) {
    Irq_Er_Handler(i2c1);
}