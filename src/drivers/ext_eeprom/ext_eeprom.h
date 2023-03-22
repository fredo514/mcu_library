#ifndef EXT_EEPROM_H
#define EXT_EEPROM_H

#include "stdint.h"
#include <stdbool.h>
#include "error.h"

typedef enum {
    EXT_EE_REG_STATUS,
    EXT_EE_REG_PWR,
    EXT_EE_REG_MAX
} EXT_EE_REG_t;

typedef struct {
    uint8_t (*Transfer_Func)(uint8_t const ch);
} EXT_EE_CONFIG_t;

ERROR_CODE_t Ext_Ee_Init(EXT_EE_CONFIG_t const * const config);

ERROR_CODE_t Ext_Ee_Write_Enable(void);
ERROR_CODE_t Ext_Ee_Write_Disable(void);
ERROR_CODE_t Ext_Ee_Erase(uint32_t const address);

ERROR_CODE_t Ext_Ee_Buf_Write(uint32_t const address, uint8_t const * const src, uint8_t const len);
ERROR_CODE_t Ext_Ee_Buf_Read(uint32_t const address, uint8_t * const dest, uint8_t const len);

// Use these SPARINGLY to extend the API
ERROR_CODE_t Ext_ee_Reg_Write (EXT_EE_REG_t const reg, uint8_t const val);
uint8_t Ext_ee_Reg_Read (EXT_EE_REG const reg);

#endif // EXT_EEPROM_H