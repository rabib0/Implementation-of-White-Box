
#ifndef ENCODING_H
#define ENCODING_H

#include "structures.h"
#include "table.h"


const unsigned char MODULUS = 0xFF; // Modulus for GF(2^8) represented as hexadecimal 0xFF


// Addition in GF(2^8)
unsigned char gf2_8_addition(unsigned char a, unsigned char b) {
    return a ^ b;
}

// Subtraction in GF(2^8)
unsigned char gf2_8_subtraction(unsigned char a, unsigned char b) {
    return a ^ b;
}

// Multiplication in GF(2^8)
unsigned char gf2_8_multiplication(unsigned char a, unsigned char b) {
    unsigned char result = 0;
    for (int i = 0; i < 8; ++i) {
        if (b & 1) {
            result ^= a;
        }
        bool carry = a & 0x80; 
        a <<= 1;
        if (carry) {
            a ^= 0x1B; 
        }
        b >>= 1;
    }
    return result;
}

// Division in GF(2^8)
unsigned char gf2_8_division(unsigned char a, unsigned char b) {
    if (b == 0) {
        std::cerr << "Error: Division by zero." << std::endl;
        return 0; // Error code
    }

    unsigned char inverseB = 1;
    while (gf2_8_multiplication(b, inverseB) != 1) {
        inverseB++;
    }

    return gf2_8_multiplication(a, inverseB);
}

// Evaluate the function 2x + 4 in GF(2^8)
unsigned char evaluate_function(unsigned char x) {
    unsigned char term1 = gf2_8_multiplication(2, x); // 2x in GF(2^8)
    unsigned char term2 = 4; // 4 in GF(2^8)
    return gf2_8_addition(term1, term2); // 2x + 4 in GF(2^8)
}

// Calculate the inverse function of 2x + 4 in GF(2^8)
unsigned char inverse_function(unsigned char y) {
    unsigned char term = gf2_8_subtraction(y, 4); // y - 4 in GF(2^8)
    unsigned char x = gf2_8_multiplication(term, gf2_8_division(1, 2)); // Multiplicative inverse of 2 in GF(2^8)
    return x;
}

// Bijective function using Galois field
unsigned char bijectiveFunction(unsigned char x) {
    return x ^ 0x04;
}


int generateRandomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}


#endif /* ENCODING_H */
