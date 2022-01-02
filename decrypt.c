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
#define OPTIONS "i:n:o:vh"

int main(int argc, char **argv) {
    int opt = 0;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    char *pvname = "rsa.priv";
    bool verbose = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            if (optarg) {
                infile = fopen(optarg, "r");
            } else {
                fprintf(stderr, "./keygen: option requires an argument -- 'i'\n");
            }
            break;

        case 'o':
            if (optarg) {
                outfile = fopen(optarg, "w");
            } else {
                fprintf(stderr, "./keygen: option requires an argument -- 'o'\n");
            }
            break;

        case 'n':
            if (optarg) {
                pvname = optarg;
                break;
            } else {
                fprintf(stderr, "./keygen: option requires an argument -- 'n'\n");
            }
            break;

        case 'v':
            if (optarg) {
                verbose = true;
            } else {
                fprintf(stderr, "./keygen: option requires an argument -- 'v'\n");
            }
            break;

        case 'h':
            printf("SYNPOSIS\n");
            printf("   Encrypts data using RSA encryption.\n");
            printf("   Encrypted data is decrypted by the decrypt program.\n");
            printf("\n");
            printf("USAGE\n");
            printf("   ./encrypt [-hv] [-i infile] [-o outfile] -n pubkey\n");
            printf("\n");
            printf("OPTIONS\n");
            printf("   -h              Display program help and usage.\n");
            printf("   -v              Display verbose program output.\n");
            printf("   -i infile       Input file of data to encrypt (default: stdin).\n");
            printf("   -o outfile      Output file for encrypted data (default: stdout).\n");
            printf("   -n pbfile       Public key file (default: rsa.pub).\n");
            printf("\n");
            break;
        }
    }
    FILE *pvfile = fopen(pvname, "r");
    if (pvfile == NULL) {
        fprintf(stderr, "File is empty\n");
    }
    mpz_t n, e;
    mpz_inits(n, e, NULL);
    rsa_read_priv(n, e, pvfile);
    if (verbose) {
        //stats
    }
    rsa_decrypt_file(infile, outfile, n, e);
    fclose(infile);
    fclose(outfile);
    fclose(pvfile);
    mpz_clears(n, e, NULL);
}
