#ifndef CLOCK_H
#define CLOCK_H

/*
 * System clock setup.
 *
 * With a crystal: SYSCLK = HSE 8 MHz * 9 = 72 MHz, AHB = 72 MHz, APB1 = 36 MHz,
 * APB2 = 72 MHz. Without one, or if the crystal does not start, the clock runs
 * from HSI / 2 * 16 = 64 MHz so the board keeps working.
 */

#include <stdbool.h>

typedef enum {
    CLOCK_SOURCE_HSE,
    CLOCK_SOURCE_HSI,
} clock_source_t;

/*
 * Configure flash wait states, PLL and bus prescalers and update SystemCoreClock.
 * use_hse = false skips the crystal start-up wait on boards without HSE.
 */
clock_source_t clock_init(bool use_hse);

#endif /* CLOCK_H */
