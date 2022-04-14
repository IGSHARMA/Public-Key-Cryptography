#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include "numtheory.h"
#include "randstate.h"

//Computes the greatest common divisor of a and b
void gcd(mpz_t g, mpz_t a, mpz_t b) {
    mpz_t temp_a, temp_b, temp_t;
    mpz_inits(temp_a, temp_b, temp_t, NULL);

    mpz_set(temp_a, a);
    mpz_set(temp_b, b);
    while (mpz_sgn(temp_b)) {
        mpz_set(temp_t, temp_b); //t = b;
        mpz_mod(temp_b, temp_a, temp_b); //b = a % b;
        mpz_set(temp_a, temp_t); //a = t;
    }
    mpz_set(g, temp_a);
    mpz_clears(temp_a, temp_b, temp_t, NULL);
    return;
}

//Performs fast modular exponentiation
void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) {
    mpz_t v, p, p_sqr, v_times_p, temp_d, temp_n;

    mpz_init_set_ui(v, 1);
    mpz_init_set(p, a);
    mpz_init(p_sqr);
    mpz_init(v_times_p);
    mpz_init_set(temp_d, d);
    mpz_init_set(temp_n, n);
    while ((mpz_cmp_ui(temp_d, 0)) > 0) {
        if ((mpz_odd_p(temp_d)) > 0) {
            mpz_mul(v_times_p, v, p);
            mpz_mod(v, v_times_p, temp_n);
        }
        mpz_mul(p_sqr, p, p); //p = (p * p) ;
        mpz_mod(p, p_sqr, temp_n); //p = p % n
        mpz_fdiv_q_ui(temp_d, temp_d, 2); //d = d / 2;
    }
    mpz_set(o, v); //Sets o to the value of v
    mpz_clears(v, p, p_sqr, v_times_p, temp_d, temp_n, NULL);
    return;
}

bool is_prime(mpz_t n, uint64_t iters) {
    //mpz_t n_minus_one, two, r, temp, a, upper_bound, y, temp2;
    mpz_t n_minus_one;
    mpz_t two;
    mpz_t r;
    mpz_t temp;
    mpz_t a;
    mpz_t upper_bound;
    mpz_t y;
    //mpz_t temp2;
    //mpz_t temp_j;
    //mpz_t temp_s;

    //mpz_inits(n, n_minus_one, two, r, temp, a, upper_bound, y, temp2, NULL);
    mpz_init(n_minus_one);
    mpz_init(two);
    mpz_init(r);
    mpz_init(temp);
    mpz_init(a);
    mpz_init(upper_bound);
    mpz_init(y);
    //mpz_init(temp2);
    //mpz_init(temp_j);
    mpz_sub_ui(n_minus_one, n, 1);
    mpz_set_ui(two, 2);
    mp_bitcnt_t s = 2;
    //mpz_init2(temp_s, s);
    mpz_mod(temp, n, two);

    if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0) {
        mpz_clears(n_minus_one, two, r, temp, a, upper_bound, y, NULL);
        return true;
    }

    if (mpz_cmp_ui(n, 1) == 0 || mpz_cmp_ui(n, 0) == 0) {
        mpz_clears(n_minus_one, two, r, temp, a, upper_bound, y, NULL);
        return false;
    }

    while (mpz_divisible_2exp_p(n_minus_one, s)) {
        s++;
    }
    s--;
    mpz_tdiv_q_2exp(r, n_minus_one, s);
    mp_bitcnt_t j = 0;

    for (uint64_t i = 0; i < iters; i++) {
        mpz_sub_ui(upper_bound, n, 3);
        mpz_urandomm(a, state, upper_bound); //subtract 2 from a and do power mod
        mpz_sub_ui(a, a, 2);
        pow_mod(y, a, r, n);

        if ((mpz_cmp_ui(y, 1) != 0) && (mpz_cmp(y, n_minus_one) != 0)) {
            j = 1; //mpz_set_ui(j, 1);
            //mpz_set_ui(temp_j, j);
            while (j <= s - 1 && mpz_cmp(y, n_minus_one)) {
                pow_mod(y, y, two, n);
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(n_minus_one, two, r, temp, a, upper_bound, y, NULL);
                    return false;
                }
                j++; //j can be a normal int
            }
            if (mpz_cmp(y, n_minus_one) != 0) {
                mpz_clears(n_minus_one, two, r, temp, a, upper_bound, y, NULL);
                return false;
            }
        }
    }
    //Should I clear n ?
    mpz_clears(n_minus_one, two, r, temp, a, upper_bound, y, NULL);
    return true;
}

//Generates a new prime number stored in p
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mp_bitcnt_t mp_bits = bits;
    mpz_urandomb(p, state, mp_bits);
    while (!is_prime(p, iters) || mpz_sizeinbase(p, 2) < bits) {
        mpz_urandomb(p, state, mp_bits);
    }
}

//Computes the inverse i of a modulo n
void mod_inverse(mpz_t o, mpz_t a, mpz_t n) {
    //Declaring and setting all my temp variables
    mpz_t r, r_1, t, t_1, q, temp1, temp2;

    mpz_init_set(r, n);
    mpz_init_set(r_1, a);
    mpz_init_set_ui(t, 0);
    mpz_init_set_ui(t_1, 1);
    mpz_init_set_ui(q, 0);
    mpz_init(temp1);
    mpz_init(temp2);

    while (mpz_sgn(r_1) != 0) {
        mpz_fdiv_q(q, r, r_1);
        mpz_mul(temp1, q, r_1);
        mpz_sub(temp1, r, temp1);
        mpz_set(r, r_1);
        mpz_set(r_1, temp1);
        mpz_mul(temp2, q, t_1);
        mpz_sub(temp2, t, temp2);
        mpz_set(t, t_1);
        mpz_set(t_1, temp2);
    }
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(o, 0);
        mpz_clear(r);
        mpz_clear(r_1);
        mpz_clear(t);
        mpz_clear(t_1);
        mpz_clear(q);
        mpz_clear(temp1);
        mpz_clear(temp2);
        return;
    }
    if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n);
    }
    mpz_set(o, t); //Sets the output o to the value in t
    mpz_clears(r, r_1, t, t_1, q, temp1, temp2, NULL);
    return;
}
