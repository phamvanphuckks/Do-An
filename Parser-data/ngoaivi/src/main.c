/**
**
**                           Main.c
**
**
*******************Copyright (C)free license, 2020*****************************/
/**
 @attention
* Any commercial product related to this source code is
* required to specify the source
* This is the program to 
*
*********************************************************************
*/

#include "stm32f4xx_conf.h"
#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "fatfs_sd_sdio.h"
#include "tm_stm32f4_fatfs.h"
#include "uart.h"
#include <stdlib.h>
#include "delay.h"
#include <unistd.h>
#define TRUE 1
#define FALSE 0

#define PLL_M 8
#define PLL_N 144
/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P 2

/**USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q 6
#define button_save_start GPIO_Pin_4
#define button_save_stop GPIO_Pin_3
#define button_send_data GPIO_Pin_5

/******************************************************************************/
FATFS FatFs; /** Fatfs object */
FIL pFile;   /** File object */
FRESULT fr;  /** FatFs return code */
char Rxbuffer[500];
UINT br, bw; /** File read/write count */
extern int h, t;
char data_temp[4];
char data_hump[4];
/******************************************************************************/
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

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
GPIO_InitTypeDef GPIO_InitStructure;

void TIM_PWM_Configuration(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_Period = 1499; //
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;
    //TIM_OCStructInit(&TIM_OCInitStructure);

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    /* TIM1 enable counter */
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

/*!
 * @brief           configuration buttons
 */
static void Button_Config(void)
{
    /**Enable clock GPIOE*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitTypeDef Button_Initstructure;
    Button_Initstructure.GPIO_Mode = GPIO_Mode_IN;
    Button_Initstructure.GPIO_OType = GPIO_OType_PP;
    Button_Initstructure.GPIO_Pin = button_save_start | button_save_stop | button_send_data;
    Button_Initstructure.GPIO_PuPd = GPIO_PuPd_UP;
    Button_Initstructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOE, &Button_Initstructure);
}
/*!
 * @brief           mode show data read from sensor DHT11 on the lcd
 */

struct HEADER
{
    unsigned char riff[4];              // RIFF string
    unsigned int overall_size;          // overall size of file in bytes
    unsigned char wave[4];              // WAVE string
    unsigned char fmt_chunk_marker[4];  // fmt string with trailing null char
    unsigned int length_of_fmt;         // length of the format data
    unsigned int format_type;           // format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
    unsigned int channels;              // no.of channels
    unsigned int sample_rate;           // sampling rate (blocks per second)
    unsigned int byterate;              // SampleRate * NumChannels * BitsPerSample/8
    unsigned int block_align;           // NumChannels * BitsPerSample/8
    unsigned int bits_per_sample;       // bits per sample, 8- 8bits, 16- 16 bits etc
    unsigned char data_chunk_header[4]; // DATA string or FLLR string
    unsigned int data_size;             // NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
};

/**
* Convert seconds into hh:mm:ss format
* Params:
*    seconds - seconds value
* Returns: hms - formatted string
**/
char *seconds_to_time(float raw_seconds)
{
    char *hms;
    int hours, hours_residue, minutes, seconds, milliseconds;
    hms = (char *)malloc(100);

    sprintf(hms, "%f", raw_seconds);

    hours = (int)raw_seconds / 3600;
    hours_residue = (int)raw_seconds % 3600;
    minutes = hours_residue / 60;
    seconds = hours_residue % 60;
    milliseconds = 0;

    // get the decimal part of raw_seconds to get milliseconds
    char *pos;
    pos = strchr(hms, '.');
    int ipos = (int)(pos - hms);
    char decimalpart[15];
    memset(decimalpart, ' ', sizeof(decimalpart));
    strncpy(decimalpart, &hms[ipos + 1], 3);
    milliseconds = atoi(decimalpart);

    sprintf(hms, "%d:%d:%d.%d", hours, minutes, seconds, milliseconds);
    return hms;
}

unsigned char buffer4[4];
unsigned char buffer2[2];

char test_buff[100];
char *filename;
struct HEADER header;
int width;
int main()
{
    System_Configuration();
    SystemCoreClockUpdate();
    SysTick_Init();
    UART1_Config();
    Button_Config();
    int read = 0;
    TIM_PWM_Configuration();

    if (f_mount(&FatFs, "", 1) == FR_OK)
    {
        /** Open a text file */
        if (f_open(&pFile, "data.wav", FA_READ) == FR_OK)
        {
            read = f_read(&pFile, header.riff, sizeof(header.riff), &br);
            printf("(1-4): %s \n", header.riff);
            read = f_read(&pFile, buffer4, sizeof(buffer4), &br);
            printf("%x %x %x %x\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

            // convert little endian to big endian 4 byte int
            header.overall_size = buffer4[0] |
                                  (buffer4[1] << 8) |
                                  (buffer4[2] << 16) |
                                  (buffer4[3] << 24);
            printf("(5-8) Overall size: bytes:%u, Kb:%u \n", header.overall_size, header.overall_size / 1024);
            read = f_read(&pFile, header.wave, sizeof(header.wave), &br);
            printf("(9-12) Wave marker: %s\n", header.wave);

            read = f_read(&pFile, header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), &br);
            printf("(13-16) Fmt marker: %s\n", header.fmt_chunk_marker);

            read = f_read(&pFile, buffer4, sizeof(buffer4), &br);
            printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
            // convert little endian to big endian 4 byte integer
            header.length_of_fmt = buffer4[0] |
                                   (buffer4[1] << 8) |
                                   (buffer4[2] << 16) |
                                   (buffer4[3] << 24);
            printf("(17-20) Length of Fmt header: %u \n", header.length_of_fmt);

            read = f_read(&pFile, buffer2, sizeof(buffer2), &br);
            printf("%u %u \n", buffer2[0], buffer2[1]);

            header.format_type = buffer2[0] | (buffer2[1] << 8);
            char format_name[10] = "";
            if (header.format_type == 1)
                strcpy(format_name, "PCM");
            else if (header.format_type == 6)
                strcpy(format_name, "A-law");
            else if (header.format_type == 7)
                strcpy(format_name, "Mu-law");

            printf("(21-22) Format type: %u %s \n", header.format_type, format_name);

            read = f_read(&pFile, buffer2, sizeof(buffer2), &br);
            printf("%x %x \n", buffer2[0], buffer2[1]);

            header.channels = buffer2[0] | (buffer2[1] << 8);
            printf("(23-24) Channels: %u \n", header.channels);

            read = f_read(&pFile, buffer4, sizeof(buffer4), &br);
            printf("%x %x %x %z\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

            header.sample_rate = buffer4[0] |
                                 (buffer4[1] << 8) |
                                 (buffer4[2] << 16) |
                                 (buffer4[3] << 24);

            printf("(25-28) Sample rate: %u\n", header.sample_rate);

            read = f_read(&pFile, buffer4, sizeof(buffer4), &br);
            printf("%x %x %x %x\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
            header.byterate = buffer4[0] |
                              (buffer4[1] << 8) |
                              (buffer4[2] << 16) |
                              (buffer4[3] << 24);
            printf("(29-32) Byte Rate: %u , Bit Rate:%u\n", header.byterate, header.byterate * 8);

            read = f_read(&pFile, buffer2, sizeof(buffer2), &br);
            printf("%x %x \n", buffer2[0], buffer2[1]);

            header.block_align = buffer2[0] |
                                 (buffer2[1] << 8);
            printf("(33-34) Block Alignment: %u \n", header.block_align);

            read = f_read(&pFile, buffer2, sizeof(buffer2), &br);
            printf("%u %u \n", buffer2[0], buffer2[1]);

            header.bits_per_sample = buffer2[0] |
                                     (buffer2[1] << 8);
            printf("(35-36) Bits per sample: %u \n", header.bits_per_sample);

            read = f_read(&pFile, header.data_chunk_header, sizeof(header.data_chunk_header), &br);
            printf("(37-40) Data Marker: %s \n", header.data_chunk_header);

            read = f_read(&pFile, buffer4, sizeof(buffer4), &br);
            printf("%x %x %x %x\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

            header.data_size = buffer4[0] |
                               (buffer4[1] << 8) |
                               (buffer4[2] << 16) |
                               (buffer4[3] << 24);
            printf("(41-44) Size of data chunk: %u \n", header.data_size);

            // calculate no.of samples
            long num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);
            printf("Number of samples:%lu \n", num_samples);

            long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
            printf("Size of each sample:%ld bytes\n", size_of_each_sample);

            // calculate duration of file
            float duration_in_seconds = (float)header.overall_size / header.byterate;

            printf("Approx.Duration in h:m:s=%s\n", seconds_to_time(duration_in_seconds));
            if (header.format_type == 1)
            { // PCM
                long i = 0;
                char data_buffer[size_of_each_sample];
                int size_is_correct = TRUE;

                // make sure that the bytes-per-sample is completely divisible by num.of channels
                long bytes_in_each_channel = (size_of_each_sample / header.channels);
                if ((bytes_in_each_channel * header.channels) != size_of_each_sample)
                {
                    size_is_correct = FALSE;
                }

                if (size_is_correct)
                {
                    // the valid amplitude range for values based on the bits per sample
                    for (i = 1; i <= num_samples; i++)
                    {

                        read = f_read(&pFile, data_buffer, sizeof(data_buffer), &br);
                        if (read == 0)
                        {
                            // dump the data read
                            unsigned int xchannels = 0;
                            int data_in_channel = 0;
                            int offset = 0; // move the offset for every iteration in the loop below
                                            // convert data from little endian to big endian based on bytes in each channel sample
                            data_in_channel = data_buffer[offset] | (data_buffer[offset + 1] << 8);
                            offset += bytes_in_each_channel;
                            float a = (((float)data_in_channel / 0x10000) + 1) / 2;
                            int width = a * 1500;

                            while ((TIM1->SR & TIM_FLAG_Update) == RESET)
                                ;                        // doi co
                            TIM1->SR = ~TIM_FLAG_Update; //xoa co
                            TIM1->CCR1 = width;
                        }
                        else
                        {
                            printf("Error reading file. %d bytes\n", read);
                            break;
                        }

                    } //     for (i =1; i <= num_samples; i++) {

                } //     if (size_is_correct) {

            } //  if (header.format_type == 1) {
        }
        f_close(&pFile);
    }
    while (1)
    {

        delay_ms(100);
    } //end loop
} //end main
/******************* (C) COPYRIGHT 2020 *****END OF FILE****/
