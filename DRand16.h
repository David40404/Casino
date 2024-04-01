#pragma once
#include <time.h>

unsigned short DRand16() {
    static unsigned int seed = time(NULL);
    static unsigned int special_seed = seed;

    special_seed ^= (1 ^ seed) * 3 + 44;
    seed ^= 47 * (seed >> 2) + 84 + special_seed;

    return static_cast <unsigned short> (seed % UINT16_MAX);
}