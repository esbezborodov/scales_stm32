#ifndef I2C_H
#define I2C_H

/*
 * Blocking I2C master for the STM32F1 I2C peripheral (standard mode).
 *
 * Every wait is bounded by I2C_TIMEOUT_MS, so a missing or stuck device
 * returns an error instead of hanging the firmware.
 */

#include <stddef.h>
#include <stdint.h>

#include "gpio.h"
#include "stm32f1xx.h"

#define I2C_TIMEOUT_MS 10u

typedef enum {
    I2C_OK,
    I2C_ERR_NACK,    /* address or data byte was not acknowledged */
    I2C_ERR_TIMEOUT, /* a flag never came up, the bus may be stuck */
    I2C_ERR_BUS,     /* bus error or arbitration lost */
} i2c_status_t;

typedef struct {
    I2C_TypeDef *regs;
    gpio_pin_t scl;
    gpio_pin_t sda;
} i2c_bus_t;

/*
 * Set up I2C1 or I2C2 on the given pins. speed_hz must not exceed 100 kHz.
 * A slave left in the middle of a transfer (SDA held low) is released first.
 */
void i2c_init(i2c_bus_t *bus, I2C_TypeDef *regs, gpio_pin_t scl, gpio_pin_t sda, uint32_t speed_hz);

/* Write len bytes to the 7-bit address. len = 0 only checks that the device answers. */
i2c_status_t i2c_write(i2c_bus_t *bus, uint8_t addr, const uint8_t *data, size_t len);

#endif /* I2C_H */
