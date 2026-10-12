#ifndef LCD1602_H
#define LCD1602_H

/*
 * HD44780-compatible 16x2 character LCD behind a PCF8574 I2C expander
 * (the common "I2C backpack"). The controller runs in 4-bit mode.
 *
 * PCF8574 wiring: P0 = RS, P1 = RW, P2 = EN, P3 = backlight, P4..P7 = D4..D7.
 */

#include <stdbool.h>
#include <stdint.h>

#include "i2c.h"

#define LCD_COLS 16u
#define LCD_ROWS 2u

typedef struct {
    i2c_bus_t *bus;
    uint8_t addr;
    uint8_t backlight; /* P3 bit, kept in every byte written to the expander */
} lcd1602_t;

/* Run the 4-bit initialization sequence and clear the screen. */
i2c_status_t lcd_init(lcd1602_t *lcd, i2c_bus_t *bus, uint8_t addr);

i2c_status_t lcd_clear(lcd1602_t *lcd);
i2c_status_t lcd_set_cursor(lcd1602_t *lcd, uint8_t col, uint8_t row);
i2c_status_t lcd_backlight(lcd1602_t *lcd, bool on);

/* Print a string at the cursor position. */
i2c_status_t lcd_print(lcd1602_t *lcd, const char *text);

/* Replace a whole row: the text is padded with spaces to the full width. */
i2c_status_t lcd_print_line(lcd1602_t *lcd, uint8_t row, const char *text);

#endif /* LCD1602_H */
