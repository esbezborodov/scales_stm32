#include "leds.h"
#include "stdint.h"

static void _init_led (led_t* leds, uint8_t num, GPIO_TypeDef* gpio, uint16_t pin);
static void _on_all_leds(led_t* leds);
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

void leds_module_toogle(led_t* leds, float data) {
    _off_all_leds(leds);
    if (0.0 < data < 1.25) led_on(leds, LED125);
    if (1.26 < data < 2.50) led_on(leds, LED250);
    if (2.51 < data < 3.75) led_on(leds, LED375);
    if (3.76 < data < 5.00) led_on(leds, LED500);
}

static void _init_led (led_t* leds, uint8_t num, GPIO_TypeDef* gpio, uint16_t pin) {
    leds[num].led_gpio = gpio;
    leds[num].led_pin = pin;
    leds[num].state = false;
}

static void _off_all_leds(led_t* leds) {
    int i = 0;
    while (i < NUM_LEDS) {
    HAL_GPIO_WritePin(leds[i].led_gpio, leds[i].led_pin, GPIO_PIN_RESET);
    leds[i].state = 0;
    }
}

static void _on_all_leds(led_t* leds) {
    int i = 0;
    while (i < NUM_LEDS) {
    HAL_GPIO_WritePin(leds[i].led_gpio, leds[i].led_pin, GPIO_PIN_SET);
    leds[i].state = 1;
    }
}