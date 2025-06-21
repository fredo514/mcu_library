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
// void assertion_failure(uint8_t file, uint16_t linenum) {  
void assertion_failure(char const * const file, uint16_t const linenum) {  
    __disable_irq();
    
    // reset stack pointer if overflow?
    
#ifdef __DEBUG
    // software breakpoint if debugging  
    // debugger can look at these 
    volatile char * v_file; 
    volatile uint16_t v_linenum; 
 
    // assign variables to volatiles 
    v_file = file; 
    v_linenum = linenum;

    printf("Assert fail at file %s, line %u ", __file, linenum); // move to assert_ind?

    Core_Breakpoint();
#else // __DEBUG
    // call function that indicates an assertion failure  
    assert_ind();
#endif // __DEBUG

    // halt (or reset?) 
    while(1);  // move to assert_int?

} 
#endif // NO_ASSERTIONS