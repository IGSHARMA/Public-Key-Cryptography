#Assignment 6 - Public Key Cryptography 
In this assignment we generate a public and private key. Along with that we encrcypt an
decrypt a file. We create a private key and public key which mimics communications 
between one person to another. Where only those two can see the message and nobody else

```
randstate.c
```
In randstate we initalize the seed and clear the seed 

```
numtheory.c
```
This function is what allows rsa to run which also leads to the main files. This is more computational stuff like calculating the gcd, power mod, the modular inverse, checking if a number is prime, and making a prime number. 

```
rsa.c
```
This file read and write files. Everything that deals with files is in rsa.c. Along with that it makes a public key and private key. Also encrypting and decrypting files. 

```
keygen.c 
```
Generates a private a public key

```
encrypt.c 
```
Encrypts the file. 

```decrypt.c```
Prints out the message that was passed into encrypt. 


