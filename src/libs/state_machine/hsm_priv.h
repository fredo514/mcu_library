#ifndef HSM_PRIV_H
#define HSM_PRIV_H

struct _hsm_ctx {
   hsm_state_t* curr_state;   /* current active leaf state */
   hsm_state_t* target_state; /* transition target (set by handler via macro) */
};

#endif