/**
  ******************************************************************************
  * @file    dma.c
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
#include "dma.h"
#include "dac.h"

void DACDMA_Initial(int16_t *Memory0BaseAddr)
{
    DMA_InitTypeDef DMA_InitStructure;
    
    /* DMA1 clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

    /* DMA1_Streamx channel_x configuration */  
    DMA_DeInit(DMA1_Stream5);
    DMA_InitStructure.DMA_Channel = DMA_Channel_7;  
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) DAC_DHR12R1_ADDRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(Memory0BaseAddr);
    
    /*
    BufferSize decrements after each DMA transfer, 
    Once the transfer has completed, this register can either stay at zero (when the stream is in
    normal mode) or be reloaded automatically with the previously programmed value in the following cases:
    – when the stream is configured in Circular mode.
    – when the stream is enabled again by setting EN bit to '1'
    If the value of this register is zero, no transaction can be served even if the stream is
    enabled.
    */
    
    DMA_InitStructure.DMA_BufferSize = 1; 
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // the Peripheral data width
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; // the Memory data width.
    
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; // data transfer from memory(array,variable) to Peripheral(DAC_DHRx)
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // whether the Peripheral address register should be incremented or not.
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; // Specifies whether the memory address register should be incremented or not
    
    
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // the operation mode of the DMA Streamx is Circular
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //  the software priority for the DMA Stream
    
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //  Direct mode will be used for the Stream  
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; // the FIFO threshold level
    
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; // the Burst transfer configuration for the memory transfers
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; // the Burst transfer configuration for the peripheral transfers. 
    DMA_Init(DMA1_Stream5, &DMA_InitStructure);  

    /* Enable DMA1_Streamx */
    DMA_Cmd(DMA1_Stream5, ENABLE);

    /* Enable DAC Channel1 */
    DAC_Cmd(DAC_Channel_1, ENABLE);

    /* Enable DMA for DAC Channel1 */
    DAC_DMACmd(DAC_Channel_1, ENABLE);
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
