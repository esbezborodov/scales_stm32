#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include "clock.h"
#include "delay.h"
#include "lcd_i2c.h"
#include "number_parts.h"
#include "leds.h"
#include "board.h"
#include "hx711.h"
#include "i2c.h"

i2c_bus_t i2c;
LCD_I2C_t lcd;
number_parts_t data;
hx711_t loadcell;

/* Load cell sensitivity: raw ADC counts per kilogram. */
#define COUNTS_PER_KG 385070.0f
/* Samples averaged for tare and for every weight reading. */
#define TARE_SAMPLES   16u
#define WEIGHT_SAMPLES 4u

float weight = 0.0;
char buffer[20];

int main(void)
{
  clock_init();
  delay_init();
  leds_init();
  i2c_init(&i2c, BOARD_LCD_I2C, BOARD_LCD_I2C_SCL, BOARD_LCD_I2C_SDA, 100000u);

  hx711_init(&loadcell, BOARD_HX711_SCK, BOARD_HX711_DOUT);
  /* Let the ADC settle after power-up, then take the empty scale as zero. */
  int32_t tare = 0;
  int32_t raw;
  hx711_read_average(&loadcell, 4, &raw);
  hx711_read_average(&loadcell, TARE_SAMPLES, &tare);

  LCD_Init(&lcd, &i2c, BOARD_LCD_ADDR, 16, 2);
  LCD_Backlight(&lcd, 1);
  LCD_Clear(&lcd);
  LCD_SendString(&lcd, "Scale:");

  while (1)
  {
    LCD_SetCursor(&lcd, 0, 1);
    if (hx711_read_average(&loadcell, WEIGHT_SAMPLES, &raw) == HX711_OK) {
      weight = (float)(raw - tare) / COUNTS_PER_KG;
    }
    data.intg = integer_part(weight);
    data.frac = fractional_part(weight);

    /* Trailing spaces wipe leftovers of a longer previous value. */
    snprintf(buffer, sizeof(buffer), "%s%d.%02d kg   ",
             (weight < 0.0f && data.intg == 0) ? "-" : "", data.intg, data.frac);
    LCD_SendString(&lcd, buffer);

    leds_show_weight(weight);
    delay_ms(250);

  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
    LCD_SendString(&lcd, "Error");
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */
