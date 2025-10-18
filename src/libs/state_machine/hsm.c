#include "hsm.h"

#include <stdint.h>
#include <stdlib.h>

#include "assert.h"
#include "stdio.h"

static uint8_t Build_Ancestor_List(hsm_state_t const **const ancestor_list, hsm_state_t const *const source,
                                   hsm_state_t const *const ancestor);
static void Exit_Up_To(hsm_t *const sm, hsm_state_t const *const source, hsm_state_t const *const lca);
static void Enter_Down_To(hsm_t *const sm, hsm_state_t const *const target, hsm_state_t const *const lca);
static hsm_state_t *Find_Lca(hsm_state_t const *const source, hsm_state_t const *const target);

static hsm_status_t Top_Handler(hsm_t *const sm, hsm_sig_t const signal);

hsm_state_t hsm_top_state = {
    .parent = NULL,
    .handler = &Top_Handler,
};

static hsm_event_t entry_event = {
    .signal = HSM_SIG_ENTRY,
    .param = NULL,
};
static hsm_event_t exit_event = {
    .signal = HSM_SIG_EXIT,
    .param = NULL,
};
static hsm_event_t init_event = {
    .signal = HSM_SIG_INIT,
    .param = NULL,
};

static hsm_status_t Top_Handler(hsm_t *const sm, hsm_sig_t const signal) {
   (void)sm;
   (void)signal;

   // top sinks all events
   return HSM_STATUS_IGNORED;
}

void Hsm_Init(hsm_t *const sm, hsm_state_t const *const pInitial_state) {
   assert(sm);
   assert(pInitial_state);
   assert(Find_Lca(pInitial_state, &hsm_top_state) == &hsm_top_state);

   sm->target_state = NULL;

   // post entry action from top to leaf
   Enter_Down_To(sm, pInitial_state, NULL);

   sm->curr_state = (hsm_state_t *)pInitial_state;

   // post init event to initial state
   Hsm_Dispatch(sm, HSM_SIG_INIT);
}

void Hsm_Dispatch(hsm_t *const sm, hsm_sig_t const signal) {
   assert(sm);

   hsm_status_t ret = HSM_STATUS_UNHANDLED;
   hsm_state_t *state = sm->curr_state;
   assert(state->handler);

   // try to handle the event
   do {
      ret = state->handler(sm, signal);
      // transitions are illegal as outcome of entry or exit
      if (ret == HSM_STATUS_TRAN) {
         assert(signal != HSM_SIG_ENTRY);
         assert(signal != HSM_SIG_EXIT);
      }
      // init signal must result in nothing or transition
      if (signal == HSM_SIG_INIT) {
         assert((ret == HSM_STATUS_UNHANDLED) || (ret == HSM_STATUS_TRAN));
      }

      // init (if not explicitely causing transition), entry and exit actions are always processed by the current state
      // this saves having to explicitely define these in the state handler of every state even if they are not needed
      if ((signal == HSM_SIG_ENTRY) || (signal == HSM_SIG_EXIT) ||
          ((signal == HSM_SIG_INIT) && (ret != HSM_STATUS_TRAN))) {
         ret = HSM_STATUS_HANDLED;
      }
      // if not, try to have a parent handle it
      state = state->parent;
      // this loop will always at least end on the top state, which is the ultimate parent of all states and sinks all
      // events
   } while (ret == HSM_STATUS_UNHANDLED);

   if (ret == HSM_STATUS_TRAN) {
      // find least common ancestor (lca) of transition source and target
      hsm_state_t *lca = NULL;
      // is transition to self?
      if (sm->curr_state == sm->target_state) {
         // set common ancestor as parent to exit and re-enter current state
         lca = sm->curr_state->parent;
      } else {
         lca = Find_Lca(sm->curr_state, sm->target_state);
      }

      // post exit to ancestor list (excluding lca)
      Exit_Up_To(sm, sm->curr_state, lca);

      // post entry to child list (excluding lca)
      Enter_Down_To(sm, sm->target_state, lca);

      // finally move to new state
      sm->curr_state = sm->target_state;

      // dispatch init signal to final state (may be nested causing more transitions to a final-final state)
      do {
         ret = sm->curr_state->handler(sm, HSM_SIG_INIT);
         // init signal must result in nothing or transition
         assert((ret == HSM_STATUS_UNHANDLED) || (ret == HSM_STATUS_TRAN));

         if (ret == HSM_STATUS_TRAN) {
            // ensure that target of initial transition is child of source state
            // TODO: too expensive to do on each transition?
            assert(Find_Lca(sm->curr_state, sm->target_state) == sm->curr_state);

            // post entry to child list (excluding lca)
            Enter_Down_To(sm, sm->target_state, sm->curr_state);

            // finally move to new final state
            sm->curr_state = sm->target_state;
         }
         // check if nested initial transition
      } while (ret == HSM_STATUS_TRAN);
   }
}

hsm_state_t *Hsm_State_Get(hsm_t const *const sm) {
   return sm->curr_state;
}

void Hsm_State_Set(hsm_t *const sm, hsm_state_t *const pTarget_state) {
   sm->curr_state = pTarget_state;
}

static uint8_t Build_Ancestor_List(hsm_state_t const **const ancestors_list, hsm_state_t const *const source,
                                   hsm_state_t const *const ancestor) {
   uint8_t depth = 0;
   if (source != ancestor) {
      ancestors_list[0] = source;
      depth++;

      while ((ancestors_list[depth - 1]->parent != ancestor) && (depth < HSM_MAX_DEPTH)) {
         ancestors_list[depth] = ancestors_list[depth - 1]->parent;
         depth++;
      }
   }

   // make sure we didn't blew past max
   assert(depth <= HSM_MAX_DEPTH);

   return depth;
}

static void Exit_Up_To(hsm_t *const sm, hsm_state_t const *const source, hsm_state_t const *const lca) {
   hsm_state_t const *curr_state = source;
   hsm_status_t ret = HSM_STATUS_UNHANDLED;

   // post exit from current to child of LCA
   while (curr_state && (curr_state != lca)) {
      ret = curr_state->handler(sm, HSM_SIG_EXIT);
      // transitions are illegal as outcome of exit
      assert(ret != HSM_STATUS_TRAN);

      curr_state = curr_state->parent;
   }
}

static void Enter_Down_To(hsm_t *const sm, hsm_state_t const *const target, hsm_state_t const *const lca) {
   // build child list from lca to target
   hsm_state_t *ancestors[HSM_MAX_DEPTH] = {0};
   uint8_t depth = Build_Ancestor_List(ancestors, target, lca);
   hsm_status_t ret = HSM_STATUS_UNHANDLED;

   // post entry to all starting with topmost
   for (uint8_t i = depth; i > 0; --i) {
      ret = ancestors[i - 1]->handler(sm, HSM_SIG_ENTRY);
      // transitions are illegal as outcome of entry
      assert(ret != HSM_STATUS_TRAN);
   }
}

static hsm_state_t *Find_Lca(hsm_state_t const *const source, hsm_state_t const *const target) {
   // note: will always at least find top in common
   // starting from source of transition up to top
   hsm_state_t *tentative_lca_source = source;
   hsm_state_t *tentative_lca_target = target;
   // for each from target to top, check if is tentative lca
   while (tentative_lca_source != tentative_lca_target) {
      if (tentative_lca_target == &hsm_top_state) {
         // reached target top, climb source ancestors and start again
         tentative_lca_target = target;
         tentative_lca_source = tentative_lca_source->parent;
      } else {
         // climb target ancestors
         tentative_lca_target = tentative_lca_target->parent;
      }
   }

   return tentative_lca_source;
}
