#include "console.h"

typedef enum {
    CONSOLE_HELP,
    CONSOLE_CMD_MAX
} CONSOLE_CMD;

typedef struct {
    CONSOLE_CMD command;
    void (*cb)(void);
} CONSOLE_ELEM;

static void Help_Func(void);

static CONSOLE_ELEM command_array[CONSOLE_CMD_MAX] = {
    {CONSOLE_HELP, &Help_Func},
};

ERROR_CODE Console_Parse(uint8_t const ch){
    
}

static void Help_Func(void){

}