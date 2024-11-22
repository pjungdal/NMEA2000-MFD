#ifndef _state_h_
#define _state_h_

 typedef enum ap_mode_t{
    A,
    ND,
    NAV,
    W,
    NFU,
    FU
} ap_mode_t;

 typedef enum ap_state_t{
    E,
    S
} ap_state_t;

extern ap_mode_t mode;
extern ap_mode_t previous_mode;
extern ap_state_t state;

bool apstate(const char * Command);
#endif