/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void display_distort (uint8_t state);
extern uint8_t 	 	ADC_flag ;

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SW1_1_Pin GPIO_PIN_2
#define SW1_1_GPIO_Port GPIOE
#define SW1_2_Pin GPIO_PIN_4
#define SW1_2_GPIO_Port GPIOE
#define SW1_3_Pin GPIO_PIN_6
#define SW1_3_GPIO_Port GPIOE
#define SW2_1_Pin GPIO_PIN_14
#define SW2_1_GPIO_Port GPIOC
#define SW2_2_Pin GPIO_PIN_0
#define SW2_2_GPIO_Port GPIOF
#define SW3_1_Pin GPIO_PIN_2
#define SW3_1_GPIO_Port GPIOF
#define SW3_2_Pin GPIO_PIN_4
#define SW3_2_GPIO_Port GPIOF
#define KEY0_Pin GPIO_PIN_9
#define KEY0_GPIO_Port GPIOG
#define KEY0_EXTI_IRQn EXTI9_5_IRQn
#define KEY1_Pin GPIO_PIN_10
#define KEY1_GPIO_Port GPIOG
#define KEY1_EXTI_IRQn EXTI15_10_IRQn
#define KEY2_Pin GPIO_PIN_11
#define KEY2_GPIO_Port GPIOG
#define KEY2_EXTI_IRQn EXTI15_10_IRQn
/* USER CODE BEGIN Private defines */
#define NPT  1024  //1024点FFT
#define Fs 512  //采样频率 5120Hz 频率分辨率 5Hz
#define PI2 6.28318530717959
#define PD  64	//1000Hz时的采样点
#define Hn	5   //所取的最大谐波次数
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
