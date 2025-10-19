#include <stdlib.h>

#include "hsm.h"
#include "unity.h"

enum my_sigs {
   A_SIG = HSM_SIG_USER_DEFINED_START,
   A1_SIG,
   A2_SIG,
   A3_SIG,
   A31_SIG,
   B_SIG,
   B1_SIG,
   A_INT_SIG,
   UNKNOWN_SIG,
};

static void Clean_Sigs(void);

static hsm_status_t A_Handler(hsm_t* const sm, hsm_sig_t const signal);
static hsm_status_t A1_Handler(hsm_t* const sm, hsm_sig_t const signal);
static hsm_status_t A2_Handler(hsm_t* const sm, hsm_sig_t const signal);
static hsm_status_t A3_Handler(hsm_t* const sm, hsm_sig_t const signal);
static hsm_status_t A31_Handler(hsm_t* const sm, hsm_sig_t const signal);
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

static hsm_state_t state_a3 = {
    .parent = &state_a,
    .handler = &A3_Handler,
};

static hsm_state_t state_a31 = {
    .parent = &state_a3,
    .handler = &A31_Handler,
};

static hsm_state_t state_b = {
    .parent = &hsm_top_state,
    .handler = &B_Handler,
};

static hsm_state_t state_b1 = {
    .parent = &state_b,
    .handler = &B1_Handler,
};

// hsm_t sm;
hsm_t* sm;

static hsm_sig_t last_top_event;
static hsm_sig_t last_a_event;
static hsm_sig_t last_a1_event;
static hsm_sig_t last_a2_event;
static hsm_sig_t last_a3_event;
static hsm_sig_t last_a31_event;
static hsm_sig_t last_b_event;
static hsm_sig_t last_b1_event;

static bool is_a_int_executed;

static bool is_a1_entry_executed;
static bool is_a1_exit_executed;
static bool is_a1_init_executed;

static hsm_status_t A_Handler(hsm_t* const sm, hsm_sig_t const signal) {
   last_a_event = signal;

   switch (signal) {
      case A_INT_SIG:
         is_a_int_executed = true;
         break;

      case A1_SIG:
         return HSM_TRAN(&state_a1);
         break;

      case A31_SIG:
         return HSM_TRAN(&state_a31);
         break;

      case B_SIG:
         return HSM_TRAN(&state_b);
         break;
   }

   return HSM_STATUS_UNHANDLED;
}

static hsm_status_t A1_Handler(hsm_t* const sm, hsm_sig_t const signal) {
   last_a1_event = signal;

   switch (signal) {
      case HSM_SIG_ENTRY:
         is_a1_entry_executed = true;
         break;

      case HSM_SIG_EXIT:
         is_a1_exit_executed = true;
         break;

      case HSM_SIG_INIT:
         is_a1_init_executed = true;
         break;

      case A_SIG:
         return HSM_TRAN(&state_a);
         break;

      case A1_SIG:
         return HSM_TRAN(&state_a1);
         break;

      case A2_SIG:
         return HSM_TRAN(&state_a2);
         break;

      case A3_SIG:
         return HSM_TRAN(&state_a3);
         break;

      case A31_SIG:
         return HSM_TRAN(&state_a31);
         break;

      case B1_SIG:
         return HSM_TRAN(&state_b1);
         break;
   }

   return HSM_STATUS_UNHANDLED;
}

static hsm_status_t A2_Handler(hsm_t* const sm, hsm_sig_t const signal) {
   last_a2_event = signal;

   switch (signal) {
      case A2_SIG:
         return HSM_STATUS_HANDLED;
         break;
   }

   return HSM_STATUS_UNHANDLED;
}

static hsm_status_t A3_Handler(hsm_t* const sm, hsm_sig_t const signal) {
   last_a3_event = signal;

   switch (signal) {
      case HSM_SIG_INIT:
         return HSM_TRAN(&state_a31);
         break;
   }

   return HSM_STATUS_UNHANDLED;
}

