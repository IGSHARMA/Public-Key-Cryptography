#include <stdio.h>
#include <stdint.h>
#include <gmp.h>
#include "randstate.h"

gmp_randstate_t state;
//Intializes the state
void randstate_init(uint64_t seed) {
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
}
//Clears the state
void randstate_clear(void) {
    gmp_randclear(state);
}
