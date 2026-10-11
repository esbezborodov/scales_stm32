#include "leds.h"

#include "board.h"
#include "gpio.h"

static const gpio_pin_t led_pins[LED_COUNT] = {
    [LED_RANGE1] = BOARD_LED_RANGE1,
    [LED_RANGE2] = BOARD_LED_RANGE2,
    [LED_RANGE3] = BOARD_LED_RANGE3,
    [LED_RANGE4] = BOARD_LED_RANGE4,
};

void leds_init(void)
{
    for (int i = 0; i < LED_COUNT; i++) {
        gpio_init(led_pins[i], GPIO_OUT_PP);
        gpio_write(led_pins[i], false);
    }
}

void leds_set(led_t led, bool on)
{
    if (led < LED_COUNT) {
        gpio_write(led_pins[led], on);
    }
}

void leds_all_off(void)
{
    for (int i = 0; i < LED_COUNT; i++) {
        gpio_write(led_pins[i], false);
    }
}

void leds_show_weight(float weight)
{
    leds_all_off();
    if ((0.0f < weight) && (weight <= 1.25f)) {
        leds_set(LED_RANGE1, true);
    } else if ((1.25f < weight) && (weight <= 2.50f)) {
        leds_set(LED_RANGE2, true);
    } else if ((2.50f < weight) && (weight <= 3.75f)) {
        leds_set(LED_RANGE3, true);
    } else if ((3.75f < weight) && (weight <= 5.00f)) {
        leds_set(LED_RANGE4, true);
    }
}
