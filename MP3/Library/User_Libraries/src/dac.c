/**
  ******************************************************************************
  * @file    dac.c
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
#include "dac.h"

void DAC_Channel1_Initial(void)
{
    DAC_Config();
}

void DAC_Config(void)
{
    DAC_InitTypeDef DAC_InitStruct;
    GPIO_InitTypeDef GPIO_InitStructut;

    /* Peripheral Clock Enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* DAC channel_1 DAC_OUT1 = PA.4 configuration */    
    GPIO_InitStructut.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructut.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructut.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructut);

    /*DAC chanel_1 Timer trigger configuration */
    DAC_InitStruct.DAC_Trigger = DAC_Trigger_T6_TRGO; 
    DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_Init(DAC_Channel_1, &DAC_InitStruct);
    
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
