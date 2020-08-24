/**
  ******************************************************************************
  * @file    uart.c
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
#include "uart.h"

struct __FILE {
    int dummy;
};
FILE __stdout;

int fputc(int ch, FILE *f){
    /* Send your custom byte */
    USART_SendData(USART1, ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
    /* If everything is OK, you have to return character written */
    return ch;
}


void UART_SendChar(USART_TypeDef *USART, char data){

    USARTx->DR = 0x00;
    USART_SendData(USART,data);
    //TxE = 1: Data is transferred to the shift register)
    //TxE = 0; Data is not transferred to the shift register
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

void UART_Puts(USART_TypeDef *USART, char *data)
{
    while(*data){
        UART_SendChar(USARTx, *data);
        data++;
    }
}

uint8_t USART_GetChar(USART_TypeDef* USART){
    uint8_t Data;
    while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
    Data = (uint8_t)USART_ReceiveData(USARTx);
    return Data;
}

void UART1_Config(void){

    USART_InitTypeDef USART_InitStruct;
    GPIO_InitTypeDef GPIO_InitStructut;
    NVIC_InitTypeDef NVIC_InitStructut;

    /* Peripheral Clock Enable -------------------------------------------------*/
    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);

    /* USARTx GPIO configuration -----------------------------------------------*/
    /* Connect USART pins to AF7 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    /* Configure USART Tx and Rx as alternate function push-pull */
    GPIO_InitStructut.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructut.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructut.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructut.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_InitStructut.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_InitStructut);

    GPIO_InitStructut.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructut);

    /*NVIC---------------------------------------------------------------*/
    NVIC_InitStructut.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructut.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructut.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructut.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructut);


    /* USARTx configuration ----------------------------------------------------*/

    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStruct);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
    NVIC_EnableIRQ(USART1_IRQn);

    /* Configure LED- PA6*/
    GPIO_InitStructut.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_7;
    GPIO_InitStructut.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructut.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructut.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructut);
}

void UART6_Config(void){
    USART_InitTypeDef USART_InitStruct;
    GPIO_InitTypeDef GPIO_InitStructut;

    /* Peripheral Clock Enable -------------------------------------------------*/
    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART6, ENABLE);

    /* USARTx GPIO configuration -----------------------------------------------*/
    /* Connect USART pins to AF8 */
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

    GPIO_InitStructut.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructut.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructut.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructut.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_InitStructut.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOC, &GPIO_InitStructut);

    GPIO_InitStructut.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOC, &GPIO_InitStructut);

    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    /* When using Parity the word length must be configured to 9 bits */
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART6, &USART_InitStruct);

    USART_Cmd(USART6, ENABLE);
}
int _write(int file, char *ptr, int len)
{
    for (int i = len; i != 0; i--)
    {
        while ((USART1->SR & USART_FLAG_TXE) == 0);
        USART1->DR = *ptr++;
    }
    return len;
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
