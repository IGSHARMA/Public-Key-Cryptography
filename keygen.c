#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <gmp.h>
#include "randstate.h"
#include "numtheory.h"
#include "rsa.h"
#define OPTIONS "b:i:n:d:s:vh"

//Cite: @Erik
int main(int argc, char **argv) {
    int opt = 0;
    uint64_t bits = 256;
    uint64_t iters = 50;
    char *pbname = "rsa.pub";
    char *pvname = "rsa.priv";
    bool verbose = false;
    uint64_t seed = 0;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h':
            printf("SYNOPSIS\n");
            printf("   Generates an RSA public/private key pair..\n");
            printf("\n");
            printf("USAGE\n");
            printf("   ./keygen [-hv] [-b bits] -n pbfile -d pvfile\n");
            printf("\n");
            printf("OPTIONS\n");
            printf("   -h              Display program help and usage.\n");
            printf("   -v              Display verbose program output.\n");
            printf("   -b bits         Minimum bits needed for public key n.\n");
            printf(
                "   -c confidence   Miller-Rabin iterations for testing primes (default: 50).\n");
            printf("   -n pbfile       Public key file (default: rsa.pub).\n");
            printf("   -d pvfile       Private key file (default: rsa.priv).\n");
            printf("   -s seed         Random seed for testing.\n");
            exit(EXIT_SUCCESS);

        case 'b': //put in error message
            if (optarg) {
                bits = (uint64_t) optarg;
            } else {
                fprintf(stderr, "./keygen: option requires an argument -- 'b'\n");
            }
        case 'i': //put in error message
            if (optarg) {
                iters = (uint64_t) optarg;
            } else {
                fprintf(stderr, "./keygen: option requires an argument -- 'i'\n");
            }

        case 'n':
            if (optarg) {
                pbname = optarg;
                break;
            } else {
                fprintf(stderr, "./keygen: option requires an argument -- 'n'\n");
            }

        case 'd':
            if (optarg) {
                pvname = optarg;
                break;
            } else {
                fprintf(stderr, "./keygen: option requires an argument -- 'd'\n");
            }

        case 'v':
            if (optarg) {
                verbose = true;
            } else {
                fprintf(stderr, "./keygen: option requires an argument -- 'v'\n");
            }

        case 's':
            if (optarg == NULL) {
                seed = time(NULL);
            } else {
                seed = (uint64_t) optarg;
            }
        }
    }

    FILE *pbfile = fopen(pbname, "w");
    if (pbfile == NULL) {
        fprintf(stderr, "File is Null\n");
    }

    FILE *pvfile = fopen(pvname, "w");
    if (pvfile == NULL) {
        fprintf(stderr, "File is Null\n");
    }

    fchmod(fileno(pvfile), 0600);
    randstate_init(seed);

    mpz_t s, p, q, n, e, d;
    mpz_inits(s, p, q, n, e, d, NULL);

    rsa_make_pub(p, q, n, e, bits, iters);
    rsa_make_priv(d, e, p, q);

    char *username = getenv("USER");
    mpz_set_str(s, username, 62);
    rsa_sign(s, s, d, n);
    rsa_write_pub(n, e, s, username, pbfile);
    rsa_write_priv(n, d, pvfile);

    //if (verbose) {
    //print verbose
    //}
    //close(pbname);
    //close(pvname);
    randstate_clear();
    fclose(pbfile);
    fclose(pvfile);
    mpz_clears(s, p, q, n, e, d, NULL);
}
