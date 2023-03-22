#ifndef CONSOLE_H
#define	CONSOLE_H

#include <stdint.h>
#include "error.h"

ERROR_CODE_t Console_Parse(uint8_t const ch);

#endif // CONSOLE_H