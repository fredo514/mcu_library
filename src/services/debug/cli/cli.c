#include "cli.h"

typedef enum {
    CLI_HELP,
    CLI_CMD_MAX
} CLI_CMD_t;

typedef struct {
    CLI_CMD_t command;
    void (*cb)(void);
} CLI_ELEM_t;

static void Help_Func(void);

static CLI_ELEM_t command_array[CLI_CMD_MAX] = {
    {CLI_HELP, &Help_Func},
};

ERROR_CODE_t Cli_Parse(uint8_t const ch){
    
}

static void Help_Func(void){

}