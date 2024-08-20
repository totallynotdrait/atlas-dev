#include "stdlib.h"

static uint32_t seed = 1;

// Function to set the seed value
void srand(uint32_t new_seed) {
    seed = new_seed;
}

// Function to generate a random number
uint32_t rand() {
    // Constants for the LCG algorithm
    const uint32_t a = 1664525;
    const uint32_t c = 1013904223;
    const uint32_t m = 0xFFFFFFFF; // 2^32

    // Update the seed value using the LCG formula
    seed = (a * seed + c) & m;

    // Return the generated random number
    return seed;
}