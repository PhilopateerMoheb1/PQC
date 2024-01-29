
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
