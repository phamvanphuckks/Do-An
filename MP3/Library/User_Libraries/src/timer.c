/**
  ******************************************************************************
  * @file    timer.c
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
#include "timer.h"


void TIM6_Trigger_ADC_Init()
{
    // 44.1 Khz arr=1633
    TIM6_Trigger_ADC_Config(7199UL, 9999UL);
}

static void TIM6_Trigger_ADC_Config(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
    /* TIM6 Periph clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    /* --------------------------------------------------------
    TIM6 input clock (TIM6CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
    TIM6CLK = 2 * PCLK1  
    
    TIM6CLK = 2*(HCLK / 4) = SystemCoreClock /2 
          
    ----------------------------------------------------------- */
    /* Time base configuration */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
    TIM_TimeBaseStructure.TIM_Period = arr;          
    TIM_TimeBaseStructure.TIM_Prescaler = psc;       
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    /* TIM6 TRGO selection 
        The update event is selected as the trigger output
        every time an update event occured, the DAC is active
    */
    TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

    /* TIM6 enable counter */
    TIM_Cmd(TIM6, ENABLE);
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
