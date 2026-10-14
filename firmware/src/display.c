#include "display.h"

#include "weight_format.h"

#define TITLE_ROW 0u
#define VALUE_ROW 1u

static lcd1602_t *display_lcd;

void display_init(lcd1602_t *lcd)
{
    display_lcd = lcd;
    lcd_print_line(display_lcd, TITLE_ROW, "Scale:");
}

void display_taring(void)
{
    lcd_print_line(display_lcd, VALUE_ROW, "Taring...");
}

void display_weight(int32_t grams)
{
    char text[WEIGHT_TEXT_SIZE];
    format_weight(grams, text, sizeof(text));
    lcd_print_line(display_lcd, VALUE_ROW, text);
}

void display_overload(void)
{
    lcd_print_line(display_lcd, VALUE_ROW, "Overload");
}

void display_sensor_error(void)
{
    lcd_print_line(display_lcd, VALUE_ROW, "Sensor error");
}

void display_counts(int32_t counts)
{
    char text[LCD_COLS + 1u] = "raw ";
    format_int(counts, &text[4], sizeof(text) - 4u);
    lcd_print_line(display_lcd, TITLE_ROW, text);
}
