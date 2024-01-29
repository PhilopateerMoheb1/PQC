// toy.h

#ifndef TOY_H
#define TOY_H

#define TK_K 3
#define TK_N 4
#define TK_Q 97

// Helper function for polynomial multiplication in Z97[X]/(X^4+1)
static void toy_polmul_naive(short* dst, const short* a, const short* b, int add_to_dst);

// Main API functions
void toy_gen(short* A, short* t, short* s);
void toy_enc(const short* A, const short* t, int plain, short* u, short* v);
int toy_dec(const short* s, const short* u, const short* v);
// Helper function to print vectors
static void print_vector(const char* name, const short* vector, int size);

#endif // TOY_H
