#include <string.h>

#include "mock_core.h"
#include "msgq.h"
#include "unity.h"

msgq_t* pQueue;

void setUp(void) {
   pQueue = Msgq_Create(256);
   Msgq_Init(pQueue);

   uint8_t msg = 42;
   Msgq_Push(pQueue, &msg, sizeof(msg));

   Msgq_Init(pQueue);
}

void tearDown(void) {
   free(pQueue);
}

void test_init(void) {
   // should be empty after init
   TEST_ASSERT_FALSE(Msgq_Is_Msg_Available(pQueue));
}

void test_push_one(void) {
   char msg_in[32] = "Hello, World!";

   TEST_ASSERT_TRUE(Msgq_Push(pQueue, &msg_in, strnlen(msg_in, sizeof(msg_in)) + 1));

   // should be able to retrieve
   TEST_ASSERT_TRUE(Msgq_Is_Msg_Available(pQueue));
   char msg_out[sizeof(msg_in)];
   uint8_t msg_len = 0;
   TEST_ASSERT_TRUE(Msgq_Pop(pQueue, msg_out, sizeof(msg_out), &msg_len));
   TEST_ASSERT_EQUAL_STRING_LEN(msg_in, msg_out, sizeof(msg_in));
   TEST_ASSERT_EQUAL(strnlen(msg_in, sizeof(msg_in)) + 1, msg_len);

   // should be empty now
   TEST_ASSERT_FALSE(Msgq_Is_Msg_Available(pQueue));
}

void test_push_2_retrieve_in_order(void) {
   TEST_FAIL();
}

void test_push_full(void) {
   TEST_FAIL();
}

void test_push_wrap(void) {
   TEST_FAIL();
}

void test_pop_empty(void) {
   TEST_FAIL();
}