#include "hsm.h"

#include <stdint.h>
#include <stdlib.h>

#include "assert.h"
#include "stdio.h"

static uint8_t Build_Ancestor_List(hsm_state_t const **const ancestor_list, hsm_state_t const *const source,
                                   hsm_state_t const *const ancestor);
static void Exit_Up_To(hsm_t *const sm, hsm_state_t const **const ancestor_list, uint8_t depth);
static void Enter_Down_To(hsm_t *const sm, hsm_state_t const **const ancestor_list, uint8_t depth);
static hsm_state_t *Find_Lca(hsm_state_t const *const source, hsm_state_t const *const target);

static hsm_status_t Top_Handler(hsm_t *const sm, hsm_sig_t const signal);

hsm_state_t hsm_top_state = {
    .parent = NULL,
    .handler = &Top_Handler,
};

static hsm_status_t Top_Handler(hsm_t *const sm, hsm_sig_t const signal) {
   (void)sm;
   (void)signal;
   return HSM_STATUS_IGNORED;
}

void Hsm_Init(hsm_t *const sm, hsm_state_t const *const pInitial_state) {
   assert(sm);
   assert(pInitial_state);

   sm->curr_state = NULL;
   sm->target_state = NULL;

   // find all ancestors of initial state up to top
   hsm_state_t const *target_ancestors[HSM_MAX_DEPTH] = {0};
   uint8_t depth = Build_Ancestor_List(target_ancestors, pInitial_state, NULL);

   // make sure we didn't blew past max
   assert(depth <= HSM_MAX_DEPTH);

   // post entry action from top to leaf
   Enter_Down_To(sm, target_ancestors, depth);

   sm->curr_state = (hsm_state_t *)pInitial_state;

   // post init event to initial state
   Hsm_Dispatch(sm, HSM_SIG_INIT);
}

void Hsm_Dispatch(hsm_t *const sm, hsm_sig_t const signal) {
   hsm_status_t ret = HSM_STATUS_UNHANDLED;
   hsm_state_t *state = sm->curr_state;

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
         assert((ret == HSM_STATUS_IGNORED) || (ret == HSM_STATUS_TRAN));
      }

      // init (if not explicitely causing transition), entry and exit actions are always handled
      if ((signal == HSM_SIG_ENTRY) || (signal == HSM_SIG_ENTRY) ||
          ((signal == HSM_SIG_INIT) && (ret != HSM_STATUS_TRAN))) {
         ret = HSM_STATUS_HANDLED;
      }
      // if not, try to have a parent handle it
      state = state->parent;
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

      // build ancestors list from current to lca
      hsm_state_t *source_ancestors[HSM_MAX_DEPTH] = {0};
      uint8_t source_depth = Build_Ancestor_List(source_ancestors, sm->curr_state, lca);

      // build child list from lca to target
      hsm_state_t *target_ancestors[HSM_MAX_DEPTH] = {0};
      uint8_t target_depth = Build_Ancestor_List(target_ancestors, sm->target_state, lca);

      // post exit to ancestor list (excluding lca)
      Exit_Up_To(sm, source_ancestors, source_depth);

      // post entry to child list (excluding lca)
      Enter_Down_To(sm, target_ancestors, target_depth);

      // set target as current
      sm->curr_state = sm->target_state;

      // dispatch init signal to final state
      // TODO: find a way to make this not recursive
      Hsm_Dispatch(sm, HSM_SIG_INIT);
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

   return depth;
}

static void Exit_Up_To(hsm_t *const sm, hsm_state_t const **const ancestor_list, uint8_t depth) {
   for (uint8_t i = 0; i < depth; ++i) {
      (void)ancestor_list[i]->handler(sm, HSM_SIG_EXIT);
   }
}

static void Enter_Down_To(hsm_t *const sm, hsm_state_t const **const ancestor_list, uint8_t depth) {
   for (uint8_t i = depth; i > 0; --i) {
      (void)ancestor_list[i - 1]->handler(sm, HSM_SIG_ENTRY);
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