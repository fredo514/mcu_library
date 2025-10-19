#include "msgq.h"
#include "unity.h"

msgq_t* queue;

void setUp(void) {
   queue = Msgq_Create(256);
}

void tearDown(void) {
   free(queue);
}

void test_start_here(void) {
   TEST_FAIL_MESSAGE("Update me!");
}