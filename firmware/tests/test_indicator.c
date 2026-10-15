#include "indicator.h"
#include "test.h"

static const int32_t ranges[] = {1250, 2500, 3750, 5000};
#define COUNT     (sizeof(ranges) / sizeof(ranges[0]))
#define ZERO_BAND 10

static int range(int32_t grams)
{
    return indicator_range(grams, ranges, COUNT, ZERO_BAND);
}

static void test_empty_scale(void)
{
    CHECK_INT(INDICATOR_NONE, range(0));
    CHECK_INT(INDICATOR_NONE, range(ZERO_BAND));
    CHECK_INT(INDICATOR_NONE, range(-300));
}

static void test_ranges(void)
{
    CHECK_INT(0, range(ZERO_BAND + 1));
    CHECK_INT(0, range(1250));
    CHECK_INT(1, range(1251));
    CHECK_INT(1, range(2500));
    CHECK_INT(2, range(2501));
    CHECK_INT(2, range(3750));
    CHECK_INT(3, range(3751));
    CHECK_INT(3, range(5000));
}

static void test_overload(void)
{
    CHECK_INT(INDICATOR_OVERLOAD, range(5001));
    CHECK_INT(INDICATOR_OVERLOAD, range(INT32_MAX));
}

int main(void)
{
    test_empty_scale();
    test_ranges();
    test_overload();
    return TEST_RESULT();
}
