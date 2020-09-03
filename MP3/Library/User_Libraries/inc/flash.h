/**
  ********************************************************************************************
  * @file    flash.h
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
  
#ifndef __FLASH_H__
#define __FLASH_H__

#include "stm32f4xx_flash.h"

#ifdef __cplusplus
    extern "C"{
#endif

void FLASH_EarsePage();
void FLASH_Sector();
void Get_Song();
      
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
