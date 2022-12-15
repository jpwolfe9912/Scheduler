/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f7xx_it.h
  * @brief   This file contains the headers of the interrupt handlers.
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
#ifndef __STM32F7xx_IT_H
#define __STM32F7xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif

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
#define COUNT_1000HZ 1U // Number of 1000 Hz frames for 500 Hz Loop
#define COUNT_500HZ 2U  // Number of 1000 Hz frames for 500 Hz Loop
#define COUNT_200HZ 5U  // Number of 1000 Hz frames for 100 Hz Loop
#define COUNT_100HZ 10U // Number of 1000 Hz frames for 100 Hz Loop
#define COUNT_50HZ 20U  // Number of 1000 Hz frames for  50 Hz Loop
#define COUNT_10HZ 100U // Number of 1000 Hz frames for  10 Hz Loop
#define COUNT_5HZ 200U  // Number of 1000 Hz frames for   5 Hz Loop
#define COUNT_1HZ 1000U // Number of 1000 Hz frames for   1 Hz Loop

extern uint16_t globalTick;
extern volatile uint8_t loopMask;
extern volatile bool flag;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F7xx_IT_H */
