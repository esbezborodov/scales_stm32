#include "hx711.h"

#include <stdbool.h>

#include "delay.h"

/* 24 data bits + 1 extra pulse that selects channel A, gain 128. */
#define DATA_BITS   24u
#define GAIN_PULSES 1u

static void clock_pulse(const hx711_t *dev)
{
    gpio_write(dev->sck, true);
    delay_us(1);
    gpio_write(dev->sck, false);
    delay_us(1);
}

static bool wait_ready(const hx711_t *dev)
{
    const uint32_t start = delay_millis();
    while (gpio_read(dev->dout)) {
        if ((delay_millis() - start) > HX711_READY_TIMEOUT_MS) {
            return false;
        }
    }
    return true;
}

void hx711_init(hx711_t *dev, gpio_pin_t sck, gpio_pin_t dout)
{
    dev->sck = sck;
    dev->dout = dout;

    gpio_init(dev->sck, GPIO_OUT_PP);
    gpio_write(dev->sck, false);
    /* With a pull-up a disconnected chip reads as "not ready" and times out. */
    gpio_init(dev->dout, GPIO_IN_PULLUP);
}

hx711_status_t hx711_read(const hx711_t *dev, int32_t *raw)
{
    if (!wait_ready(dev)) {
        return HX711_TIMEOUT;
    }

    /* An interrupt stretching SCK high beyond 60 us would power the chip down. */
    const uint32_t primask = __get_PRIMASK();
    __disable_irq();

    uint32_t value = 0;
    for (uint32_t i = 0; i < DATA_BITS; i++) {
        gpio_write(dev->sck, true);
        delay_us(1);
        value = (value << 1) | (gpio_read(dev->dout) ? 1u : 0u);
        gpio_write(dev->sck, false);
        delay_us(1);
    }
    for (uint32_t i = 0; i < GAIN_PULSES; i++) {
        clock_pulse(dev);
    }

    __set_PRIMASK(primask);

    /* The result is 24-bit two's complement: sign-extend it to 32 bits. */
    if (value & 0x800000u) {
        value |= 0xFF000000u;
    }
    *raw = (int32_t)value;
    return HX711_OK;
}

hx711_status_t hx711_read_average(const hx711_t *dev, uint8_t samples, int32_t *raw)
{
    if (samples == 0u) {
        samples = 1u;
    }

    int64_t sum = 0;
    for (uint8_t i = 0; i < samples; i++) {
        int32_t value;
        const hx711_status_t status = hx711_read(dev, &value);
        if (status != HX711_OK) {
            return status;
        }
        sum += value;
    }
    *raw = (int32_t)(sum / samples);
    return HX711_OK;
}

void hx711_power_down(const hx711_t *dev)
{
    gpio_write(dev->sck, false);
    gpio_write(dev->sck, true);
    delay_us(100);
}

void hx711_power_up(const hx711_t *dev)
{
    gpio_write(dev->sck, false);
}
