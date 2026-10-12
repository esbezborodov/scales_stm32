/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32f1xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "main.h"
#include "delay.h"

/* The HAL time base is provided by the delay module (SysTick). */
uint32_t HAL_GetTick(void)
{
  return delay_millis();
}

void HAL_Delay(uint32_t ms)
{
  delay_ms(ms);
}

void HAL_MspInit(void)
{
  __HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /** DISABLE: JTAG-DP Disabled and SW-DP Disabled */
  __HAL_AFIO_REMAP_SWJ_DISABLE();
}
