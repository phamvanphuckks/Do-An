/**
  ******************************************************************************
  * @file    delay.h
  * @author  Giaosu
  * @version
  * @date    2020
  * @brief   This file contains all the functions prototypes for the miscellaneous
  *          firmware library functions (add-on to CMSIS functions).
  ******************************************************************************
  * @attention
  *
  * Any commercial product related to this source code is
  * required to specify the source
  * <h2><center>&copy; COPYRIGHT 2020 nguyendonbg@gmail.com</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/*!
 * @brief           function init systick
 */
void SysTick_Init(void);
/*!
 * @brief           function interrup sevice systick
 */
void SysTick_Handler(void);
uint64_t SysTick64(void);
uint32_t SysTick32(void);
uint32_t SysTick24(void);
uint64_t SysTick_Millis(void);
uint64_t SysTick_Micros(void);
/*!
 * @brief    function delay microseconds
 * @param  [IN]unsigned long us    value delay microseconds
 */

void delay_us(unsigned long us);
/*!
 * @brief    function delay milliseconds
 * @param  [IN]unsigned long ms    value delay milliseconds
 */
void delay_ms(unsigned long ms);

#ifdef __cplusplus
}
#endif

#endif /* __MISC_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2020 nguyendonbg@gmail.com*****END OF FILE****/
