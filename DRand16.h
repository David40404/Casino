#pragma once
#include <time.h>

unsigned short DRand16() {
    static unsigned int seed = (unsigned int)time(NULL);

    static const unsigned int a = 2932355;
    static const unsigned int b = 5456263;

    seed ^= a * (seed >> 2) + b * seed;

    return static_cast <unsigned short> (seed % UINT16_MAX);
}
