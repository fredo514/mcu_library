#ifndef NVM_H
#define NVM_H

#include "stdint.h"

typedef struct {

} NVM_CONFIG_t;

ERROR_CODE_t Nvm_Init(NVM_CONFIG_t const * const config);
ERROR_CODE_t Nvm_Write(uint32_t address, REG_SIZE_t * data_buf, uint8_t len);
ERROR_CODE_t Nvm_Read(uint32_t address, REG_SIZE_t * data_buf, uint8_t len);
ERROR_CODE_t Nvm_Erase(uint32_t address);

// Use these SPARINGLY to extend the API
ERROR_CODE_t Nvm_Reg_Write (REG_SIZE_t * const address, uint32_t const val);
REG_SIZE_t Nvm_Reg_Read (REG_SIZE_t * const address);

#endif // NVM_H