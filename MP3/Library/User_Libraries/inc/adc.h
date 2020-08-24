#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f4xx_adc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#define ADCx                     ADC3
#define ADC_CHANNEL              ADC_Channel_10
#define ADCx_CLK                 RCC_APB2Periph_ADC3
#define ADCx_CHANNEL_GPIO_CLK    RCC_AHB1Periph_GPIOC
#define GPIO_PIN                 GPIO_Pin_0
#define GPIO_PORT                GPIOC

void ADC_Initial(void);


#endif
