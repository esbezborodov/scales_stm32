#include "clock.h"

#include <stdbool.h>
#include <stdint.h>

#include "stm32f1xx.h"

/* Polling iterations to wait for HSERDY (several ms at 8 MHz HSI). */
#define HSE_READY_TIMEOUT 0x10000u

static bool hse_start(void)
{
    RCC->CR |= RCC_CR_HSEON;
    for (uint32_t i = 0; i < HSE_READY_TIMEOUT; i++) {
        if (RCC->CR & RCC_CR_HSERDY) {
            return true;
        }
    }
    RCC->CR &= ~RCC_CR_HSEON;
    return false;
}

clock_source_t clock_init(void)
{
    /* Two wait states are required for 48 MHz < SYSCLK <= 72 MHz. */
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_1;

    const bool hse_ok = hse_start();

    uint32_t cfgr = RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_PPRE2_DIV1;
    if (hse_ok) {
        cfgr |= RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9; /* 8 MHz * 9 = 72 MHz */
    } else {
        cfgr |= RCC_CFGR_PLLMULL16; /* HSI / 2 = 4 MHz * 16 = 64 MHz */
    }
    RCC->CFGR = cfgr; /* SYSCLK is still HSI here */

    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY)) {
    }

    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {
    }

    SystemCoreClockUpdate();
    return hse_ok ? CLOCK_SOURCE_HSE : CLOCK_SOURCE_HSI;
}
