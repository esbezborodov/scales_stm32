#include <stdint.h>
#include <stdio.h>

#include "board.h"
#include "clock.h"
#include "delay.h"
#include "hx711.h"
#include "i2c.h"
#include "lcd1602.h"
#include "leds.h"
#include "number_parts.h"

/* Load cell sensitivity: raw ADC counts per kilogram. */
#define COUNTS_PER_KG 385070.0f
/* Samples averaged for tare and for every weight reading. */
#define TARE_SAMPLES   16u
#define WEIGHT_SAMPLES 4u

static i2c_bus_t i2c;
static lcd1602_t lcd;
static hx711_t loadcell;

int main(void)
{
    clock_init();
    delay_init();
    leds_init();
    i2c_init(&i2c, BOARD_LCD_I2C, BOARD_LCD_I2C_SCL, BOARD_LCD_I2C_SDA, 100000u);

    hx711_init(&loadcell, BOARD_HX711_SCK, BOARD_HX711_DOUT);
    /* Let the ADC settle after power-up, then take the empty scale as zero. */
    int32_t tare = 0;
    int32_t raw;
    hx711_read_average(&loadcell, 4, &raw);
    hx711_read_average(&loadcell, TARE_SAMPLES, &tare);

    lcd_init(&lcd, &i2c, BOARD_LCD_ADDR);
    lcd_print_line(&lcd, 0, "Scale:");

    float weight = 0.0f;
    char buffer[LCD_COLS + 1];

    while (1) {
        if (hx711_read_average(&loadcell, WEIGHT_SAMPLES, &raw) == HX711_OK) {
            weight = (float)(raw - tare) / COUNTS_PER_KG;
        }

        number_parts_t parts = {integer_part(weight), fractional_part(weight)};
        snprintf(buffer, sizeof(buffer), "%s%d.%02d kg",
                 (weight < 0.0f && parts.intg == 0) ? "-" : "", parts.intg, parts.frac);
        lcd_print_line(&lcd, 1, buffer);

        leds_show_weight(weight);
        delay_ms(250);
    }
}
