#ifndef CONFIG_H
#define CONFIG_H

/* Application settings. The pin assignment lives in board.h. */

/*
 * Load cell sensitivity: raw HX711 counts per kilogram (channel A, gain 128).
 * To recalibrate, set SCALE_CALIBRATION_MODE to 1, power the scale up empty,
 * put a known mass on it and divide the shown count by the mass in kg.
 */
#define SCALE_COUNTS_PER_KG 385070

/* 1: show raw counts (minus tare) on the first LCD row instead of the title. */
#define SCALE_CALIBRATION_MODE 0

/*
 * LED ranges: upper bound of every range in grams, one per LED. Above the last
 * bound the scale shows "Overload" and all LEDs blink.
 */
#define INDICATOR_RANGE1_MAX_GRAMS 1250
#define INDICATOR_RANGE2_MAX_GRAMS 2500
#define INDICATOR_RANGE3_MAX_GRAMS 3750
#define INDICATOR_RANGE4_MAX_GRAMS 5000

/* Readings up to this weight count as an empty scale (no LED is lit). */
#define INDICATOR_ZERO_BAND_GRAMS 10

/* Conversions averaged for the tare and for every reading (10 per second). */
#define SCALE_TARE_SAMPLES 16u
#define SCALE_READ_SAMPLES 4u

#define SCALE_UPDATE_PERIOD_MS 250u

#endif /* CONFIG_H */
