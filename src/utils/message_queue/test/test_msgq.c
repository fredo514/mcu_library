#include <stdlib.h>
#include <string.h>

#include "mock_core.h"
#include "msgq.h"
#include "unity.h"

#define QUEUE_SIZE (256)

msgq_t* pQueue;

void setUp(void) {
   pQueue = Msgq_Create(QUEUE_SIZE);
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
   // push a message
   char msg_in[32] = "Hello, World!";
   TEST_ASSERT_TRUE(Msgq_Push(pQueue, &msg_in, strnlen(msg_in, sizeof(msg_in)) + 1));

   // push another one
   float another_msg_in = 42.0f;
   TEST_ASSERT_TRUE(Msgq_Push(pQueue, &another_msg_in, sizeof(another_msg_in)));

   // should be able to retrieve
   TEST_ASSERT_TRUE(Msgq_Is_Msg_Available(pQueue));

   // first one out should be the first
   char msg_out[sizeof(msg_in)];
   uint8_t msg_len = 0;
   TEST_ASSERT_TRUE(Msgq_Pop(pQueue, msg_out, sizeof(msg_out), &msg_len));
   TEST_ASSERT_EQUAL_STRING_LEN(msg_in, msg_out, sizeof(msg_in));
   TEST_ASSERT_EQUAL(strnlen(msg_in, sizeof(msg_in)) + 1, msg_len);

   // should not be empty yet
   TEST_ASSERT_TRUE(Msgq_Is_Msg_Available(pQueue));

   // then the other
   float another_msg_out;
   TEST_ASSERT_TRUE(Msgq_Pop(pQueue, &another_msg_out, sizeof(another_msg_out), &msg_len));
   TEST_ASSERT_EQUAL_FLOAT(another_msg_in, another_msg_out);

   // should be empty now
   TEST_ASSERT_FALSE(Msgq_Is_Msg_Available(pQueue));
}

void test_push_not_enough_space_left(void) {
   // push a message that almost fills the queue
   char msg_in[QUEUE_SIZE - sizeof(float) + 1];
   TEST_ASSERT_TRUE(Msgq_Push(pQueue, &msg_in, sizeof(msg_in)));

   // try push another one, should fail
   float another_msg_in = 42.0f;
   TEST_ASSERT_FALSE(Msgq_Push(pQueue, &another_msg_in, sizeof(another_msg_in)));
}

void test_push_wrap(void) {
   // fill queue bu leave at least 1 byte out
   uint8_t msg_in = 0;
   for (size_t i = 0; i < (QUEUE_SIZE / 3); ++i) {
      msg_in = i;
      TEST_ASSERT_TRUE(Msgq_Push(pQueue, &msg_in, sizeof(msg_in)));
   }

   // pop 2 to leave 4 bytes free
   uint8_t msg_out;
   uint8_t msg_len;
   TEST_ASSERT_TRUE(Msgq_Pop(pQueue, &msg_out, sizeof(msg_out), &msg_len));
   TEST_ASSERT_EQUAL(0, msg_out);
   TEST_ASSERT_EQUAL(1, msg_len);
   TEST_ASSERT_TRUE(Msgq_Pop(pQueue, &msg_out, sizeof(msg_out), &msg_len));
   TEST_ASSERT_EQUAL(1, msg_out);
   TEST_ASSERT_EQUAL(1, msg_len);

   // should not be empty
   TEST_ASSERT_TRUE(Msgq_Is_Msg_Available(pQueue));

   // now push 4 bytes, should cross wrap boundary
   uint32_t another_msg_in = 0x12345678;
   TEST_ASSERT_TRUE(Msgq_Push(pQueue, &another_msg_in, sizeof(another_msg_in)));

   // should still have messages available
   TEST_ASSERT_TRUE(Msgq_Is_Msg_Available(pQueue));

   // empty queue of filler data
   for (size_t i = 2; i < (QUEUE_SIZE / 3); ++i) {
      TEST_ASSERT_TRUE(Msgq_Pop(pQueue, &msg_out, sizeof(msg_out), &msg_len));
      TEST_ASSERT_EQUAL(i, msg_out);
      TEST_ASSERT_EQUAL(sizeof(msg_in), msg_len);
   }

   // should still have messages available
   TEST_ASSERT_TRUE(Msgq_Is_Msg_Available(pQueue));

   // take out last element
   uint32_t another_msg_out = 0;
   TEST_ASSERT_TRUE(Msgq_Pop(pQueue, &another_msg_out, sizeof(another_msg_out), &msg_len));
   TEST_ASSERT_EQUAL_HEX32(another_msg_in, another_msg_out);
   TEST_ASSERT_EQUAL(sizeof(another_msg_in), msg_len);

   // should still now be empty
   TEST_ASSERT_FALSE(Msgq_Is_Msg_Available(pQueue));
}

void test_pop_empty(void) {
   TEST_ASSERT_FALSE(Msgq_Is_Msg_Available(pQueue));

   uint8_t msg_out = 0;
   uint8_t msg_len = 0;
   TEST_ASSERT_FALSE(Msgq_Pop(pQueue, &msg_out, sizeof(msg_out), &msg_len));

   // should not have writen to containers
   TEST_ASSERT_EQUAL(0, msg_out);
   TEST_ASSERT_EQUAL(0, msg_len);
}

void test_Is_Msg_Available_exactly_full(void) {
   // fill queue
   uint16_t msg_in = 0;
   for (size_t i = 0; i < (QUEUE_SIZE / 2 + sizeof(msg_in)); ++i) {
      msg_in = i;
      TEST_ASSERT_TRUE(Msgq_Push(pQueue, &msg_in, sizeof(msg_in)));
   }

   // should have messages available
   TEST_ASSERT_TRUE(Msgq_Is_Msg_Available(pQueue));
}