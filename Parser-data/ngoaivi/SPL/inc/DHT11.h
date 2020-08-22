/**
  ******************************************************************************
  * @file    dht11.h
  * @author  GIAOSU
  * @date    15-06-2020
  * @brief   This file contains all the functions prototypes for the miscellaneous
  *          firmware library functions (add-on to CMSIS functions).
  ******************************************************************************
  * @attention
  * This file is used to declare the initialization
  * of function prototypes for the function of supporting
  * reading the DHT11 humidity temperature sensor
  * All information related to copyright contact nguyendonbg@gmail.com
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DHT11_H
#define __DHT11_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "delay.h"
#define  _BV(pos)  (1 << (pos))

#define  bool   char
#define  true   1
#define  false  0
#define MAXTIMINGS 85

#define Port_dht GPIOA
#define pin_dht GPIO_Pin_8

/*!
 * @brief           read value from sensor
 */
void read_dht11_dat(void);

#ifdef __cplusplus
}
#endif

#endif /* __DHT11_H */

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
