#include <stdint.h>

#include "board.h"
#include "clock.h"
#include "config.h"
#include "delay.h"
#include "display.h"
#include "hx711.h"
#include "i2c.h"
#include "lcd1602.h"
#include "leds.h"
#include "scale.h"

static i2c_bus_t i2c;
static lcd1602_t lcd;
static hx711_t loadcell;
static scale_t scale;

static void tare(void)
{
    int32_t raw;
    /* The first conversions after power-up are not settled yet. */
    hx711_read_average(&loadcell, 4, &raw);
    if (hx711_read_average(&loadcell, SCALE_TARE_SAMPLES, &raw) == HX711_OK) {
        scale_set_tare(&scale, raw);
    }
}

int main(void)
{
    clock_init();
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
            leds_all_off();
        } else {
            const int32_t grams = scale_to_grams(&scale, raw);
            if (grams > SCALE_MAX_GRAMS) {
                display_overload();
            } else {
                display_weight(grams);
            }
            leds_show_weight((float)grams / 1000.0f);
#if SCALE_CALIBRATION_MODE
            display_counts(scale_net_counts(&scale, raw));
#endif
        }
        delay_ms(SCALE_UPDATE_PERIOD_MS);
    }
}
