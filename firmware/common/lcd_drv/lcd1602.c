#include "lcd1602.h"

#include "delay.h"

/* PCF8574 pins. */
#define PIN_RS        0x01u
#define PIN_EN        0x04u
#define PIN_BACKLIGHT 0x08u

/* HD44780 commands. */
#define CMD_CLEAR          0x01u
#define CMD_ENTRY_MODE     0x04u
#define CMD_DISPLAY        0x08u
#define CMD_FUNCTION_SET   0x20u
#define CMD_SET_DDRAM_ADDR 0x80u

#define ENTRY_INCREMENT 0x02u
#define DISPLAY_ON      0x04u
#define FUNCTION_2LINES 0x08u

/* Clear and return home take up to 1.52 ms, other commands are done within one I2C write. */
#define CLEAR_DELAY_MS 2u

static const uint8_t row_offsets[LCD_ROWS] = {0x00u, 0x40u};

/* Latch one nibble (upper four bits of `nibble`): EN high, then EN low. */
static i2c_status_t write_nibble(lcd1602_t *lcd, uint8_t nibble, uint8_t flags)
{
    const uint8_t bits = (nibble & 0xF0u) | flags | lcd->backlight;
    const uint8_t frame[2] = {bits | PIN_EN, bits};
    return i2c_write(lcd->bus, lcd->addr, frame, sizeof(frame));
}

static i2c_status_t write_byte(lcd1602_t *lcd, uint8_t value, uint8_t flags)
{
    const uint8_t high = (value & 0xF0u) | flags | lcd->backlight;
    const uint8_t low = (uint8_t)(value << 4) | flags | lcd->backlight;
    const uint8_t frame[4] = {high | PIN_EN, high, low | PIN_EN, low};
    return i2c_write(lcd->bus, lcd->addr, frame, sizeof(frame));
}

static i2c_status_t command(lcd1602_t *lcd, uint8_t cmd)
{
    return write_byte(lcd, cmd, 0u);
}

i2c_status_t lcd_init(lcd1602_t *lcd, i2c_bus_t *bus, uint8_t addr)
{
    lcd->bus = bus;
    lcd->addr = addr;
    lcd->backlight = PIN_BACKLIGHT;

    /* Power-on wait, then the "8-bit mode" reset sequence from the HD44780 datasheet. */
    delay_ms(50);
    i2c_status_t status = write_nibble(lcd, 0x30u, 0u);
    if (status != I2C_OK) {
        return status;
    }
    delay_ms(5);
    write_nibble(lcd, 0x30u, 0u);
    delay_ms(1);
    write_nibble(lcd, 0x30u, 0u);
    delay_ms(1);
    write_nibble(lcd, 0x20u, 0u); /* switch to 4-bit mode */
    delay_ms(1);

    command(lcd, CMD_FUNCTION_SET | FUNCTION_2LINES);
    command(lcd, CMD_DISPLAY); /* display off */
    lcd_clear(lcd);
    command(lcd, CMD_ENTRY_MODE | ENTRY_INCREMENT);
    return command(lcd, CMD_DISPLAY | DISPLAY_ON);
}

i2c_status_t lcd_clear(lcd1602_t *lcd)
{
    const i2c_status_t status = command(lcd, CMD_CLEAR);
    delay_ms(CLEAR_DELAY_MS);
    return status;
}

i2c_status_t lcd_set_cursor(lcd1602_t *lcd, uint8_t col, uint8_t row)
{
    if (row >= LCD_ROWS) {
        row = LCD_ROWS - 1u;
    }
    if (col >= LCD_COLS) {
        col = LCD_COLS - 1u;
    }
    return command(lcd, CMD_SET_DDRAM_ADDR | (uint8_t)(row_offsets[row] + col));
}

i2c_status_t lcd_backlight(lcd1602_t *lcd, bool on)
{
    lcd->backlight = on ? PIN_BACKLIGHT : 0u;
    const uint8_t bits = lcd->backlight;
    return i2c_write(lcd->bus, lcd->addr, &bits, 1u);
}

i2c_status_t lcd_print(lcd1602_t *lcd, const char *text)
{
    i2c_status_t status = I2C_OK;
    while (*text != '\0' && status == I2C_OK) {
        status = write_byte(lcd, (uint8_t)*text++, PIN_RS);
    }
    return status;
}

i2c_status_t lcd_print_line(lcd1602_t *lcd, uint8_t row, const char *text)
{
    i2c_status_t status = lcd_set_cursor(lcd, 0u, row);
    for (uint8_t col = 0; col < LCD_COLS && status == I2C_OK; col++) {
        const char c = (*text != '\0') ? *text++ : ' ';
        status = write_byte(lcd, (uint8_t)c, PIN_RS);
    }
    return status;
}
