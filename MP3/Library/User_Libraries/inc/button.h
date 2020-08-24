/**
  ********************************************************************************************
  * @file    button.h
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
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define button_save_start GPIO_Pin_4
#define button_save_stop GPIO_Pin_3
#define button_send_data GPIO_Pin_5

#ifdef __cplusplus
    extern "C"{
#endif
        
void button_Initial(void);
static void button_Config(void);
        
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
