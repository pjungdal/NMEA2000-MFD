#ifndef d_state_h_
#define d_state_h_

 typedef enum RAM_G_load{
    SAILST,
    STEER,
    COMB1,
    COMB2,
    EMPTY
} rg_state_t;
extern rg_state_t rg_state;
#endif