#include <stdint.h>
#include <stdio.h>

#include "board.h"
#include "clock.h"
#include "config.h"
#include "delay.h"
#include "hx711.h"
#include "i2c.h"
#include "lcd1602.h"
#include "leds.h"
#include "number_parts.h"
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
    lcd_print_line(&lcd, 0, "Scale:");
    lcd_print_line(&lcd, 1, "Taring...");

    hx711_init(&loadcell, BOARD_HX711_SCK, BOARD_HX711_DOUT);
    scale_init(&scale, SCALE_COUNTS_PER_KG);
    tare();

    char buffer[LCD_COLS + 1];

    while (1) {
        int32_t raw;
        if (hx711_read_average(&loadcell, SCALE_READ_SAMPLES, &raw) == HX711_OK) {
            const float weight = (float)scale_to_grams(&scale, raw) / 1000.0f;

            number_parts_t parts = {integer_part(weight), fractional_part(weight)};
            snprintf(buffer, sizeof(buffer), "%s%d.%02d kg",
                     (weight < 0.0f && parts.intg == 0) ? "-" : "", parts.intg, parts.frac);
            lcd_print_line(&lcd, 1, buffer);
            leds_show_weight(weight);

#if SCALE_CALIBRATION_MODE
            snprintf(buffer, sizeof(buffer), "raw %ld", (long)scale_net_counts(&scale, raw));
            lcd_print_line(&lcd, 0, buffer);
#endif
        }
        delay_ms(SCALE_UPDATE_PERIOD_MS);
    }
}
