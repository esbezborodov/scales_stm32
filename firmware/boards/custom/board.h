#ifndef BOARD_H
#define BOARD_H

/*
 * Pin assignment of the custom PCB (see hardware/). Same signals as the
 * Blue Pill prototype, but the range LEDs are routed in a different order.
 */

#include "gpio.h"

/*
 * Rev. 1 of the PCB routes the 8 MHz crystal ZQ1 to PC14/PC15 (the 32 kHz LSE
 * pins) instead of OSC_IN/OSC_OUT, so there is no usable HSE: run from HSI.
 */
#define BOARD_HAS_HSE 0

/*
 * Weight range LEDs, from the lightest range to the heaviest. On the PCB
 * they sit left to right as HL5 (PA6), HL3 (PA7), HL4 (PB14), HL2 (PB15).
 */
#define BOARD_LED_RANGE1 ((gpio_pin_t){GPIOA, 6})
#define BOARD_LED_RANGE2 ((gpio_pin_t){GPIOA, 7})
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
