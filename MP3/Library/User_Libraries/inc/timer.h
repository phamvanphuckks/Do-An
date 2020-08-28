/**
  ********************************************************************************************
  * @file    timer.h
  * @author  PhamVanPhuckks
  * @date    2020
  * @brief   This file contains all the functions prototypes for ...
  *          firmware library functions (add-on to CMSIS functions).
  *********************************************************************************************
  * @attention
  * This file is used to declare function
  * prototypes for the.....
  * All information related to copyright contact phamvanphuckks@gmail.com
  *********************************************************************************************
  */
#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#ifdef __cplusplus
 extern "C" {
#endif 

void TIM6_Trigger_DAC_Init(void);     
static void TIM6_Trigger_DAC_Config(uint16_t arr, uint16_t psc);   
     
#ifdef __cplusplus
}
#endif

#endif

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2020 phamvanphuckks@gmail.com *****END OF FILE****/
