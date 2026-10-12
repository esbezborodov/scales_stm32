#ifndef BOARD_H
#define BOARD_H

/*
 * Pin assignment. The custom board uses the same pinout as a Blue Pill
 * (STM32F103C8T6 with an 8 MHz crystal).
 */

#include "gpio.h"

/* Weight range LEDs, from the lightest range to the heaviest. */
#define BOARD_LED_RANGE1 ((gpio_pin_t){GPIOA, 7})
#define BOARD_LED_RANGE2 ((gpio_pin_t){GPIOA, 6})
#define BOARD_LED_RANGE3 ((gpio_pin_t){GPIOB, 14})
#define BOARD_LED_RANGE4 ((gpio_pin_t){GPIOB, 15})

/* HX711 load cell ADC. */
#define BOARD_HX711_SCK  ((gpio_pin_t){GPIOB, 12})
#define BOARD_HX711_DOUT ((gpio_pin_t){GPIOB, 13})

/* LCD1602 with a PCF8574 I2C backpack. */
#define BOARD_LCD_I2C     I2C2
#define BOARD_LCD_I2C_SCL ((gpio_pin_t){GPIOB, 10})
#define BOARD_LCD_I2C_SDA ((gpio_pin_t){GPIOB, 11})
#define BOARD_LCD_ADDR    0x27u

#endif /* BOARD_H */
