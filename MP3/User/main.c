#include "delay.h"
#include "uart.h"
#include "stm32f4xx_sdio.h"

/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "fatfs_sd_sdio.h"
#include "tm_stm32f4_fatfs.h"

/*
\verbatim
   _________________
  / 1 2 3 4 5 6 7 8 |  NR   |SDIO INTERFACE                               |SPI INTERFACE
 /                  |       |NAME     STM32Fxxx     DESCRIPTION           |NAME   STM32Fxxx   DESCRIPTION 
/ 9                 |       |         4-BIT  1-BIT                        | 
|                   |       |                                             | 
|                   |   1   |CD/DAT3  PC11   -      Connector data line 3 |CS     PB5         Chip select for SPI 
|                   |   2   |CMD      PD2    PD2    Command/Response line |MOSI   PA7         Data input for SPI 
|                   |   3   |VSS1     GND    GND    GND                   |VSS1   GND         GND 
|   SD CARD Pinout  |   4   |VDD      3.3V   3.3V   3.3V Power supply     |VDD    3.3V        3.3V Power supply 
|                   |   5   |CLK      PC12   PC12   Clock                 |SCK    PA5         Clock for SPI 
|                   |   6   |VSS2     GND    GND    GND                   |VSS2   GND         GND 
|                   |   7   |DAT0     PC8    PC8    Connector data line 0 |MISO   PA6         Data output for SPI 
|                   |   8   |DAT1     PC9    -      Connector data line 1 |-      -           - 
|___________________|   9   |DAT2     PC10   -      Connector data line 2 |-      -           - 
\endverbatim
*/

/************************* PLL Parameters *************************************/
/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
#define PLL_M      8
#define PLL_N      144


/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      2

/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q      6

/******************************************************************************/

void System_Configuration(void);
void GPIO_Config(void);

FATFS FatFs; 		/* Fatfs object */
FIL fil; 				/* File object */
FRESULT fr;     /* FatFs return code */
char Rxbuffer[5];
UINT br, bw;         /* File read/write count */

int main()
{
	System_Configuration();
	SystemCoreClockUpdate();
	SysTick_Init();

	UART1_Config();


	uint32_t total, free; 	//Free and total space

	if(f_mount(&FatFs, "", 1) == FR_OK){
		
    GPIO_ToggleBits(GPIOA, GPIO_Pin_7);

		/* Open a text file */
		if (f_open(&fil, "1stfile.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK) {

			f_printf(&fil, "phamvanphuckks\n");
		
			//If we put more than 0 characters (everything OK)
			if (f_puts("First string in my file\n", &fil) > 0) {
				
				if (TM_FATFS_DriveSize(&total, &free) == FR_OK) {
						//Data for drive size are valid
				}
				
			}
			
		}
		f_close(&fil);
		
		f_open(&fil, "1stfile.txt", FA_READ);
		fr=f_read(&fil, Rxbuffer, sizeof(Rxbuffer), &br);
		/* Close the file */
		f_close(&fil);

	}
	//			//Unmount drive, don't forget this!
  //			f_mount(0, "", 1);
  while(1)
  {

		GPIO_ToggleBits(GPIOA, GPIO_Pin_6);
    delay_ms(1000);
  }
}

void System_Configuration(void)
{
/******************************************************************************/
/*             HSI used as System clock source                                */
/******************************************************************************/
	RCC_DeInit();
  __IO uint32_t HSIStatus = 0;
  
  /* Enable HSI */
  RCC->CR |= ((uint32_t)RCC_CR_HSION);
 
  /* Wait till HSI is ready  */

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
    /* Select regulator voltage output Scale 1 mode */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;

    /* HCLK = SYSCLK / 1*/
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
     
    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
    
    /* PCLK1 = HCLK / 4*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
       
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
		
		/* Configure the main PLL */
	
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSI) | (PLL_Q << 24);

    /* Enable the main PLL */
    RCC->CR |= RCC_CR_PLLON;
		

    /* Wait till the main PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
    /* Select the main PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
    {
    }
		
  }
  else
  { /* If HSI fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
	
} 
