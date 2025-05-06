#ifndef MATH_H
#define MATH_H

#include <stdio.h>
#include <stdint.h>

#define INTEGER_PART(x) ((int)(x))
#define FRACTIONAL_PART(x) ((int)((x - INTEGER_PART(x)) * 100))


typedef struct {
    uint8_t intg;
    uint8_t frac;
} float_t;

#endif /* MATH_H */


