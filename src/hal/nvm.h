#ifndef NVM_H
#define NVM_H

#include "error.h"
#include "stdint.h"
#include "stdbool.h"

typedef enum {
  FLASH_SECTOR_0,
  FLASH_SECTOR_1,
  FLASH_SECTOR_2,
  FLASH_SECTOR_3,
  FLASH_SECTOR_4,
  FLASH_SECTOR_5,
  FLASH_SECTOR_6,
  FLASH_SECTOR_7,
  FLASH_SECTOR_8,
  FLASH_SECTOR_9,
  FLASH_SECTOR_10,
  FLASH_SECTOR_11,
  FLASH_SECTOR_MAX
} FLASH_SECTOR_t;

typedef struct {

} NVM_CONFIG_t;

ERROR_CODE_t Nvm_Init(NVM_CONFIG_t const * const config);
bool Flash_Is_Busy(void);

ERROR_CODE_t Nvm_Unlock(void);
ERROR_CODE_t Nvm_Lock(void);

ERROR_CODE_t Nvm_Word_Write(REG_SIZE_t * const address, REG_SIZE_t const data);
ERROR_CODE_t Nvm_Buffer_Write(REG_SIZE_t* const address, REG_SIZE_t const * const data_buf, size_t const len);
REG_SIZE_t Nvm_Word_Read(REG_SIZE_t const * const address);
ERROR_CODE_t Nvm_Buffer_Read(REG_SIZE_t const * const address, REG_SIZE_t * const data_buf, size_t const len);

ERROR_CODE_t Nvm_Sector_Erase(FLASH_SECTOR_t const start_sector, size_t const nb_sector);
ERROR_CODE_t Nvm_Erase_All(void);

// Use these SPARINGLY to extend the API
ERROR_CODE_t Nvm_Reg_Write(REG_SIZE_t * const address, uint32_t const val);
REG_SIZE_t Nvm_Reg_Read(REG_SIZE_t const * const address);

#endif // NVM_H