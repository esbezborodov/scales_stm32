#ifndef SCALE_H
#define SCALE_H

/*
 * Conversion of raw ADC readings to grams. Pure integer math without
 * hardware access, so it can be unit tested on the host.
 */

#include <stdint.h>

typedef struct {
    int32_t tare;          /* raw reading of the empty scale */
    int32_t counts_per_kg; /* sensitivity, see SCALE_COUNTS_PER_KG */
} scale_t;

void scale_init(scale_t *scale, int32_t counts_per_kg);

/* Remember the current raw reading as zero weight. */
void scale_set_tare(scale_t *scale, int32_t raw);

/* Raw reading minus tare. */
int32_t scale_net_counts(const scale_t *scale, int32_t raw);

/* Weight in grams, rounded to the nearest gram. Negative below the tare. */
int32_t scale_to_grams(const scale_t *scale, int32_t raw);

#endif /* SCALE_H */
