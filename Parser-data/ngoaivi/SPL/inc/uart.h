#ifndef __UART_H
#define __UART_H

#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stdio.h"
#include "misc.h"

/* Definition for USARTx resources ****************************/
#define USARTx                           USART1
#define USARTx_CLK                       RCC_APB2Periph_USART1
#define USARTx_CLK_INIT                  RCC_APB2PeriphClockCmd
#define USARTx_IRQn                      USART1_IRQn
#define USARTx_IRQHandler                USART1_IRQHandler

#define USARTx_TX_PIN                    GPIO_Pin_9
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_GPIO_CLK               RCC_AHB1Periph_GPIOA
#define USARTx_TX_SOURCE                 GPIO_PinSource9
#define USARTx_TX_AF                     GPIO_AF_USART1

#define USARTx_RX_PIN                    GPIO_Pin_10
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_GPIO_CLK               RCC_AHB1Periph_GPIOA
#define USARTx_RX_SOURCE                 GPIO_PinSource10
#define USARTx_RX_AF                     GPIO_AF_USART1

//RS485 Direction control
#define SSerialTxControl 								 GPIO_Pin_8
#define RS485Transmit    								 1
#define RS485Receive     								 0


uint8_t USART_GetChar(USART_TypeDef* USART);
void UART_SendChar(USART_TypeDef *USART, char data);
void UART_Puts(USART_TypeDef *USART, char *data);
void UART1_Config(void);
void UART6_Config(void);
int _write(int file, char *ptr, int len);
/*--------------------end--------------------------------------*/

#endif
