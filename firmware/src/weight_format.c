#include "weight_format.h"

#include <stdbool.h>

/* Write the decimal digits of value (at least min_digits) into buf, return the count. */
static size_t put_digits(uint32_t value, size_t min_digits, char *buf)
{
    char tmp[10];
    size_t n = 0;
    do {
        tmp[n++] = (char)('0' + value % 10u);
        value /= 10u;
    } while (value != 0u || n < min_digits);

    for (size_t i = 0; i < n; i++) {
        buf[i] = tmp[n - 1u - i];
    }
    return n;
}

/* |value| without overflow for INT32_MIN. */
static uint32_t magnitude(int32_t value)
{
    return (value < 0) ? (uint32_t)(-(value + 1)) + 1u : (uint32_t)value;
}

size_t format_int(int32_t value, char *buf, size_t size)
{
    char text[12]; /* "-2147483648" */
    size_t len = 0;
    if (value < 0) {
        text[len++] = '-';
    }
    len += put_digits(magnitude(value), 1u, &text[len]);

    if (len + 1u > size) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        buf[i] = text[i];
    }
    buf[len] = '\0';
    return len;
}

size_t format_weight(int32_t grams, char *buf, size_t size)
{
    /* Round to hundredths of a kilogram (10 g), half away from zero. */
    const bool negative = grams < 0;
    const uint32_t centi = (magnitude(grams) + 5u) / 10u;

    char text[WEIGHT_TEXT_SIZE];
    size_t len = 0;
    if (negative && centi != 0u) {
        text[len++] = '-';
    }
    len += put_digits(centi / 100u, 1u, &text[len]);
    text[len++] = '.';
    len += put_digits(centi % 100u, 2u, &text[len]);
    text[len++] = ' ';
    text[len++] = 'k';
    text[len++] = 'g';

    if (len + 1u > size) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        buf[i] = text[i];
    }
    buf[len] = '\0';
    return len;
}
