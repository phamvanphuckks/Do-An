/**
  ******************************************************************************
  * @file    button.c
  * @author  PhamVanPhuckks
  * @date    2020
  * @brief   This file contains all the functions definition for ....
  *          firmware library (add-on to CMSIS functions).
  ******************************************************************************
  * @attention
  * This file is used to describe in detail
  * the functions that serve.....
  * All information related to copyright contact phamvanphuckks@gmail.com
  ******************************************************************************
  */
#include "button.h"

void button_Initial(void)
{
    button_Config();
}
void button_Config(void)
{
    /**Enable clock GPIOE*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitTypeDef Button_Initstructure;
    Button_Initstructure.GPIO_Mode = GPIO_Mode_IN;
    Button_Initstructure.GPIO_OType = GPIO_OType_PP;
    Button_Initstructure.GPIO_Pin = button_save_start | button_save_stop | button_send_data;
    Button_Initstructure.GPIO_PuPd = GPIO_PuPd_UP;
    Button_Initstructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOE, &Button_Initstructure);   
}
//#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
//#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
//#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
//#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0


///*???????????????????????IO*/
///*
//#define KEY0 		PEin(4)   	//PE4
//#define KEY1 		PEin(3)		//PE3 
//#define KEY2 		PEin(2)		//P32
//#define WK_UP 	PAin(0)		//PA0
//*/


//#define KEY0_PRES 	1
//#define KEY1_PRES	2
////#define KEY2_PRES	3

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
