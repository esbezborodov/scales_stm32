#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>
#include <stdint.h>

#include "stm32f1xx.h"

/* A single pin: port + pin number (0..15). */
typedef struct {
    GPIO_TypeDef *port;
    uint8_t pin;
} gpio_pin_t;

typedef enum {
    GPIO_OUT_PP,      /* general purpose output, push-pull */
    GPIO_OUT_OD,      /* general purpose output, open drain */
    GPIO_AF_PP,       /* alternate function, push-pull */
    GPIO_AF_OD,       /* alternate function, open drain */
    GPIO_IN_FLOAT,    /* input without pull resistor */
    GPIO_IN_PULLUP,   /* input with internal pull-up */
    GPIO_IN_PULLDOWN, /* input with internal pull-down */
} gpio_mode_t;

/* Enable the port clock and configure the pin. Outputs run at 10 MHz max. */
void gpio_init(gpio_pin_t pin, gpio_mode_t mode);

static inline void gpio_write(gpio_pin_t pin, bool high)
{
    /* BSRR: the low half sets bits, the high half resets them. */
    pin.port->BSRR = high ? (1u << pin.pin) : (1u << (pin.pin + 16u));
}

static inline bool gpio_read(gpio_pin_t pin)
{
    return (pin.port->IDR & (1u << pin.pin)) != 0u;
}

static inline void gpio_toggle(gpio_pin_t pin)
{
    gpio_write(pin, (pin.port->ODR & (1u << pin.pin)) == 0u);
}

#endif /* GPIO_H */
