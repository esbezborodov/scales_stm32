#ifndef HX711_H
#define HX711_H

/*
 * HX711 24-bit ADC for load cells (bit-banged two-wire interface).
 *
 * Only channel A with gain 128 is used: every conversion is read with
 * 25 clock pulses. At RATE = 0 the chip produces 10 samples per second.
 */

#include <stdint.h>

#include "gpio.h"

/* How long to wait for a conversion (the first one after power-up takes ~400 ms). */
#define HX711_READY_TIMEOUT_MS 500u

typedef struct {
    gpio_pin_t sck;
    gpio_pin_t dout;
} hx711_t;

typedef enum {
    HX711_OK,
    HX711_TIMEOUT, /* DOUT never went low: chip missing or powered down */
} hx711_status_t;

void hx711_init(hx711_t *dev, gpio_pin_t sck, gpio_pin_t dout);

/* Read one conversion as a signed value (-8388608..8388607). */
hx711_status_t hx711_read(const hx711_t *dev, int32_t *raw);

/* Read `samples` conversions and return their mean. */
hx711_status_t hx711_read_average(const hx711_t *dev, uint8_t samples, int32_t *raw);

/* SCK held high for more than 60 us puts the chip into power-down mode. */
void hx711_power_down(const hx711_t *dev);
void hx711_power_up(const hx711_t *dev);

#endif /* HX711_H */
