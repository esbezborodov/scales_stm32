#ifndef LEDS_H
#define LEDS_H

#include <stdbool.h>

/* Weight range LEDs. */
typedef enum { LED_RANGE1 = 0, LED_RANGE2, LED_RANGE3, LED_RANGE4, LED_COUNT } led_t;

void leds_init(void);
void leds_set(led_t led, bool on);
void leds_set_all(bool on);

#endif /* LEDS_H */
