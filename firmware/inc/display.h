#ifndef DISPLAY_H
#define DISPLAY_H

/* What the scale shows on the LCD. */

#include <stdint.h>

#include "lcd1602.h"

void display_init(lcd1602_t *lcd);

/* "Taring..." while the empty scale is being measured. */
void display_taring(void);

void display_weight(int32_t grams);

/* The load is above the measuring range. */
void display_overload(void);

/* The HX711 does not answer. */
void display_sensor_error(void);

/* Calibration mode: raw counts minus tare on the first row. */
void display_counts(int32_t counts);

#endif /* DISPLAY_H */
