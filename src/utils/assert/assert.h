#ifndef ASSERT_H
#define ASSERT_H

#include "error.h"
#include "platform.h"
#include "stdint.h"

// Redefine the __FILE__ macro so it contains just the file name and no path
// Add -Wno-builtin-macro-redefined to the compiler options to suppress the warning about this.
// #define __FILE__ (__builtin_strrchr("/"__BASE_FILE__, '/') + 1)

// macro to give files a unique ID
// // use in each .c file
// #define FILENUM(num) \
//     enum { F_NUM=num }; \
//     void _dummy##num(void) {}

// TODO: if init called without callback, initialize to default breakpoint+loop
// TODO: have real-time handler that
//  - turn led on
//  - save coredump to log (e.g. ram ring buffer with background task that flushes it to non-volatile)
//  - throw highest level fault for app to go to safe state
//  - consider having severity levels with different response (e.g. critical, moderate, minor)

// Convenience structure to store debug info.
typedef struct {
   REG_SIZE_t const pc;
   REG_SIZE_t const lr;
   //   uint8_t file_num;
   char const* const file;
   uint16_t const line;
   error_t const error_code;
} assert_info_t;

typedef enum {
   ASSERT_SEVERITY_MINOR,
   ASSERT_SEVERITY_MODERATE,
   ASSERT_SEVERITY_CRITICAL,
   ASSERT_SEVERITY_MAX
} assert_severity_t;

void assert_init(void (*callback)(void));

#ifndef NO_ASSERTIONS
// void assertion_failure(uint8_t file, uint16_t linenum);
void assertion_failure(char const* const file, uint16_t const linenum);
// void assertion_failure(char const* const file, uint16_t const linenum, assert_severity_t severity);
void assertion_failure_log(assert_info_t const* const info);

// #define assert(expr) ((expr) ? (void)0 : assertion_failure((uint8_t)F_NUM, (uint16_t)__LINE__))
// #define assert(expr) ((expr) ? (void)0 : assertion_failure(__FILE__, (uint16_t)__LINE__))
#define assert(expr) ((expr) ? (void)0 : assertion_failure(__FILE_NAME__, (uint16_t)__LINE__))

#ifndef NO_DEBUG
#define assert_debug(expr) assert(expr)
#else                                // NO_DEBUG
#define assert_debug(expr) ((void)0) /*nothing*/
#endif                               // NO_DEBUG

#ifndef NO_LOG
// use number ID (unique number, hash of filename, etc) instead?
#define assert_log(expr, err)                     \
   do {                                           \
      if (!(expr)) {                              \
         assert_info_t assert_info;               \
         assert_info.pc = GET_PC();               \
         assert_info.lr = GET_LR();               \
         assert_info.file = __FILE__;             \
         assert_info.line = (uint16_t)__LINE__);  \
         assert_info.error_code = (error_t)(err); \
         assertion_failure_log(&assert_info);     \
      }                                           \
   } while (0)
#else  // NO_LOG
#define assert_log(expr, err) assert(expr)
#endif                                  // NO_LOG
#else                                   // NO_ASSERTIONS
#define assert(expr) ((void)0)          /*nothing*/
#define asser_debug(expr) ((void)0)     /*nothing*/
#define assert_log(expr, err) ((void)0) /*nothing*/
#endif                                  // NO_ASSERTIONS

#endif /* ASSERT_H */