#ifndef INDICATOR_H
#define INDICATOR_H

/*
 * Weight range indicator logic: every LED stands for one range of weights.
 * Pure function without hardware access, unit tested on the host.
 */

#include <stddef.h>
#include <stdint.h>

#define INDICATOR_NONE     (-1) /* empty scale: no LED is lit */
#define INDICATOR_OVERLOAD (-2) /* heavier than the last range */

/*
 * Return the index of the range that contains `grams`.
 * Range i covers (upper_bounds[i - 1], upper_bounds[i]]; the first range starts
 * above zero_band so that noise around zero does not light the first LED.
 * upper_bounds must be sorted in ascending order.
 */
int indicator_range(int32_t grams, const int32_t *upper_bounds, size_t count, int32_t zero_band);

#endif /* INDICATOR_H */
