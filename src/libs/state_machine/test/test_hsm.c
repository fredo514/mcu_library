#include <stdlib.h>

#include "hsm.h"
#include "unity.h"

enum my_sigs {
   A_SIG = HSM_SIG_USER_DEFINED_START,
   A1_SIG,
   A2_SIG,
   B_SIG,
   B1_SIG,
};

static void Clean_Sigs(void);

static hsm_status_t A_Handler(hsm_t* const sm, hsm_sig_t const signal);
static hsm_status_t A1_Handler(hsm_t* const sm, hsm_sig_t const signal);
static hsm_status_t A2_Handler(hsm_t* const sm, hsm_sig_t const signal);
static hsm_status_t B_Handler(hsm_t* const sm, hsm_sig_t const signal);
static hsm_status_t B1_Handler(hsm_t* const sm, hsm_sig_t const signal);

static hsm_state_t state_a = {
    .parent = &hsm_top_state,
    .handler = &A_Handler,
};

static hsm_state_t state_a1 = {
    .parent = &state_a,
    .handler = &A1_Handler,
};

static hsm_state_t state_a2 = {
    .parent = &state_a,
    .handler = &A2_Handler,
};

static hsm_state_t state_b = {
    .parent = &hsm_top_state,
    .handler = &B_Handler,
};

static hsm_state_t state_b1 = {
    .parent = &state_b,
    .handler = &B1_Handler,
};

hsm_t sm;

static hsm_sig_t last_top_event;
static hsm_sig_t last_a_event;
static hsm_sig_t last_a1_event;
static hsm_sig_t last_a2_event;
static hsm_sig_t last_b_event;
static hsm_sig_t last_b1_event;

void setUp(void) {
   Clean_Sigs();

   printf("\ninit\n");
   printf("state top: %x\n", &hsm_top_state);
   printf("state a: %x\n", &state_a);
   printf("state a1: %x\n", &state_a1);
   printf("state a2: %x\n", &state_a2);
   printf("state b: %x\n", &state_b);
   printf("state b1: %x\n", &state_b1);

   Hsm_Init(&sm, &state_a1);
}

void tearDown(void) {
}

static void Clean_Sigs(void) {
   last_top_event = HSM_SIG_EMPTY;
   last_a_event = HSM_SIG_EMPTY;
   last_a1_event = HSM_SIG_EMPTY;
   last_a2_event = HSM_SIG_EMPTY;
   last_b_event = HSM_SIG_EMPTY;
   last_b1_event = HSM_SIG_EMPTY;
}

hsm_status_t A_Handler(hsm_t* const sm, hsm_sig_t const signal) {
   last_a_event = signal;
   printf("state: a, signal: %u\n", signal);

   switch (signal) {
      case A1_SIG:
         HSM_TRAN(&state_a1);
         break;

      case B_SIG:
         HSM_TRAN(&state_b);
         break;
   }

   return HSM_STATUS_UNHANDLED;
}

hsm_status_t A1_Handler(hsm_t* const sm, hsm_sig_t const signal) {
   last_a1_event = signal;
   printf("state: a1, signal: %u\n", signal);

   switch (signal) {
      case A_SIG:
         HSM_TRAN(&state_a);
         break;

      case A1_SIG:
         HSM_TRAN(&state_a1);
         break;

      case A2_SIG:
         HSM_TRAN(&state_a2);
         break;

      case B1_SIG:
         HSM_TRAN(&state_b1);
         break;
   }

   return HSM_STATUS_UNHANDLED;
}

hsm_status_t A2_Handler(hsm_t* const sm, hsm_sig_t const signal) {
   last_a2_event = signal;
   printf("state: a2, signal: %u\n", signal);

   switch (signal) {
      case A2_SIG:
         return HSM_STATUS_HANDLED;
         break;
   }

   return HSM_STATUS_UNHANDLED;
}

