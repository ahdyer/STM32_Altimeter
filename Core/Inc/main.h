/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f0xx_hal.h"

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
#define Status_1_Pin GPIO_PIN_0
#define Status_1_GPIO_Port GPIOA
#define Status_2_Pin GPIO_PIN_1
#define Status_2_GPIO_Port GPIOA
#define Acc_Int_Pin GPIO_PIN_2
#define Acc_Int_GPIO_Port GPIOA
#define EM1_Trig_Pin GPIO_PIN_0
#define EM1_Trig_GPIO_Port GPIOB
#define EM2_Trig_Pin GPIO_PIN_1
#define EM2_Trig_GPIO_Port GPIOB
#define EM3_Trig_Pin GPIO_PIN_2
#define EM3_Trig_GPIO_Port GPIOB
#define Buz_Trig_Pin GPIO_PIN_8
#define Buz_Trig_GPIO_Port GPIOA
#define DRO_SW_Pin GPIO_PIN_11
#define DRO_SW_GPIO_Port GPIOA
#define SFT_SW_Pin GPIO_PIN_12
#define SFT_SW_GPIO_Port GPIOA
#define EM4_Trig_Pin GPIO_PIN_3
#define EM4_Trig_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
