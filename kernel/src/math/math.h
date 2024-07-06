#pragma once
#include <stdint.h>

struct Point {
    long X;
    long Y;
};

inline int abs(int val) {
    if (val < 0)
        return -val;
    return val;
}