hsm_status_t B_Handler(hsm_t* const sm, hsm_sig_t const signal) {
   last_b_event = signal;
   printf("state: b, signal: %u\n", signal);

   switch (signal) {}

   return HSM_STATUS_UNHANDLED;
}

hsm_status_t B1_Handler(hsm_t* const sm, hsm_sig_t const signal) {
   last_b1_event = signal;
   printf("state: b1, signal: %u\n", signal);

   switch (signal) {}

   return HSM_STATUS_UNHANDLED;
}

void test_init(void) {
   // is in provided initial state
   TEST_ASSERT_EQUAL(&state_a1, Hsm_State_Get(&sm));

   // posted init to initial
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_a1_event);

   // has posted entry signals to all parent states of initial state
   TEST_ASSERT_EQUAL(HSM_SIG_ENTRY, last_a_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_transition_to_self(void) {
   Clean_Sigs();

   Hsm_Dispatch(&sm, A1_SIG);

   TEST_ASSERT_EQUAL(&state_a1, Hsm_State_Get(&sm));

   // has posted exit signals to all parent states up to least common ancestor
   // TEST_ASSERT_EQUAL(HSM_SIG_EXIT, last_a1_event);

   // then entry signals down to the target
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_a1_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_transition_to_direct_child(void) {
   Clean_Sigs();

   Hsm_State_Set(&sm, &state_a);

   Hsm_Dispatch(&sm, A1_SIG);

   TEST_ASSERT_EQUAL(&state_a1, Hsm_State_Get(&sm));

   // then entry signals down to the target
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_a1_event);

   // but not others
   TEST_ASSERT_EQUAL(A1_SIG, last_a_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_transition_to_direct_sibling(void) {
   Clean_Sigs();

   Hsm_Dispatch(&sm, A2_SIG);

   TEST_ASSERT_EQUAL(&state_a2, Hsm_State_Get(&sm));

   // has posted exit signals to all parent states up to least common ancestor
   TEST_ASSERT_EQUAL(HSM_SIG_EXIT, last_a1_event);

   // then entry signals down to the target
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_a2_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_transition_to_direct_parent(void) {
   Clean_Sigs();

   Hsm_Dispatch(&sm, A_SIG);

   TEST_ASSERT_EQUAL(&state_a, Hsm_State_Get(&sm));

   // has posted exit signals to all parent states up to least common ancestor
   TEST_ASSERT_EQUAL(HSM_SIG_EXIT, last_a1_event);

   // then entry signals down to the target
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_a_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_complex_transition(void) {
   Clean_Sigs();

   Hsm_Dispatch(&sm, B1_SIG);

   // has posted exit signals to all parent states up to least common ancestor
   TEST_ASSERT_EQUAL(HSM_SIG_EXIT, last_a1_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EXIT, last_a_event);

   // then entry signals down to the target
   TEST_ASSERT_EQUAL(HSM_SIG_ENTRY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_b1_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
}

void test_dispatch_unhandled_pass_to_parent(void) {
   Clean_Sigs();

   Hsm_Dispatch(&sm, B_SIG);

   // has posted exit signals to all parent states up to least common ancestor
   TEST_ASSERT_EQUAL(HSM_SIG_EXIT, last_a1_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EXIT, last_a_event);

   // then entry signals down to the target
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_b_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_handled_internal(void) {
   Clean_Sigs();

   Hsm_State_Set(&sm, &state_a2);

   Hsm_Dispatch(&sm, A2_SIG);

   TEST_ASSERT_EQUAL(&state_a2, Hsm_State_Get(&sm));

   // then entry signals down to the target
   TEST_ASSERT_EQUAL(A2_SIG, last_a2_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a1_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_initial_transition(void) {
   TEST_FAIL();
}

void test_dispatch_entry_action(void) {
   TEST_FAIL();
}

void test_dispatch_exit_action(void) {
   TEST_FAIL();
}