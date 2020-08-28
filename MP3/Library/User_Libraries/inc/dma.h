/**
  ********************************************************************************************
  * @file    dma.h
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
#ifndef __DMA_H__
#define __DMA_H__


#include "stm32f4xx_dma.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"

#ifdef __cplusplus
 extern "C" {
#endif 
     
void DACDMA_Initial(int16_t *Memory0BaseAddr);
     
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
