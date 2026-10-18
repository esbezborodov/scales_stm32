# Hardware

A two-layer 50 × 45 mm PCB designed in Altium Designer 23 as a course project (2025) and
manufactured by NextPCB. The original documentation was drawn to GOST (ESKD) standards; the
drawings here are redrawn from the Altium project with English labels and without the drawing
frame.

## Block diagram

![Block diagram](block-diagram.svg)

- **Power.** +5 V from USB comes in through XS1 (VD1 is a Schottky protection diode) and feeds
  the display. An AMS1117-3.3 regulator (DD1) supplies the MCU and the ADC, HL1 shows power.
- **ADC.** HX711 (DA1) on the board. VT1 with the divider R5/R8 forms the HX711 analog supply
  regulator; its output AVDD also excites the load cell bridge connected to XS3.
- **MCU.** STM32F103C8T6 (DD2) with a reset button SW1, an SWD header XS2 and BOOT0 pulled low
  by R15.
- **Display** on XP1: I2C with 4.7 kΩ pull-ups to 3.3 V (R6, R7) and 100 Ω series resistors
  (R9, R10), powered from 5 V.
- **Range LEDs** HL2–HL5 with 270 Ω resistors, driven high by the MCU.

## Schematic

[![Schematic](schematic.svg)](schematic.svg)

Also available as a [PDF](schematic.pdf).

## Connectors

| Connector | Pin 1 | Pin 2 | Pin 3 | Pin 4 |
| --- | --- | --- | --- | --- |
| XS1 — power | +5 V | GND | | |
| XS2 — SWD | +3.3 V | SWDIO | SWCLK | GND |
| XS3 — load cell | AVDD (E+) | AGND (E−) | INA− (A−) | INA+ (A+) |
| XP1 — display | SCL | SDA | +5 V | GND |

## Known issues (rev. 1)

- **The crystal is on the wrong pins.** ZQ1 (8 MHz) is connected to PC14/PC15 (pins 3 and 4),
  which are the 32.768 kHz LSE oscillator pins, instead of OSC_IN/OSC_OUT (PD0/PD1, pins 5
  and 6). The external clock cannot be used, so the firmware runs this board from the internal
  HSI oscillator at 64 MHz (`BOARD_HAS_HSE 0` in
  [`firmware/boards/custom/board.h`](../firmware/boards/custom/board.h)); that is accurate
  enough for the HX711 and I2C timing. Rev. 2 should move ZQ1, C8 and C9 to pins 5 and 6.
- **LEDs are not placed in signal order.** Left to right the board has HL5 (PA6), HL3 (PA7),
  HL4 (PB14) and HL2 (PB15). The custom board header maps the ranges so that they still light
  up from left to right.
