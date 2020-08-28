/**
  ********************************************************************************************
  * @file    dac.h
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
#ifndef __DAC_H__
#define __DAC_H__

#include "stm32f4xx_dac.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#define DAC_DHR12R1_ADDRESS    0x40007408UL
#define DAC_DHR8R1_ADDRESS     0x40007410UL

extern uint8_t _data_wave;

#ifdef __cplusplus
 extern "C" {
#endif 

void DAC_Channel1_Initial(void);     
static void DAC_Config(void);
     
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
