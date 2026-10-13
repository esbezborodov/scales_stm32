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

/* Conversions averaged for the tare and for every reading (10 per second). */
#define SCALE_TARE_SAMPLES 16u
#define SCALE_READ_SAMPLES 4u

#define SCALE_UPDATE_PERIOD_MS 250u

#endif /* CONFIG_H */
