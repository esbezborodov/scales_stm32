#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

/*
 * Time base and blocking delays.
 *
 * SysTick generates a 1 ms tick, the DWT cycle counter is used for
 * microsecond delays. Call delay_init() after the system clock is set up.
 */

void delay_init(void);

/* Milliseconds since delay_init(). Wraps around after ~49 days. */
uint32_t delay_millis(void);

void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

#endif /* DELAY_H */
