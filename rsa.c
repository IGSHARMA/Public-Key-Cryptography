#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <gmp.h>
#include "numtheory.h"
#include "randstate.h"

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    mpz_t totient, p_minus_one, q_minus_one, gcd_e_n;
    mpz_inits(p_minus_one, q_minus_one, gcd_e_n, totient, NULL);

    uint64_t pbits = (random() % (nbits / 2)) + (nbits / 4);
    uint64_t qbits = nbits - pbits;
    make_prime(p, pbits, iters);
    make_prime(q, qbits, iters);
    mpz_sub_ui(p_minus_one, p, 1);
    mpz_sub_ui(q_minus_one, q, 1);
    mpz_mul(n, p, q);
    mpz_mul(totient, p_minus_one, q_minus_one);

    do {
        mpz_urandomb(e, state, nbits);
        gcd(gcd_e_n, e, totient);
    } while (mpz_cmp_ui(gcd_e_n, 1) != 0);
    mpz_clears(totient, p_minus_one, q_minus_one, gcd_e_n, NULL);
}

//Write a public RSA key to pbfile
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n%Zx\n%Zx\n", n, e, s);
    fprintf(pbfile, "%s\n", username);
}

//Reads a public RSA key from pbfile
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n%Zx\n%Zx\n", n, e, s);
    fscanf(pbfile, "%s\n", username);
}

//Creates a new RSA private key d given primes p and q and public exponent e
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    mpz_t totient, p_minus_one, q_minus_one;
    mpz_inits(totient, p_minus_one, q_minus_one, NULL);
    mpz_sub_ui(p_minus_one, p, 1);
    mpz_sub_ui(q_minus_one, q, 1);
    mpz_mul(totient, p_minus_one, q_minus_one);
    mod_inverse(d, e, totient);
    mpz_clears(totient, p_minus_one, q_minus_one, NULL);
}

//Writes a private RSA key to pvfile
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", n, d);
}

//Read a private RSA key from pvfile
void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n%Zx\n", n, d);
}

//Performs RSA encryption
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n);
}

//Encrypts the contents of infile, writing the  encrypted contents to outfile
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    mpz_t c, m;
    mpz_inits(c, m, NULL);
    size_t k = (mpz_sizeinbase(n, 2) - 1) / 8;
    uint8_t *k_array = calloc(k, sizeof(uint8_t));
    k_array[0] = 0xFF;
    uint64_t j = 1;
    while (j > 0) {
        j = fread(k_array + 1, sizeof(uint8_t), k - 1, infile);
        mpz_import(m, j + 1, 1, sizeof(uint8_t), 1, 0, k_array);
        rsa_encrypt(c, m, e, n);
        gmp_fprintf(outfile, "%Zx\n", c);
    }
    free(k_array);
    mpz_clears(c, m, NULL);
}

//Performs RSA decryption
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n);
}

//Decrypts the contents of infile, writing the the decrypted contents to outfile
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    mpz_t c, m;
    mpz_inits(c, m, NULL);
    size_t k = (mpz_sizeinbase(n, 2) - 1) / 8;
    uint8_t *k_array = calloc(k, sizeof(uint8_t));
    size_t j;
    while (feof(infile) == 0) {
        gmp_fscanf(infile, "%Zx\n", c);
        rsa_decrypt(m, c, d, n);
        mpz_export(k_array, &j, 1, sizeof(uint8_t), 1, 0, m);
        fwrite(k_array + 1, sizeof(uint8_t), j - 1, outfile);
    }
    mpz_clears(c, m, NULL);
    free(k_array);
}

//Performs RSA signing
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d, n);
}

//Performs RSA verification
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    mpz_t output;
    mpz_init(output);
    pow_mod(output, s, e, n);
    if (mpz_cmp(output, m) == 0) {
        mpz_clear(output);
        return true;
    } else {
        mpz_clear(output);
        return false;
    }
    mpz_clear(output);
}