static hsm_status_t A31_Handler(hsm_t* const sm, hsm_sig_t const signal) {
   last_a31_event = signal;

   switch (signal) {
      case A_SIG:
         return HSM_TRAN(&state_a);
         break;
   }

   return HSM_STATUS_UNHANDLED;
}

static hsm_status_t B_Handler(hsm_t* const sm, hsm_sig_t const signal) {
   last_b_event = signal;

   switch (signal) {}

   return HSM_STATUS_UNHANDLED;
}

static hsm_status_t B1_Handler(hsm_t* const sm, hsm_sig_t const signal) {
   last_b1_event = signal;

   switch (signal) {}

   return HSM_STATUS_UNHANDLED;
}

void setUp(void) {
   sm = Hsm_Create();
   Hsm_Init(sm, &state_a1);

   Clean_Sigs();
}

void tearDown(void) {
   free(sm);
}

static void Clean_Sigs(void) {
   last_top_event = HSM_SIG_EMPTY;
   last_a_event = HSM_SIG_EMPTY;
   last_a1_event = HSM_SIG_EMPTY;
   last_a2_event = HSM_SIG_EMPTY;
   last_a3_event = HSM_SIG_EMPTY;
   last_a31_event = HSM_SIG_EMPTY;
   last_b_event = HSM_SIG_EMPTY;
   last_b1_event = HSM_SIG_EMPTY;

   is_a_int_executed = false;

   is_a1_entry_executed = false;
   is_a1_exit_executed = false;
   is_a1_init_executed = false;
}

