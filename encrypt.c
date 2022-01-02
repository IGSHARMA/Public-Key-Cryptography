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
    //char *infile_name = NULL;
    //char *outfile_name = NULL;
    char *pbname = "rsa.pub";
    bool verbose = false;
    //bool in_flag = false;
    //bool out_flag = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            if (optarg) {
                //in_flag = true;
                //infile_name = optarg;
                infile = fopen(optarg, "r");

            } else {
                fprintf(stderr, "./keygen: option requires an argument -- 'i'\n");
            }
            break;

        case 'o':
            if (optarg) {
                //out_flag = true;
                //outfile_name = optarg;
                outfile = fopen(optarg, "w");
            } else {
                fprintf(stderr, "./keygen: option requires an argument -- 'o'\n");
            }
            break;

        case 'n':
            if (optarg) {
                pbname = optarg;
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
    FILE *pbfile = fopen(pbname, "r");
    if (pbfile == NULL) {
        fprintf(stderr, "File is empty.\n");
    }
    mpz_t s, p, n, e, m;
    mpz_inits(s, p, n, e, m, NULL);
    char *username = getenv("USER");
    rsa_read_pub(n, e, s, username, pbfile); //valgrind
    if (verbose) {
        //stats
    }
    mpz_set_str(m, username, 62);
    if (!rsa_verify(m, s, e, n)) {
        exit(EXIT_FAILURE);
        //fprintf(stderr, "Error message.\n");
    }
    rsa_encrypt_file(infile, outfile, n, e); //valgrind
    mpz_clears(s, p, n, e, m, NULL);
    fclose(infile);
    fclose(outfile);
    fclose(pbfile);
}
