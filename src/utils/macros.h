#ifndef MACROS_H
#define	MACROS_H

// bitfield manipulation
#define SET_BIT(REG, BIT)     ((REG) |= (1<<(BIT)))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(1<<(BIT)))
#define READ_BIT(REG, BIT)    ((REG) & (1<<(BIT)))

#define SET_MASK(REG, MASK)     ((REG) |= (MASK))
#define CLEAR_MASK(REG, MASK)   ((REG) &= ~(MASK))
#define READ_MASK(REG, MASK)    ((REG) & (MASK))

// unique temporary variable name for macros
#define MACRO_VAR(name) (name##__LINE__)  

// execute start before running what comes in the provided {} after the macro, then calls end
// usage:
// DEFER(start, end) { 
//     foo(); 
// } 
#define DEFER(start, end) (start); for (                  \ 
                          uint_fast8_t MACRO_VAR(_i_) = FALSE; \ 
                          !MACRO_VAR(_i_);                \ 
                          (MACRO_VAR(_i_) = TRUE), (end))  

#endif // MACROS_H