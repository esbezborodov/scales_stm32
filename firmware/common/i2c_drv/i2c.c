#include "i2c.h"

#include <stdbool.h>

#include "delay.h"

#define RECOVERY_CLOCKS 9u

static uint32_t pclk1_hz(void)
{
    const uint32_t ppre1 = (RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos;
    return SystemCoreClock >> APBPrescTable[ppre1];
}

/* Clock SCL until the slave lets SDA go, then issue a STOP condition. */
static void bus_recover(const i2c_bus_t *bus)
{
    gpio_write(bus->scl, true);
    gpio_write(bus->sda, true);
    gpio_init(bus->scl, GPIO_OUT_OD);
    gpio_init(bus->sda, GPIO_OUT_OD);
    delay_us(5);

    for (uint32_t i = 0; (i < RECOVERY_CLOCKS) && !gpio_read(bus->sda); i++) {
        gpio_write(bus->scl, false);
        delay_us(5);
        gpio_write(bus->scl, true);
        delay_us(5);
    }

    /* STOP: SDA goes high while SCL is high. */
    gpio_write(bus->scl, false);
    delay_us(5);
    gpio_write(bus->sda, false);
    delay_us(5);
    gpio_write(bus->scl, true);
    delay_us(5);
    gpio_write(bus->sda, true);
    delay_us(5);
}

void i2c_init(i2c_bus_t *bus, I2C_TypeDef *regs, gpio_pin_t scl, gpio_pin_t sda, uint32_t speed_hz)
{
    bus->regs = regs;
    bus->scl = scl;
    bus->sda = sda;

    bus_recover(bus);
    gpio_init(scl, GPIO_AF_OD);
    gpio_init(sda, GPIO_AF_OD);

    const uint32_t enable = (regs == I2C1) ? RCC_APB1ENR_I2C1EN : RCC_APB1ENR_I2C2EN;
    const uint32_t reset = (regs == I2C1) ? RCC_APB1RSTR_I2C1RST : RCC_APB1RSTR_I2C2RST;
    RCC->APB1ENR |= enable;
    RCC->APB1RSTR |= reset;
    RCC->APB1RSTR &= ~reset;

    const uint32_t pclk = pclk1_hz();
    uint32_t ccr = pclk / (2u * speed_hz); /* standard mode: Thigh = Tlow = CCR * Tpclk */
    if (ccr < 4u) {
        ccr = 4u;
    }

    regs->CR1 = 0;
    regs->CR2 = pclk / 1000000u; /* peripheral clock in MHz */
    regs->CCR = ccr;
    regs->TRISE = pclk / 1000000u + 1u; /* 1000 ns maximum rise time */
    regs->CR1 = I2C_CR1_PE;
}

static i2c_status_t stop(I2C_TypeDef *regs, i2c_status_t status)
{
    regs->CR1 |= I2C_CR1_STOP;
    return status;
}

/* Wait for an SR1 flag while watching for NACK, bus errors and the timeout. */
static i2c_status_t wait_sr1(I2C_TypeDef *regs, uint32_t flag, uint32_t start)
{
    while (!(regs->SR1 & flag)) {
        const uint32_t sr1 = regs->SR1;
        if (sr1 & I2C_SR1_AF) {
            regs->SR1 = ~I2C_SR1_AF;
            return stop(regs, I2C_ERR_NACK);
        }
        if (sr1 & (I2C_SR1_BERR | I2C_SR1_ARLO)) {
            regs->SR1 = ~(I2C_SR1_BERR | I2C_SR1_ARLO);
            return stop(regs, I2C_ERR_BUS);
        }
        if ((delay_millis() - start) > I2C_TIMEOUT_MS) {
            return stop(regs, I2C_ERR_TIMEOUT);
        }
    }
    return I2C_OK;
}

i2c_status_t i2c_write(i2c_bus_t *bus, uint8_t addr, const uint8_t *data, size_t len)
{
    I2C_TypeDef *regs = bus->regs;
    const uint32_t start = delay_millis();
    i2c_status_t status;

    while (regs->SR2 & I2C_SR2_BUSY) {
        if ((delay_millis() - start) > I2C_TIMEOUT_MS) {
            return I2C_ERR_TIMEOUT;
        }
    }

    regs->CR1 |= I2C_CR1_START;
    if ((status = wait_sr1(regs, I2C_SR1_SB, start)) != I2C_OK) {
        return status;
    }

    regs->DR = (uint32_t)addr << 1; /* R/W bit = 0: write */
    if ((status = wait_sr1(regs, I2C_SR1_ADDR, start)) != I2C_OK) {
        return status;
    }
    (void)regs->SR2; /* reading SR1 then SR2 clears ADDR */

    for (size_t i = 0; i < len; i++) {
        if ((status = wait_sr1(regs, I2C_SR1_TXE, start)) != I2C_OK) {
            return status;
        }
        regs->DR = data[i];
    }

    if (len > 0u && (status = wait_sr1(regs, I2C_SR1_BTF, start)) != I2C_OK) {
        return status;
    }
    return stop(regs, I2C_OK);
}
