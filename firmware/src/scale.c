#include "scale.h"

void scale_init(scale_t *scale, int32_t counts_per_kg)
{
    scale->tare = 0;
    scale->counts_per_kg = counts_per_kg;
}

void scale_set_tare(scale_t *scale, int32_t raw)
{
    scale->tare = raw;
}

int32_t scale_net_counts(const scale_t *scale, int32_t raw)
{
    return raw - scale->tare;
}

int32_t scale_to_grams(const scale_t *scale, int32_t raw)
{
    if (scale->counts_per_kg == 0) {
        return 0;
    }

    /* 64-bit intermediate: 24-bit counts * 1000 does not fit into 32 bits. */
    int64_t milli = (int64_t)scale_net_counts(scale, raw) * 1000;
    int64_t div = scale->counts_per_kg;
    if (div < 0) {
        /* A load cell mounted upside down gives a negative sensitivity. */
        div = -div;
        milli = -milli;
    }

    /* Round half away from zero so that +x and -x give symmetric results. */
    const int64_t grams = (milli >= 0) ? (milli + div / 2) / div : (milli - div / 2) / div;
    return (int32_t)grams;
}
