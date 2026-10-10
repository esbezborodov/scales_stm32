#ifndef CLOCK_H
#define CLOCK_H

/*
 * System clock setup.
 *
 * SYSCLK = HSE 8 MHz * 9 = 72 MHz, AHB = 72 MHz, APB1 = 36 MHz, APB2 = 72 MHz.
 * If the crystal does not start, the clock falls back to HSI / 2 * 16 = 64 MHz
 * so the board keeps working (slower, less accurate timing).
 */

typedef enum {
    CLOCK_SOURCE_HSE,
    CLOCK_SOURCE_HSI,
} clock_source_t;

/* Configure flash wait states, PLL and bus prescalers. Updates SystemCoreClock. */
clock_source_t clock_init(void);

#endif /* CLOCK_H */
