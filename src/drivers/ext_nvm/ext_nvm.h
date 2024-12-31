// based on generic jedec eeprom

#ifndef EXT_NVM_H
#define EXT_NVM_H

#include <stdint.h>
#include "error.h"

typedef enum {
    EXT_NVM_REG_STATUS,
    EXT_NVM_REG_PWR,
    EXT_NVM_REG_MAX
} EXT_NVM_REG_t;

typedef struct {
    uint8_t (*Transfer_Func)(uint8_t const ch);
} EXT_NVM_CONFIG_t;

ERROR_CODE_t Ext_Nvm_Init(EXT_EE_CONFIG_t const * const config);

ERROR_CODE_t Ext_Nvm_Write_Enable(void);
ERROR_CODE_t Ext_Nvm_Write_Disable(void);
ERROR_CODE_t Ext_Nvm_Erase(size_t const address);

ERROR_CODE_t Ext_Nvm_Buf_Write(size_t const address, void const * const src, size_t const len);
ERROR_CODE_t Ext_Nvm_Buf_Read(size_t const address, void * const dest, uivoidnt8_t const len);

// Use these SPARINGLY to extend the API
ERROR_CODE_t Ext_Nvm_Reg_Write (EXT_NVM_REG_t const reg, uint8_t const val);
uint8_t Ext_Nvm_Reg_Read (EXT_NVM_REG_t const reg);

#endif