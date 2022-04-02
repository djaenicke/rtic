/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IMU_INT_Pin GPIO_PIN_13
#define IMU_INT_GPIO_Port GPIOC
#define HSE_IN_Pin GPIO_PIN_0
#define HSE_IN_GPIO_Port GPIOH
#define HSE_OUT_Pin GPIO_PIN_1
#define HSE_OUT_GPIO_Port GPIOH
#define HEARTBEAT_LED_Pin GPIO_PIN_2
#define HEARTBEAT_LED_GPIO_Port GPIOC
#define D1_AIN1_Pin GPIO_PIN_3
#define D1_AIN1_GPIO_Port GPIOC
#define D1_MOTORA_CHA_Pin GPIO_PIN_0
#define D1_MOTORA_CHA_GPIO_Port GPIOA
#define D1_MOTORA_CHB_Pin GPIO_PIN_1
#define D1_MOTORA_CHB_GPIO_Port GPIOA
#define D1_PWMA_Pin GPIO_PIN_2
#define D1_PWMA_GPIO_Port GPIOA
#define D1_PWMB_Pin GPIO_PIN_3
#define D1_PWMB_GPIO_Port GPIOA
#define D1_AIN2_Pin GPIO_PIN_4
#define D1_AIN2_GPIO_Port GPIOA
#define VBATT_AIN_Pin GPIO_PIN_5
#define VBATT_AIN_GPIO_Port GPIOA
#define D1_MOTORB_CHA_Pin GPIO_PIN_6
#define D1_MOTORB_CHA_GPIO_Port GPIOA
#define D1_MOTORB_CHB_Pin GPIO_PIN_7
#define D1_MOTORB_CHB_GPIO_Port GPIOA
#define D1_BIN1_Pin GPIO_PIN_4
#define D1_BIN1_GPIO_Port GPIOC
#define D1_BIN2_Pin GPIO_PIN_5
#define D1_BIN2_GPIO_Port GPIOC
#define D1_STBY_Pin GPIO_PIN_0
#define D1_STBY_GPIO_Port GPIOB
#define D2_AIN1_Pin GPIO_PIN_1
#define D2_AIN1_GPIO_Port GPIOB
#define D2_AIN2_Pin GPIO_PIN_2
#define D2_AIN2_GPIO_Port GPIOB
#define CAN2_RS_Pin GPIO_PIN_10
#define CAN2_RS_GPIO_Port GPIOB
#define D2_PWMA_Pin GPIO_PIN_14
#define D2_PWMA_GPIO_Port GPIOB
#define D2_PWMB_Pin GPIO_PIN_15
#define D2_PWMB_GPIO_Port GPIOB
#define D2_MOTORA_CHA_Pin GPIO_PIN_6
#define D2_MOTORA_CHA_GPIO_Port GPIOC
#define D2_MOTORA_CHB_Pin GPIO_PIN_7
#define D2_MOTORA_CHB_GPIO_Port GPIOC
#define D2_BIN2_Pin GPIO_PIN_8
#define D2_BIN2_GPIO_Port GPIOC
#define D2_BIN1_Pin GPIO_PIN_9
#define D2_BIN1_GPIO_Port GPIOC
#define D2_MOTORB_CHA_Pin GPIO_PIN_8
#define D2_MOTORB_CHA_GPIO_Port GPIOA
#define D2_MOTORB_CHB_Pin GPIO_PIN_9
#define D2_MOTORB_CHB_GPIO_Port GPIOA
#define D2_STBY_Pin GPIO_PIN_10
#define D2_STBY_GPIO_Port GPIOA
#define RF_SCK_Pin GPIO_PIN_10
#define RF_SCK_GPIO_Port GPIOC
#define RF_MISO_Pin GPIO_PIN_11
#define RF_MISO_GPIO_Port GPIOC
#define RF_MOSI_Pin GPIO_PIN_12
#define RF_MOSI_GPIO_Port GPIOC
#define RF_CS_Pin GPIO_PIN_2
#define RF_CS_GPIO_Port GPIOD
#define RF_IRQ_Pin GPIO_PIN_4
#define RF_IRQ_GPIO_Port GPIOB
#define RF_EN_Pin GPIO_PIN_5
#define RF_EN_GPIO_Port GPIOB
#define DEBUG_TX_Pin GPIO_PIN_6
#define DEBUG_TX_GPIO_Port GPIOB
#define DEBUG_RX_Pin GPIO_PIN_7
#define DEBUG_RX_GPIO_Port GPIOB
#define IMU_SCL_Pin GPIO_PIN_8
#define IMU_SCL_GPIO_Port GPIOB
#define IMU_SDA_Pin GPIO_PIN_9
#define IMU_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
