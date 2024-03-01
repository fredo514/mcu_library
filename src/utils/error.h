#ifndef ERROR_H 
#define ERROR_H 

typedef void (*error_callback_t)(const ERROR_CTX_t* error_ctx); // application function called when declaring an error 
typedef void (*error_reboot_callback_t)(const ERROR_CTX_t* error_ctx); // application function called after boot with pointer to RAM array 
 
/* @brief Enumerated type for error codes. */ 
typedef enum  
{   
    NO_ERROR = 0,                // Operation performed successfully. 
    SUCCESS = 0,                // Operation performed successfully. 
    ERROR_NO_PERM,              // Operation not permitted 
    ERROR_NO_DEV,               // No such device or address 
    ERROR_INTERNAL,             // Internal error. 
    ERROR_BUSY,                 // Busy. 
    ERROR_NO_MEM,               // No memory for operation. 
    ERROR_TIMEOUT,              // Operation timed out. 
    ERROR_NACK,                 // Operation rejected 
    ERROR_INVALID_PARAM,        // Invalid parameter. 
    ERROR_INVALID_LENGTH,       // Invalid length. 
    ERROR_INVALID_ADDR,         // Bad memory address. 
    ERROR_NULL,                 // Null pointer. 
    ERROR_FORBIDDEN,            // Operation not allowed in this state. 
    ERROR_UNINITIALIZED,        // Instance not configured     
    ERROR_ALREADY_INITIALIZED,  // Instance already configured  
    ERROR_OVERRUN,               // Buffer overflow.
    ERROR_MAX                      
} ERROR_CODE_t; 

/**@brief Macro for calling error handler if supplied error code any other than SUCCESS. 
* 
* @param[in]   err_code    Error code to check. 
* @param[in]   handler     Action to execute if supplied error code is not SUCCESS. 
*/ 
#define HANDLE_ERROR(err_code, handler)           \ 
    do {                                          \ 
        if ((ERROR_CODE_t)err_code != SUCCESS)    \ 
        {                                         \ 
            (handler)();                            \ 
        }                                         \ 
    } while (0) 

/**@brief Macro for verifying statement to be true. It will cause the exterior function to return 
*        err_code if the statement is not true. 
* 
* @param[in]   statement   Statement to test. 
* @param[in]   err_code    Error value to return if test was invalid. 
* 
* @retval      nothing, but will cause the exterior function to return @p err_code if @p statement 
*              is false. 
*/ 
#define VERIFY_TRUE(statement, err_code)        \ 
    do                                          \ 
    {                                           \ 
        if (!(statement))                       \ 
        {                                       \ 
            return err_code;                    \ 
        }                                       \ 
    } while (0) 

/**@brief Macro for verifying statement to be true. It will cause the void-type exterior function to  
* return if the statement is not true. 
* 
* @param[in]   statement   Statement to test. 
*  
* @retval      nothing, but will still cause the exterior function to return if @p statement is false. 
*/ 
#define VERIFY_TRUE_VOID(statement) VERIFY_TRUE((statement), ) 

/**@brief Macro for verifying statement to be false. It will cause the exterior function to return 
*        err_code if the statement is true. 
* 
* @param[in]   statement   Statement to test. 
* @param[in]   err_code    Error value to return if test was invalid. 
* 
* @retval      nothing, but will cause the exterior function to return @p err_code if @p statement 
*              is true. 
*/ 
#define VERIFY_FALSE(statement, err_code)       \ 
    do                                          \ 
    {                                           \ 
        if (statement)                          \ 
        {                                       \ 
            return err_code;                    \ 
        }                                       \ 
    } while (0) 

/**@brief Macro for verifying statement to be false. It will cause the void-type exterior function to  
* return if the statement is true. 
* 
* @param[in]   statement   Statement to test. 
*  
* @retval      nothing, but will cause the exterior function to return if @p statement is true. 
*/ 
#define VERIFY_FALSE_VOID(statement) VERIFY_FALSE((statement), ) 

/**@brief Macro for verifying that a function returned SUCCESS. It will cause the exterior 
*        function to return error code of statement if it is not @ref SUCCESS. 
* 
* @param[in] statement     Statement to check against NO_ERROR. 
*/ 
#define VERIFY_SUCCESS(err_code) VERIFY_TRUE(((ERROR_CODE_t)(err_code) == SUCCESS), (err_code))

/**@brief Macro for verifying that a function returned NO_ERROR. It will cause the void-type  
* exterior function to return if the err_code is not @ref NO_ERROR. 
* 
* @param[in] err_code The error code to check. 
*/ 
#define VERIFY_SUCCESS_VOID(err_code) VERIFY_TRUE_VOID((err_code) == SUCCESS) 

#endif /* ERROR_H */ 