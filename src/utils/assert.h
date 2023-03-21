#ifndef ASSERT_H
#define	ASSERT_H

#include "platform.h"

// Redefine the __FILE__ macro so it contains just the file name and no path
// Add -Wno-builtin-macro-redefined to the compiler options to suppress the warning about this.
#define __FILE__ (__builtin_strrchr("/"__BASE_FILE__, '/') + 1)

// macro to give files a unique ID
// // use in each .c file
// #define FILENUM(num) \
//     enum { F_NUM=num }; \
//     void _dummy##num(void) {}

// // Convenience structure to store debug info.
// typedef struct {
//   REG_SIZE pc;
//   REG_SIZE lr;
//   uint8_t file_num;
//   uint16_t line;
//   ERROR_CODE error_code;
// } sAssertInfo;
// extern sAssertInfo g_assert_info;

// #define GET_PC() __builtin_program_counter(0)
// #define GET_LR() __builtin_return_address(0)
 
void assert_init(void (*callback)(void)); 

#ifndef NO_ASSERTIONS
void assertion_failure(uint8_t file, uint16_t linenum);
void assertion_failure_log(sAssertInfo info);
 
// #define ASSERT(expr) ((expr) ? (void)0 : assertion_failure((uint8_t)F_NUM, (uint16_t)__LINE__))
#define ASSERT(expr) ((expr) ? (void)0 : assertion_failure(__FILE__, (uint16_t)__LINE__))

// #define ASSERT_LOG(expr, err)                        \
//   do {                                               \
//     if (!(expr)) {                                   \
//       g_assert_info.pc = GET_PC();                   \   
//       g_assert_info.lr = GET_LR();                   \
//       g_assert_info.file_num = (uint8_t)F_NUM        \
//       g_assert_info.line = (uint16_t)__LINE__)       \
//       g_assert_info.error_code = (ERROR_CODE_t)(err) \
//       assertion_failure_log(g_assert_info);          \
//     }
//   } while (0)

#ifndef NO_DEBUG 
#define ASSERT_DEBUG(expr) ((expr) ? (void)0 : assertion_failure((uint8_t)F_NUM, (uint16_t)__LINE__))
#else 
#define ASSERT_DEBUG(expr) ((void)0) /*nothing*/ 
#endif // NO_DEBUG 
#else 
#define ASSERT_ALWAYS(expr) ((void)0) /*nothing*/ 
#define ASSERT_DEBUG(expr) ((void)0) /*nothing*/ 
#endif // ENABLE_ASSERTIONS

#endif	/* ASSERT_H */