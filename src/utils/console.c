#include "console.h"

typedef enum {
    CONSOLE_HELP,
    CONSOLE_CMD_MAX
} CONSOLE_CMD_t;

typedef struct {
    CONSOLE_CMD_t command;
    void (*cb)(void);
} CONSOLE_ELEM_t;

static void Help_Func(void);

static CONSOLE_ELEM_t command_array[CONSOLE_CMD_MAX] = {
    {CONSOLE_HELP, &Help_Func},
};

ERROR_CODE_t Console_Parse(uint8_t const ch){
    
}

static void Help_Func(void){

}