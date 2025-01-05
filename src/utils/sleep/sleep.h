#ifndef SLEEP_H 
#define SLEEP_H 

#include "stdint.h"

void Sleep_Cycle(uint32_t duration);
void Sleep_us(uint32_t duration);
void Sleep_ms(uint32_t duration);

/**
 * @brief Macro for busy waiting until condition is met.
 *
 * @param[in]  condition Condition to meet.
 * @param[in]  attempts  Maximum number of condition checks. Must not be 0.
 * @param[in]  sleep_func  Sleep function to call with provided delay.
 * @param[in]  delay  Delay between consecutive checks.
 * @param[out] result    Boolean variable to store the result of the wait process.
 *                       Set to true if the condition is met or false otherwise.
 */
#define WAIT_FOR(condition, attempts, delay_func, delay, result)     \
do {                                                         \
    result = false;                                         \
    uint8_t remaining_attempts = (attempts);                \
    do {                                                     \
           if (condition)                                    \
           {                                                 \
               result = true;                               \
               break;                                        \
           }                                                 \
           delay_func(delay);                          \
    } while (--remaining_attempts);                          \
} while(0)

#endif SLEEP_H