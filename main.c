// main.c

#include <stdio.h>
#include "toy.h"

int main()
{
    // Define parameters
    short A[TK_K * TK_K * TK_N];
    short t[TK_K * TK_N];
    short s[TK_K * TK_N];
    short u[TK_K * TK_N];
    short v[TK_N];

    // Key generation
    toy_gen(A, t, s);

    // Encryption
    int plaintext = 10; // Example plaintext (4 bits)
    toy_enc(A, t, plaintext, u, v);

    // Decryption
    int decrypted = toy_dec(s, u, v);

    // Display results
    printf("******************************************\n");
    printf("*               Cryptosystem              *\n");
    printf("******************************************\n");
    printf("Original plaintext: %d\n", plaintext);

    printf("\n******************************************\n");
    printf("*              Key Generation            *\n");
    printf("******************************************\n");
    printf("* Matrix A:\n");
    // Print A matrix
    for (int i = 0; i < TK_K * TK_K * TK_N; ++i)
    {
        printf("%4d ", A[i]);
        if ((i + 1) % TK_N == 0)
            printf("\n");
    }
    printf("* Vector t:\n");
    // Print t vector
    for (int i = 0; i < TK_K * TK_N; ++i)
    {
        printf("%4d ", t[i]);
    }
    printf("\n");
    printf("* Vector s:\n");
    // Print s vector
    for (int i = 0; i < TK_K * TK_N; ++i)
    {
        printf("%4d ", s[i]);
    }
    printf("\n");

    printf("\n******************************************\n");
    printf("*               Encryption               *\n");
    printf("******************************************\n");
    printf("* Vector u (Encrypted Message):\n");
    // Print u vector
    for (int i = 0; i < TK_K * TK_N; ++i)
    {
        printf("%4d ", u[i]);
    }
    printf("\n");
    printf("* Vector v (Ciphertext):\n");
    // Print v vector
    for (int i = 0; i < TK_N; ++i)
    {
        printf("%4d ", v[i]);
    }
    printf("\n");

    printf("\n******************************************\n");
    printf("*               Decryption               *\n");
    printf("******************************************\n");
    printf("* Decrypted plaintext: %d\n", decrypted);

    return 0;
}
