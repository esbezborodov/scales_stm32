#ifndef WEIGHT_FORMAT_H
#define WEIGHT_FORMAT_H

/*
 * Text formatting for the LCD without printf (saves several kilobytes of
 * flash and has no newlib dependencies). Pure functions, unit tested on the host.
 */

#include <stddef.h>
#include <stdint.h>

/* Enough for "-2147483.65 kg" and for any int32_t with a sign. */
#define WEIGHT_TEXT_SIZE 16u

/*
 * Format grams as kilograms with two decimals, e.g. 1234 -> "1.23 kg",
 * -50 -> "-0.05 kg". The value is rounded to 10 g; "-0.00" is never shown.
 * Returns the string length, or 0 if the buffer is too small.
 */
size_t format_weight(int32_t grams, char *buf, size_t size);

/* Format a signed integer in decimal. Returns the length, or 0 if it does not fit. */
size_t format_int(int32_t value, char *buf, size_t size);

#endif /* WEIGHT_FORMAT_H */
