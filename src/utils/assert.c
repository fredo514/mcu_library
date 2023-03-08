#include "assert.h" 
#include "printf.h"
 
// function pointer to assertion indicator 
static void (*assert_callback)(void) = NULL; 
 
// sets user's choice of failure indicator function 
void assert_init(void (*callback)(void)) { 
    assert_callback = callback; 
} 
 
// failure function called by macro. 
#ifndef NO_ASSERTIONS
void assertion_failure(uint8_t file, uint16_t linenum) {  
    // software breakpoint if debugging  
#ifdef __DEBUG
    // debugger can look at these 
    volatile uint8_t v_file; 
    volatile uint16_t v_linenum; 
 
    // assign variables to volatiles 
    v_file = file; 
    v_linenum = linenum;
    
    __builtin_software_breakpoint();
#else
    // get LR?

    // call function that indicates an assertion failure  
    assert_ind();
    
    printf("Assert fail at file %u, line %u ", file, linenum); // move to assert_ind?
    
    // halt (or reset?) 
    while(1);  // move to assert_int?
#endif
} 
#endif