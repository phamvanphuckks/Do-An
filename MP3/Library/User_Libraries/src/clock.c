/**
  ******************************************************************************
  * @file    clock.c
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
#include "clock.h"

void System_Initial(void)
{
    System_Configuration();
}

void System_Configuration(void)
{
    /******************************************************************************/
    /**             HSI used as System clock source                                */
    /******************************************************************************/
    RCC_DeInit();
    __IO uint32_t HSIStatus = 0;

    /** Enable HSI */
    RCC->CR |= ((uint32_t)RCC_CR_HSION);

    /** Wait till HSI is ready  */

    HSIStatus = RCC->CR & RCC_CR_HSIRDY;

    if ((RCC->CR & RCC_CR_HSIRDY) != RESET)
    {
        HSIStatus = (uint32_t)0x01;
    }
    else
    {
        HSIStatus = (uint32_t)0x00;
    }

    if (HSIStatus == (uint32_t)0x01)
    {
        /** Select regulator voltage output Scale 1 mode */
        RCC->APB1ENR |= RCC_APB1ENR_PWREN;
        PWR->CR |= PWR_CR_VOS;

        /** HCLK = SYSCLK / 1*/
        RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

        /** PCLK2 = HCLK / 2*/
        RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

        /** PCLK1 = HCLK / 4*/
        RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

        /** Configure Flash prefetch, Instruction cache, Data cache and wait state */
        FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;

        /**Configure the main PLL */

        RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) - 1) << 16) |
                       (RCC_PLLCFGR_PLLSRC_HSI) | (PLL_Q << 24);

        /** Enable the main PLL */
        RCC->CR |= RCC_CR_PLLON;

        /** Wait till the main PLL is ready */
        while ((RCC->CR & RCC_CR_PLLRDY) == 0)
        {
        }
        /**Select the main PLL as system clock source */
        RCC->CFGR &= (uint32_t)((uint32_t) ~(RCC_CFGR_SW));
        RCC->CFGR |= RCC_CFGR_SW_PLL;

        /** Wait till the main PLL is used as system clock source */
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
            ;
        {
        }
    }
    else
    { /** If HSI fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
    }
}

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
