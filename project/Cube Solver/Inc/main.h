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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY0_Pin GPIO_PIN_4
#define KEY0_GPIO_Port GPIOE
#define KEY0_EXTI_IRQn EXTI4_IRQn
#define TS_CS_Pin GPIO_PIN_13
#define TS_CS_GPIO_Port GPIOC
#define LED0_Pin GPIO_PIN_9
#define LED0_GPIO_Port GPIOF
#define LED1_Pin GPIO_PIN_10
#define LED1_GPIO_Port GPIOF
#define LED_Pin GPIO_PIN_3
#define LED_GPIO_Port GPIOC
#define KEY_Pin GPIO_PIN_3
#define KEY_GPIO_Port GPIOA
#define STEP_Pin GPIO_PIN_7
#define STEP_GPIO_Port GPIOA
#define DIR_Pin GPIO_PIN_4
#define DIR_GPIO_Port GPIOC
#define TS_CLK_Pin GPIO_PIN_0
#define TS_CLK_GPIO_Port GPIOB
#define TS_PEN_Pin GPIO_PIN_1
#define TS_PEN_GPIO_Port GPIOB
#define TS_DOUT_Pin GPIO_PIN_2
#define TS_DOUT_GPIO_Port GPIOB
#define TS_DIN_Pin GPIO_PIN_11
#define TS_DIN_GPIO_Port GPIOF
#define S3_Pin GPIO_PIN_12
#define S3_GPIO_Port GPIOB
#define S2_Pin GPIO_PIN_13
#define S2_GPIO_Port GPIOB
#define BL_Pin GPIO_PIN_15
#define BL_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
