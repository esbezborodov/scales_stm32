#include "delay.h"

#include "stm32f1xx.h"

static volatile uint32_t ms_ticks;

void SysTick_Handler(void)
{
    ms_ticks++;
}

void delay_init(void)
{
    SysTick_Config(SystemCoreClock / 1000u);

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint32_t delay_millis(void)
{
    return ms_ticks;
}

void delay_ms(uint32_t ms)
{
    const uint32_t start = ms_ticks;
    while ((ms_ticks - start) < ms) {
    }
}

void delay_us(uint32_t us)
{
    const uint32_t start = DWT->CYCCNT;
    const uint32_t cycles = us * (SystemCoreClock / 1000000u);
    while ((DWT->CYCCNT - start) < cycles) {
    }
}
