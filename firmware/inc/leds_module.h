#ifndef LEDS_MODULE_H
#define LEDS_MODULE_H

#include "leds.h"

/* Light the LED whose range contains the weight (kg). */
void leds_module_toggle(led_t *leds, float weight);

#endif /* LEDS_MODULE_H */
