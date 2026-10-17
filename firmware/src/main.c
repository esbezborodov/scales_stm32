#include <stdbool.h>
#include <stdint.h>

#include "board.h"
#include "clock.h"
#include "config.h"
#include "delay.h"
#include "display.h"
#include "hx711.h"
#include "i2c.h"
#include "indicator.h"
#include "lcd1602.h"
#include "leds.h"
#include "scale.h"

static i2c_bus_t i2c;
static lcd1602_t lcd;
static hx711_t loadcell;
static scale_t scale;

static const int32_t led_ranges[LED_COUNT] = {
    [LED_RANGE1] = INDICATOR_RANGE1_MAX_GRAMS,
    [LED_RANGE2] = INDICATOR_RANGE2_MAX_GRAMS,
    [LED_RANGE3] = INDICATOR_RANGE3_MAX_GRAMS,
    [LED_RANGE4] = INDICATOR_RANGE4_MAX_GRAMS,
};

static void tare(void)
{
    int32_t raw;
    /* The first conversions after power-up are not settled yet. */
    hx711_read_average(&loadcell, 4, &raw);
    if (hx711_read_average(&loadcell, SCALE_TARE_SAMPLES, &raw) == HX711_OK) {
        scale_set_tare(&scale, raw);
    }
}

/* One LED per weight range; all LEDs blink on overload. */
static void show_range(int range)
{
    static bool blink_on;

    if (range == INDICATOR_OVERLOAD) {
        blink_on = !blink_on;
        leds_set_all(blink_on);
        return;
    }
    leds_set_all(false);
    if (range != INDICATOR_NONE) {
        leds_set((led_t)range, true);
    }
}

int main(void)
{
    clock_init(BOARD_HAS_HSE);
    delay_init();
    leds_init();
    i2c_init(&i2c, BOARD_LCD_I2C, BOARD_LCD_I2C_SCL, BOARD_LCD_I2C_SDA, 100000u);
    lcd_init(&lcd, &i2c, BOARD_LCD_ADDR);
    display_init(&lcd);
    display_taring();

    hx711_init(&loadcell, BOARD_HX711_SCK, BOARD_HX711_DOUT);
    scale_init(&scale, SCALE_COUNTS_PER_KG);
    tare();

    while (1) {
        int32_t raw;
        if (hx711_read_average(&loadcell, SCALE_READ_SAMPLES, &raw) != HX711_OK) {
            display_sensor_error();
            leds_set_all(false);
        } else {
            const int32_t grams = scale_to_grams(&scale, raw);
            const int range =
                indicator_range(grams, led_ranges, LED_COUNT, INDICATOR_ZERO_BAND_GRAMS);
            if (range == INDICATOR_OVERLOAD) {
                display_overload();
            } else {
                display_weight(grams);
            }
            show_range(range);
#if SCALE_CALIBRATION_MODE
            display_counts(scale_net_counts(&scale, raw));
#endif
        }
        delay_ms(SCALE_UPDATE_PERIOD_MS);
    }
}
