#include "leds.h"
#include "leds_module.h"

static void _init_led (led_t* leds, uint8_t num, GPIO_TypeDef* gpio, uint16_t pin);
static void _off_all_leds(led_t* leds); 

void leds_init(led_t* leds) {
    _init_led(leds, LED125, GPIOA, GPIO_PIN_7);
    _init_led(leds, LED250, GPIOA, GPIO_PIN_6);
    _init_led(leds, LED375, GPIOB, GPIO_PIN_14);
    _init_led(leds, LED500, GPIOB, GPIO_PIN_15);
}

void led_on(led_t* leds, leds_e led) {
    HAL_GPIO_WritePin(leds[led].led_gpio, leds[led].led_pin, GPIO_PIN_SET);
    leds[led].state = 1;
}

void led_off(led_t* leds, leds_e led) {
    HAL_GPIO_WritePin(leds[led].led_gpio, leds[led].led_pin, GPIO_PIN_RESET);
    leds[led].state = 0;
}

void leds_module_toggle(led_t* leds, float weight) {
    _off_all_leds(leds);
    if ((0.0f < weight) && (weight <= 1.25f)) led_on(leds, LED125);
    else if ((1.25f < weight) && (weight <= 2.50f)) led_on(leds, LED250);
    else if ((2.50f < weight) && (weight <= 3.75f)) led_on(leds, LED375);
    else if ((3.75f < weight) && (weight <= 5.00f)) led_on(leds, LED500);
}

static void _init_led (led_t* leds, uint8_t num, GPIO_TypeDef* gpio, uint16_t pin) {
    leds[num].led_gpio = gpio;
    leds[num].led_pin = pin;
    leds[num].state = false;
}

static void _off_all_leds(led_t* leds) {
    for (int i = 0; i < NUM_LEDS; i++) {
        HAL_GPIO_WritePin(leds[i].led_gpio, leds[i].led_pin, GPIO_PIN_RESET);
        leds[i].state = 0;
    }
}
