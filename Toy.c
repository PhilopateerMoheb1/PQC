#include <stdio.h>
#include "toy.h"

#define TK_Q 97

// Helper function for vector addition in Z97
static void vector_add(short* result, const short* a, const short* b, int size) {
    for (int i = 0; i < size; ++i) {
        result[i] = (a[i] + b[i]) % TK_Q;
    }
}
// Helper function for polynomial multiplication in Z97[X]/(X^4+1)
static void toy_polmul_naive(short* dst, const short* a, const short* b, int add_to_dst) {
    if (!add_to_dst) {
        for (int i = 0; i < 4; ++i) {
            dst[i] = 0;
        }
    }

    for (int i = 0; i < 4; ++i) {
        dst[i] = (dst[i] + a[0] * b[i]) % TK_Q;
        dst[(i + 1) % 4] = (dst[(i + 1) % 4] + a[1] * b[i]) % TK_Q;
        dst[(i + 2) % 4] = (dst[(i + 2) % 4] + a[2] * b[i]) % TK_Q;
        dst[(i + 3) % 4] = (dst[(i + 3) % 4] + a[3] * b[i]) % TK_Q;
    }
}


// Helper function to fill an array with small random numbers mod q
static void fill_random(short *array, int size) {
    for (int i = 0; i < size; ++i) {
        int val = rand() & 3; // Uniform distribution
        array[i] = (val & 1) - ((val >> 1) & 1); // Binomial distribution
    }
}


// Implementation of toy_gen function
void toy_gen(short *A, short *t, short *s) {
    // Fill K*K-matrix A with uniformly random numbers mod q
    for (int i = 0; i < TK_K * TK_K * TK_N; ++i) {
        A[i] = rand() % TK_Q;
    }

    // Fill K-vectors s & e with small random numbers mod q
    fill_random(s, TK_K * TK_N);
    fill_random(t, TK_K * TK_N);

    // Matrix-vector multiplication in Zq[X]/(X^n+1)
    for (int i = 0; i < TK_K; ++i) {
        for (int j = 0; j < TK_N; ++j) {
            for (int k = 0; k < TK_K; ++k) {
                t[i * TK_N + j] += A[i * TK_K * TK_N + k * TK_N + j] * s[k * TK_N + j];
            }
        }
    }
}
// Implementation of toy_enc function
void toy_enc(const short* A, const short* t, int plain, short* u, short* v) {
    short r[TK_K * TK_N];
    short e1[TK_K * TK_N];
    short e2[TK_N];

    // Fill K-vectors r & e1, and the scalar (one-polynomial) e2 with small random numbers mod q
    fill_random(r, TK_K * TK_N);
    fill_random(e1, TK_K * TK_N);
    fill_random(e2, TK_N);

    // Transpose of matrix A
    short A_transpose[TK_K * TK_K * TK_N];
    for (int i = 0; i < TK_K; ++i) {
        for (int j = 0; j < TK_K; ++j) {
            for (int k = 0; k < TK_N; ++k) {
                A_transpose[j * TK_K * TK_N + i * TK_N + k] = A[i * TK_K * TK_N + j * TK_N + k];
            }
        }
    }

    // Calculate u = A_transpose . r + e1
    toy_polmul_naive(u, A_transpose, r, 0);
    vector_add(u, u, e1, TK_K * TK_N);

    // Calculate v = t dot r + e2 + msg_bits * q/2
    int msg_bits[TK_N];
    for (int i = 0; i < TK_N; ++i) {
        msg_bits[i] = (plain >> i) & 1;
    }

    toy_polmul_naive(v, t, r, 0);
    vector_add(v, v, e2, TK_N);

    for (int i = 0; i < TK_N; ++i) {
        v[i] += msg_bits[i] * (TK_Q / 2);
        v[i] %= TK_Q;
    }
}


// Helper function for vector subtraction in Z97
static void vector_subtract(short* result, const short* a, const short* b, int size) {
    for (int i = 0; i < size; ++i) {
        result[i] = (a[i] - b[i] + TK_Q) % TK_Q;
    }
}

// Implementation of toy_dec function
int toy_dec(const short* s, const short* u, const short* v) {
    short p[TK_N];

    // Calculate p = v - s dot u
    toy_polmul_naive(p, s, u, 1);

    // Subtract p from v and apply modulo operation
    for (int i = 0; i < TK_N; ++i) {
        p[i] = (v[i] - p[i] + TK_Q) % TK_Q;
    }

    // Recover plaintext from p
    int plain = 0;
    for (int i = 0; i < TK_N; ++i) {
        int val = p[i];

        // Adjust for values greater than q/2
        if (val > TK_Q / 2) {
            val -= TK_Q;
        }

        // Determine the bit and update the plaintext
        int bit = abs(val) > TK_Q / 4;
        plain |= bit << i;
    }

    return plain;
}



// Helper function to print vectors
static void print_vector(const char *name, const short *vector, int size) {
    printf("* %s: [", name);
    for (int i = 0; i < size; ++i) {
        printf("%d", vector[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

// Main program with error handling
int main() {
    // Seed for pseudo-random number generation
    srand(42);

    // Key and ciphertext vectors
    short A[TK_K * TK_K * TK_N];
    short t[TK_K * TK_N];
    short s[TK_K * TK_N];
    short u[TK_K * TK_N];
    short v[TK_N];

    // Generate keys
    toy_gen(A, t, s);

    // Print generated keys
    printf("******************************************\n");
    printf("*             Generated Keys             *\n");
    printf("******************************************\n");
    print_vector("Public Key (A)", A, TK_K * TK_K * TK_N);
    print_vector("Public Key (t)", t, TK_K * TK_N);
    print_vector("Private Key (s)", s, TK_K * TK_N);

    // Plaintext message (4 bits)
    int plain = 9; // Change this value to test different plaintexts

    // Encrypt the message
    toy_enc(A, t, plain, u, v);

    // Print intermediate and ciphertext vectors
    printf("\n******************************************\n");
    printf("*         Encryption Process             *\n");
    printf("******************************************\n");
    print_vector("Plaintext Message", &plain, 1);
    print_vector("Random Vector (u)", u, TK_K * TK_N);
    print_vector("Ciphertext Vector (v)", v, TK_N);

    // Decrypt the ciphertext
    int decrypted_plain = toy_dec(s, u, v);

    // Print decrypted plaintext
    printf("\n******************************************\n");
    printf("*         Decryption Process             *\n");
    printf("******************************************\n");
    printf("* Decrypted Plaintext: %d\n", decrypted_plain);

    // Verify correctness
    if (plain == decrypted_plain) {
        printf("\n******************************************\n");
        printf("*     Verification: Successful!          *\n");
        printf("******************************************\n");
    } else {
        printf("\n******************************************\n");
        printf("*   Verification: Mismatch!              *\n");
        printf("******************************************\n");
    }

    return 0;
}
