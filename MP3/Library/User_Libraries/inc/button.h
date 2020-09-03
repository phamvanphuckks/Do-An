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
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"

#define button_save_start GPIO_Pin_4
#define button_save_stop  GPIO_Pin_3
#define button_send_data  GPIO_Pin_5
// KEY1 PE3, KEY0 PE4
#define button_next_song_KEY1 GPIO_Pin_3
#define button_back_song_KEY0 GPIO_Pin_4

#define TONGGLE_LED2    GPIO_ToggleBits(GPIOA, GPIO_Pin_6);
#define TONGGLE_LED3    GPIO_ToggleBits(GPIOA, GPIO_Pin_7);

#ifdef __cplusplus
    extern "C"{
#endif
        
void button_Initial(void);
static void button_Key0_Config(void);
static void button_Key1_Config(void);
static void led_Config(void);
        
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
