#ifndef ASSERT_H
#define	ASSERT_H

#include "platform.h"
#include "stdint.h"
#include "error.h"

// Redefine the __FILE__ macro so it contains just the file name and no path
// Add -Wno-builtin-macro-redefined to the compiler options to suppress the warning about this.
// #define __FILE__ (__builtin_strrchr("/"__BASE_FILE__, '/') + 1)

// macro to give files a unique ID
// // use in each .c file
// #define FILENUM(num) \
//     enum { F_NUM=num }; \
//     void _dummy##num(void) {}

// Convenience structure to store debug info.
typedef struct {
  REG_SIZE_t const pc;
  REG_SIZE_t const lr;
//   uint8_t file_num;
  char const * const file;
  uint16_t const line;
  ERROR_CODE_t const error_code;
} ASSERT_INFO_t;

typedef enum {
  ASSERT_SEVERITY_MINOR,
  ASSERT_SEVERITY_MODERATE,
  ASSERT_SEVERITY_CRITICAL,
  ASSERT_SEVERITY_MAX
} ASSERT_SEVERITY_t;

void assert_init(void (*callback)(void)); 

#ifndef NO_ASSERTIONS
// void assertion_failure(uint8_t file, uint16_t linenum);
void assertion_failure(char const * const file, uint16_t const linenum, ASSERT_SEVERITY_t severity);
void assertion_failure_log(ASSERT_INFO_t const * const info);
 
// #define ASSERT(expr) ((expr) ? (void)0 : assertion_failure((uint8_t)F_NUM, (uint16_t)__LINE__))
// #define ASSERT(expr) ((expr) ? (void)0 : assertion_failure(__FILE__, (uint16_t)__LINE__))
#define ASSERT(expr) ((expr) ? (void)0 : assertion_failure(__FILE_NAME__, (uint16_t)__LINE__))

#ifndef NO_DEBUG 
#define ASSERT_DEBUG(expr) ASSERT(expr)
#else // NO_DEBUG  
#define ASSERT_DEBUG(expr) ((void)0) /*nothing*/ 
#endif // NO_DEBUG 

#ifndef NO_LOG 
#define ASSERT_LOG(expr, err)                           \
  do {                                                  \
    if (!(expr)) {                                      \
        ASSERT_INFO_t assert_info;                        \ 
        assert_info.pc = GET_PC();                      \   
        assert_info.lr = GET_LR();                      \
        assert_info.file = __FILE__;                    \   // use number ID (unique number, hash of filename, etc) instead?
        assert_info.line = (uint16_t)__LINE__);         \
        assert_info.error_code = (ERROR_CODE_t)(err);   \
        assertion_failure_log(&assert_info);            \
    }                                                   \
  } while (0)
#else // NO_LOG 
#define ASSERT_LOG(expr, err) ASSERT(expr) 
#endif // NO_LOG
#else // NO_ASSERTIONS
#define ASSERT(expr) ((void)0) /*nothing*/ 
#define ASSERT_DEBUG(expr) ((void)0) /*nothing*/ 
#define ASSERT_LOG(expr, err) ((void)0) /*nothing*/ 
#endif // NO_ASSERTIONS

#endif	/* ASSERT_H */