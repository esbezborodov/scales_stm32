#ifndef LEDS_H
#define LEDS_H

#include "stdint.h"
#include "stdbool.h"
#include "stm32f1xx_hal.h"

typedef struct {
    GPIO_TypeDef *led_gpio;
    uint16_t led_pin;
    bool state;
} led_t;

typedef enum {
    LED125 = 0,
    LED250, 
    LED375,
    LED500,
    NUM_LEDS // last element
} leds_e;

void leds_init(led_t* leds);
void led_on(led_t* leds, leds_e led); 
void led_off(led_t* leds, leds_e led);

#endif /* LEDS_H */