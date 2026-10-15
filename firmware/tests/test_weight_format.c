#include <stdint.h>

#include "test.h"
#include "weight_format.h"

static const char *weight(int32_t grams)
{
    static char buf[WEIGHT_TEXT_SIZE];
    format_weight(grams, buf, sizeof(buf));
    return buf;
}

static const char *integer(int32_t value)
{
    static char buf[16];
    format_int(value, buf, sizeof(buf));
    return buf;
}

static void test_weight(void)
{
    CHECK_STR("0.00 kg", weight(0));
    CHECK_STR("1.23 kg", weight(1234));
    CHECK_STR("1.24 kg", weight(1235)); /* rounded to 10 g */
    CHECK_STR("0.01 kg", weight(5));
    CHECK_STR("0.00 kg", weight(4));
    CHECK_STR("1.25 kg", weight(1250));
    CHECK_STR("5.00 kg", weight(5000));
    CHECK_STR("12.35 kg", weight(12345));
}

static void test_negative_weight(void)
{
    CHECK_STR("-0.05 kg", weight(-50));
    CHECK_STR("-1.25 kg", weight(-1250));
    CHECK_STR("0.00 kg", weight(-4)); /* never "-0.00" */
    CHECK_STR("-0.01 kg", weight(-5));
    CHECK_STR("-2147483.65 kg", weight(INT32_MIN));
}

static void test_weight_buffer(void)
{
    char small[8]; /* "1.23 kg" needs 8 bytes with the terminator */
    CHECK_INT(7, format_weight(1234, small, sizeof(small)));
    CHECK_STR("1.23 kg", small);
    CHECK_INT(0, format_weight(12345, small, sizeof(small)));
}

static void test_int(void)
{
    CHECK_STR("0", integer(0));
    CHECK_STR("7", integer(7));
    CHECK_STR("-123", integer(-123));
    CHECK_STR("2147483647", integer(INT32_MAX));
    CHECK_STR("-2147483648", integer(INT32_MIN));

    char small[4];
    CHECK_INT(3, format_int(-12, small, sizeof(small)));
    CHECK_INT(0, format_int(1234, small, sizeof(small)));
}

int main(void)
{
    test_weight();
    test_negative_weight();
    test_weight_buffer();
    test_int();
    return TEST_RESULT();
}
