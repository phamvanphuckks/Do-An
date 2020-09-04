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
#include "uart.h"

extern volatile uint8_t Flag_next_song;
extern volatile uint8_t Flag_back_song;

void button_Initial(void)
{
    button_Key0_Config();
    button_Key1_Config();
    led_Config();
}

void led_Config(void)
{ 
    GPIO_InitTypeDef   Led_Initstructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   
    /* LED D2, D3*/
    Led_Initstructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    Led_Initstructure.GPIO_Mode = GPIO_Mode_OUT;
    Led_Initstructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(GPIOA, &Led_Initstructure);     
}
void button_Key0_Config(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    GPIO_InitTypeDef   Button_Initstructure;  
    NVIC_InitTypeDef   NVIC_InitStructure;
    
    /**Enable clock GPIOE*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /*Button Key0*/
    Button_Initstructure.GPIO_Pin = button_back_song_KEY0 ;
    Button_Initstructure.GPIO_Mode = GPIO_Mode_IN;
    Button_Initstructure.GPIO_OType = GPIO_OType_PP;
    Button_Initstructure.GPIO_PuPd = GPIO_PuPd_UP;
    Button_Initstructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(GPIOE, &Button_Initstructure);   
    
    /* Connect EXTI Line4 to PE4 pin */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);
    

    /* Configure EXTI Line4*/
    EXTI_InitStructure.EXTI_Line = EXTI_Line4 ;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set EXTI Line4 Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void button_Key1_Config(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    GPIO_InitTypeDef   Button_Initstructure;  
    NVIC_InitTypeDef   NVIC_InitStructure;
    
    /**Enable clock GPIOE*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    
    /*Button Key1*/
    Button_Initstructure.GPIO_Pin = button_next_song_KEY1; 
    Button_Initstructure.GPIO_Mode = GPIO_Mode_IN;
    Button_Initstructure.GPIO_OType = GPIO_OType_PP;
    Button_Initstructure.GPIO_PuPd = GPIO_PuPd_UP;
    Button_Initstructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(GPIOE, &Button_Initstructure);   
    
    
    /* Connect EXTI Line3 to PE3 pin */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);

    /* Configure EXTI line3 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set EXTI Line3 Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line4) != RESET)
  {
    /* Toggle LED2 */
    TONGGLE_LED2
    Flag_back_song = 1;
    /* Clear the EXTI line 4 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line4);
  }
}

/**
  * @brief  This function handles External lines 3  interrupt request.
  * @param  None
  * @retval None
  */
void EXTI3_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line3) != RESET)
  {
    /* Toggle LED3 */
    Flag_next_song = 1;
    TONGGLE_LED3
    /* Clear the EXTI line 3 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line3);
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
