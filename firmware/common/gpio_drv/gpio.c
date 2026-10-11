#include "gpio.h"

/* CNF[1:0] and MODE[1:0] bits of one pin in CRL/CRH. */
#define MODE_INPUT     0x0u
#define MODE_OUT_10MHZ 0x1u
#define CNF_IN_FLOAT   0x1u
#define CNF_IN_PULL    0x2u
#define CNF_OUT_PP     0x0u
#define CNF_OUT_OD     0x1u
#define CNF_AF_PP      0x2u
#define CNF_AF_OD      0x3u

static void enable_port_clock(const GPIO_TypeDef *port)
{
    if (port == GPIOA) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    } else if (port == GPIOB) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    } else if (port == GPIOC) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    } else if (port == GPIOD) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
    }
    (void)RCC->APB2ENR; /* make sure the clock is running before the port is touched */
}

static uint32_t mode_bits(gpio_mode_t mode)
{
    switch (mode) {
    case GPIO_OUT_PP:
        return (CNF_OUT_PP << 2) | MODE_OUT_10MHZ;
    case GPIO_OUT_OD:
        return (CNF_OUT_OD << 2) | MODE_OUT_10MHZ;
    case GPIO_AF_PP:
        return (CNF_AF_PP << 2) | MODE_OUT_10MHZ;
    case GPIO_AF_OD:
        return (CNF_AF_OD << 2) | MODE_OUT_10MHZ;
    case GPIO_IN_PULLUP:
    case GPIO_IN_PULLDOWN:
        return (CNF_IN_PULL << 2) | MODE_INPUT;
    case GPIO_IN_FLOAT:
    default:
        return (CNF_IN_FLOAT << 2) | MODE_INPUT;
    }
}

void gpio_init(gpio_pin_t pin, gpio_mode_t mode)
{
    enable_port_clock(pin.port);

    /* Pins 0..7 live in CRL, pins 8..15 in CRH, four bits per pin. */
    volatile uint32_t *cr = (pin.pin < 8u) ? &pin.port->CRL : &pin.port->CRH;
    const uint32_t shift = (pin.pin % 8u) * 4u;
    *cr = (*cr & ~(0xFu << shift)) | (mode_bits(mode) << shift);

    /* For inputs with a pull resistor ODR selects pull-up (1) or pull-down (0). */
    if (mode == GPIO_IN_PULLUP) {
        gpio_write(pin, true);
    } else if (mode == GPIO_IN_PULLDOWN) {
        gpio_write(pin, false);
    }
}