void test_init(void) {
   Hsm_Init(sm, &state_a1);

   // is in provided initial state
   TEST_ASSERT_EQUAL(&state_a1, Hsm_State_Get(sm));

   // posted init to initial
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_a1_event);

   // has posted entry signals to all parent states of initial state
   TEST_ASSERT_EQUAL(HSM_SIG_ENTRY, last_a_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a3_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a31_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_handled_internal(void) {
   Hsm_State_Set(sm, &state_a2);

   Hsm_Dispatch(sm, A2_SIG);

   TEST_ASSERT_EQUAL(&state_a2, Hsm_State_Get(sm));

   // has posted event to current state
   TEST_ASSERT_EQUAL(A2_SIG, last_a2_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a1_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a3_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a31_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_unhandled_pass_to_parent_no_transition(void) {
   Hsm_Dispatch(sm, A_INT_SIG);

   // has posted event to current state up to parent that handle it
   TEST_ASSERT_EQUAL(A_INT_SIG, last_a1_event);
   TEST_ASSERT_EQUAL(A_INT_SIG, last_a_event);

   TEST_ASSERT_TRUE(is_a_int_executed);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a3_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a31_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_unhandled_top_sink(void) {
   Hsm_Dispatch(sm, UNKNOWN_SIG);

   // has posted event to current state up to parent that handle it
   TEST_ASSERT_EQUAL(UNKNOWN_SIG, last_a1_event);
   TEST_ASSERT_EQUAL(UNKNOWN_SIG, last_a_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a3_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a31_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_entry_action(void) {
   Hsm_Dispatch(sm, A1_SIG);

   TEST_ASSERT_TRUE(is_a1_entry_executed);
}

void test_dispatch_exit_action(void) {
   Hsm_Dispatch(sm, A1_SIG);

   TEST_ASSERT_TRUE(is_a1_exit_executed);
}

void test_dispatch_transition_to_self(void) {
   Hsm_Dispatch(sm, A1_SIG);

   TEST_ASSERT_EQUAL(&state_a1, Hsm_State_Get(sm));

   // then entry signals down to the target
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_a1_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a3_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a31_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_transition_to_direct_child(void) {
   Hsm_State_Set(sm, &state_a);

   Hsm_Dispatch(sm, A1_SIG);

   TEST_ASSERT_EQUAL(&state_a1, Hsm_State_Get(sm));

   // then entry signals down to the target
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_a1_event);

   // but not others
   TEST_ASSERT_EQUAL(A1_SIG, last_a_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a3_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a31_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_transition_to_direct_sibling(void) {
   Hsm_Dispatch(sm, A2_SIG);

   TEST_ASSERT_EQUAL(&state_a2, Hsm_State_Get(sm));

   // has posted exit signals to all parent states up to least common ancestor
   TEST_ASSERT_EQUAL(HSM_SIG_EXIT, last_a1_event);

   // then entry signals down to the target
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_a2_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a3_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a31_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_transition_to_direct_parent(void) {
   Hsm_Dispatch(sm, A_SIG);

   TEST_ASSERT_EQUAL(&state_a, Hsm_State_Get(sm));

   // has posted exit signals to all parent states up to least common ancestor
   TEST_ASSERT_EQUAL(HSM_SIG_EXIT, last_a1_event);

   // then entry signals down to the target
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_a_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a3_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a31_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_transition_to_own_grandchild(void) {
   Hsm_State_Set(sm, &state_a);

   Hsm_Dispatch(sm, A31_SIG);

   TEST_ASSERT_EQUAL(&state_a31, Hsm_State_Get(sm));

   TEST_ASSERT_EQUAL(A31_SIG, last_a_event);

   // then entry signals down to the target
   TEST_ASSERT_EQUAL(HSM_SIG_ENTRY, last_a3_event);
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_a31_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a1_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_transition_to_sibling_child(void) {
   Hsm_Dispatch(sm, A31_SIG);

   TEST_ASSERT_EQUAL(&state_a31, Hsm_State_Get(sm));

   // has posted exit signals to all parent states up to least common ancestor
   TEST_ASSERT_EQUAL(HSM_SIG_EXIT, last_a1_event);

   // then entry signals down to the target
   TEST_ASSERT_EQUAL(HSM_SIG_ENTRY, last_a3_event);
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_a31_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_transition_to_ancestor_sibling_child(void) {
   Hsm_Dispatch(sm, B1_SIG);

   // has posted exit signals to all parent states up to least common ancestor
   TEST_ASSERT_EQUAL(HSM_SIG_EXIT, last_a1_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EXIT, last_a_event);

   // then entry signals down to the target
   TEST_ASSERT_EQUAL(HSM_SIG_ENTRY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_b1_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a3_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a31_event);
}

void test_dispatch_transition_to_ancestor(void) {
   Hsm_State_Set(sm, &state_a31);

   Hsm_Dispatch(sm, A_SIG);

   TEST_ASSERT_EQUAL(&state_a, Hsm_State_Get(sm));

   // has posted exit signals to all parent states up to least common ancestor
   TEST_ASSERT_EQUAL(HSM_SIG_EXIT, last_a31_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EXIT, last_a3_event);

   // then entry signals down to the target
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_a_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a1_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_unhandled_pass_to_parent_then_transition(void) {
   Hsm_Dispatch(sm, B_SIG);

   // has posted exit signals to all parent states up to least common ancestor
   TEST_ASSERT_EQUAL(HSM_SIG_EXIT, last_a1_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EXIT, last_a_event);

   // then entry signals down to the target
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_b_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a3_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a31_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_initial_transition_nested(void) {
   Hsm_Dispatch(sm, A3_SIG);

   // has posted exit signals to all parent states up to least common ancestor
   TEST_ASSERT_EQUAL(HSM_SIG_EXIT, last_a1_event);

   // then entry signals down to the target, including nested initial transitions
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_a3_event);
   TEST_ASSERT_EQUAL(HSM_SIG_INIT, last_a31_event);

   // but not others
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_a2_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b_event);
   TEST_ASSERT_EQUAL(HSM_SIG_EMPTY, last_b1_event);
}

void test_dispatch_initial_action(void) {
   Hsm_Dispatch(sm, A1_SIG);

   TEST_ASSERT_TRUE(is_a1_init_executed);
}
