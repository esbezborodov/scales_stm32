#ifndef NUMBER_PARTS_H
#define NUMBER_PARTS_H

#include <stdio.h>
#include <stdint.h>

#define PRECISION (100)

typedef struct {
    int16_t intg;
    int8_t frac;
} number_parts_t;

static inline int16_t integer_part(float x) {
    return (int16_t)x;
}

/* Always non-negative: the sign is carried by the integer part. */
static inline int8_t fractional_part(float x) {
    int8_t frac = (int8_t)((x - (int16_t)x) * PRECISION);
    return (frac < 0) ? (int8_t)-frac : frac;
}

#endif /* NUMBER_PARTS_H */


