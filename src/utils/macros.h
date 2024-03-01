#ifndef MACROS_H
#define	MACROS_H

// bitfield manipulation
#define SET_BIT(REG, BIT)     ((REG) |= (1<<(BIT)))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(1<<(BIT)))
#define READ_BIT(REG, BIT)    ((REG) & (1<<(BIT)))
#define MODIFY_BIT(REG, BIT, VAL)  ((REG) = (((REG) & (~(1<<(BIT)))) | ((VAL)<<BIT)))

#define SET_MASK(REG, MASK)     ((REG) |= (MASK))
#define CLEAR_MASK(REG, MASK)   ((REG) &= ~(MASK))
#define READ_MASK(REG, MASK)    ((REG) & (MASK))
#define MODIFY_MASK(REG, CLEARMASK, SETMASK)  ((REG) = (((REG) & (~(CLEARMASK))) | (SETMASK)))

// need to do in 2 stages to allow proper macro expansion
#define CONCAT_(prefix, suffix) prefix##suffix
// Concatenate `prefix, suffix` into `prefixsuffix`
#define CONCAT(prefix, suffix) CONCAT_(prefix, suffix)
// Make a unique variable name containing the line number at the end of the name. 
// Ex: `uint64_t UNIQUE_VAR_NAME(counter) = 0;` would produce `uint64_t counter_7 = 0` if the call is on line 7.
#define UNIQUE_VAR_NAME(prefix) CONCAT(prefix, __LINE__)

// execute start before running what comes in the provided {} after the macro, then calls end
// usage:
// DEFER(start, end) { 
//     foo(); 
// } 
#define DEFER(start, end)                                        \
  start;                                                         \
  for (bool UNIQUE_VAR_NAME(_i_) = false; !UNIQUE_VAR_NAME(_i_); \
       (UNIQUE_VAR_NAME(_i_) = true), (end))

// Redefine the __FILE__ macro so it contains just the file name and no path
// Add -Wno-builtin-macro-redefined to the compiler options to suppress the
// warning about this.
#define __FILE__ (__builtin_strrchr("/"__BASE_FILE__, '/') + 1)

// Number of elements in the given array
#define ARRAY_SIZE(array) (sizeof(array)/sizeof((array)[0]))

#endif // MACROS_H