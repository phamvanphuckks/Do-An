/**
  ********************************************************************************************
  * @file    clock.h
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
#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "stm32f4xx_rcc.h"
/*
• f(VCO clock) = f(PLL clock input) × (PLLN / PLLM)
• f(PLL general clock output) = f(VCO clock) / PLLP
• f(USB OTG FS, SDIO, RNG clock output) = f(VCO clock) / PLLQ

clock source HSI:16Mhz
*/

#define PLL_M 8
#define PLL_N 144
/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P 2

/**USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q 6

#ifdef __cplusplus
    extern "C"{
#endif
        
void System_Initial(void);  
static void System_Configuration(void);
        
        
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
