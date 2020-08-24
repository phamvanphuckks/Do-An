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
#include <stdlib.h>
#include "delay.h"
#include "uart.h"
#include "clock.h"
#include "timer.h"
#include "dac.h"
#include "dma.h"

#include "adc.h"

#define TRUE 1
#define FALSE 0

/******************************************************************************/
FATFS FatFs; /** Fatfs object */
FIL pFile;   /** File object */
FRESULT fr;  /** FatFs return code */
char Rxbuffer[500];
UINT br, bw; /** File read/write count */
extern int h, t;

/******************************************************************************/



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

/*---------------------------*/
uint32_t test_trigger;

FATFS fs;
FRESULT res;
char buff[256];


FRESULT showlist(char* path)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;

    res = f_opendir(&dir, path);                       /* Open the directory */
    printf("path %s\n", path);
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);  
                printf("%s%s - 1\n", path, fno.fname);
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                printf("%s%s - 2\n", path, fno.fname);
            }

        }
        f_closedir(&dir);
    }

    return res;
}

FRESULT scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;

    res = f_opendir(&dir, path);                       /* Open the directory */
    printf("path %s\n", path);
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);  
                //printf("%s/%s - 1\n", path, fno.fname);
                printf("%s - 1\n", fno.fname);
                
                //res = scan_files(path);                    /* Enter the directory */
                
                //showlist(path);
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                //printf("%s/%s - 2\n", path, fno.fname);
            }

        }
        f_closedir(&dir);
    }

    return res;
}

int main()
{
    System_Initial();
    SystemCoreClockUpdate();
    SysTick_Init();
    
    UART1_Config();
//    Button_Config();
    int read = 0;
    TIM6_Trigger_ADC_Init();  
    DAC_Initial();    
    DACDMA_Ch1_Initial(&test_trigger);
    /*-------------------------------------------------*/

    ADC_Initial();
//    res = f_mount(&fs, "", 1);

//    if (res == FR_OK) {
//        strcpy(buff, "/");
////    strcpy(buff, "//test");
//        res = scan_files(buff);
//    }
    /*-------------------------------------------------*/
    
    if (f_mount(&FatFs, "", 1) == FR_OK)
    {
        /** Open a file */
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
            printf("%x %x %x %x\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

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
                            int offset = 0; // move the offset for every iteration in the loop below.
                            // convert data from little endian to big endian based on bytes in each channel sample.
                            data_in_channel = data_buffer[offset] | (data_buffer[offset + 1] << 8);
                            offset += bytes_in_each_channel;
                            
                            float a = (((float)data_in_channel / 0x10000) + 1) / 2;
                            int width = a * 1500;
                            
                            
                            // The DHRx register is then loaded into the DORx register
                            // DAC_output = VREF * DOR/4096
                            test_trigger = width;
                            printf("width = %d ADC = %d \n", width, ADC_GetConversionValue(ADC3));
                            
                            
                            // doi den khi DMA xong
                            while(DMA_GetFlagStatus(DMA1_Stream5, DMA_FLAG_TCIF5) == RESET);
                            DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
                        }
                        else
                        {
                            printf("Error reading file. %d bytes\n", read);
                            break;
                        }

                    } //     for (i =1; i <= num_samples; i++) {

                } //     if (size_is_correct) {

            } //  if (header.format_type == 1) {
            
        } //  if (f_open(&pFile, "data.wav", FA_READ) == FR_OK){
        f_close(&pFile);
    }
    while (1)
    {
        delay_ms(1000);
    } //end loop
} //end main
/******************* (C) COPYRIGHT 2020 *****END OF FILE****/
