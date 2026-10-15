#include "scale.h"
#include "test.h"

static void test_tare(void)
{
    scale_t scale;
    scale_init(&scale, 1000);
    CHECK_INT(0, scale.tare);

    scale_set_tare(&scale, 410000);
    CHECK_INT(0, scale_net_counts(&scale, 410000));
    CHECK_INT(-10, scale_net_counts(&scale, 409990));
    CHECK_INT(0, scale_to_grams(&scale, 410000));
    CHECK_INT(1500, scale_to_grams(&scale, 411500));
}

static void test_real_calibration(void)
{
    /* Sensitivity of the load cell used in the project. */
    scale_t scale;
    scale_init(&scale, 385070);
    scale_set_tare(&scale, 410232);

    CHECK_INT(1000, scale_to_grams(&scale, 410232 + 385070));
    CHECK_INT(1150, scale_to_grams(&scale, 410232 + 442830));
    CHECK_INT(5000, scale_to_grams(&scale, 410232 + 5 * 385070));
    CHECK_INT(-1000, scale_to_grams(&scale, 410232 - 385070));
}

static void test_rounding(void)
{
    scale_t scale;
    scale_init(&scale, 2000); /* 2 counts per gram */

    CHECK_INT(0, scale_to_grams(&scale, 0));
    CHECK_INT(1, scale_to_grams(&scale, 1)); /* 0.5 g rounds away from zero */
    CHECK_INT(-1, scale_to_grams(&scale, -1));
    CHECK_INT(1, scale_to_grams(&scale, 2));
    CHECK_INT(2, scale_to_grams(&scale, 3));
    CHECK_INT(-2, scale_to_grams(&scale, -3));
}

static void test_negative_sensitivity(void)
{
    /* Load cell mounted the other way round: counts go down with weight. */
    scale_t scale;
    scale_init(&scale, -1000);
    CHECK_INT(250, scale_to_grams(&scale, -250));
    CHECK_INT(-250, scale_to_grams(&scale, 250));
}

static void test_limits(void)
{
    scale_t scale;
    scale_init(&scale, 0);
    CHECK_INT(0, scale_to_grams(&scale, 123456)); /* not calibrated: no division by zero */

    /* The full 24-bit swing times 1000 does not fit into 32 bits. */
    scale_init(&scale, 385070);
    scale_set_tare(&scale, -8388608);
    CHECK_INT(43569, scale_to_grams(&scale, 8388607));
}

int main(void)
{
    test_tare();
    test_real_calibration();
    test_rounding();
    test_negative_sensitivity();
    test_limits();
    return TEST_RESULT();
}
