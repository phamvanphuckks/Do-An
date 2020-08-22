/**
  ******************************************************************************
  * @file    dht11.h
  * @author  GIAOSU
  * @date    15-06-2020
  * @brief   This file contains all the functions prototypes for the miscellaneous
  *          firmware library functions (add-on to CMSIS functions).
  ******************************************************************************
  * @attention
  * This file is used to declare function
  * prototypes for communication with LCD 1602
  * All information related to copyright contact nguyendonbg@gmail.com
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_1602_H
#define __LCD_1602_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#define LCD_RS GPIO_Pin_0
#define LCD_RW GPIO_Pin_1
#define LCD_EN GPIO_Pin_2
#define LCD_D4 GPIO_Pin_3
#define LCD_D5 GPIO_Pin_4
#define LCD_D6 GPIO_Pin_5
#define LCD_D7 GPIO_Pin_6

/*!
 * @brief           delete data on LCD
 */
void LCD_Clear(void);
/*!
 * @brief           initialize LCD
 */
void LCD_Init(void);
/*!
 * @brief           Set the position where the cursor is displayed
 * @param  [IN]unsigned char x      position on the x-axis
 * @param  [IN]unsigned char y      position on the y-axis
 */
void LCD_Gotoxy(unsigned char x, unsigned char y);
/*!
 * @brief           Send string to LCD
 * @param  [IN]char *s      string pointer
 */
void LCD_Puts (char *s);
/*!
 * @brief           Show img hump
 */
void LCD_Show_IMGTemp(void);
/*!
 * @brief           Show img temp
 */
void LCD_Show_IMGHump(void);
/*!
 * @brief           Show img temp format C
 */
void LCD_Show_Temp_C(void);
#ifdef __cplusplus
}
#endif

#endif /* __LCD_1602_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2020 nguyendonbg@gmail.com *****END OF FILE****/
