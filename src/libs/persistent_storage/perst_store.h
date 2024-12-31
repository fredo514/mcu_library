#ifndef PERST_STORE_H
#define PERST_STORE_H

#include "error.h"
#include "perst_store_config.h"

errot_t perst_store_load(void);
error_t perst_store_section_update(perst_store_section_t section, ...);
void perst_store_section_default(perst_store_section_t section)

#endif