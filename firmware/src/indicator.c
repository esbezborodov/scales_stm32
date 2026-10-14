#include "indicator.h"

int indicator_range(int32_t grams, const int32_t *upper_bounds, size_t count, int32_t zero_band)
{
    if (grams <= zero_band) {
        return INDICATOR_NONE;
    }
    for (size_t i = 0; i < count; i++) {
        if (grams <= upper_bounds[i]) {
            return (int)i;
        }
    }
    return INDICATOR_OVERLOAD;
}
