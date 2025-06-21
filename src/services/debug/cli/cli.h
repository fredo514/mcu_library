#ifndef CLI_H
#define	CLI_H

#include <stdint.h>
#include "error.h"

ERROR_CODE_t Cli_Parse(uint8_t const ch);

#endif // CLI_H