#ifndef MATH_H
#define MATH_H

#include <stdio.h>
#include <stdint.h>

#define PRECISION (100)

typedef struct {
    int8_t intg;
    int8_t frac;
} float_t;

static inline int8_t integer_part(float x) {
    return (int8_t)x;
}

static inline int8_t fractional_part(float x) {
    return (int8_t)((x - integer_part(x)) * PRECISION);
}

#endif /* MATH_H */


