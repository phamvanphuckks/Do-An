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

#include "button.h"
#include "lcd16x2.h"

#define TRUE 1
#define FALSE 0


/******************************************************************************/
FATFS FatFs; /** Fatfs object */
FIL pFile;   /** File object */
DIR pdir;
FILINFO fno;
FRESULT fr;  /** FatFs return code */

UINT br, bw; /** File read/write count */
/******************************************************************************/

volatile uint8_t Flag_next_song = 0;
volatile uint8_t Flag_back_song = 0;
volatile uint8_t number_of_song = 0;
uint8_t pos_song = 0;

typedef struct SONGS_LIST
{
    char ptr_previous_song[255];
    char ptr_current_song[255];
    char ptr_next_song[255];
}LIST;

LIST list; //= (LIST*)malloc(sizeof(LIST));



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
FATFS fs;
FRESULT res;
char buff[256];

int16_t wavBuffer1[256], wavBuffer2[256];
/*---------------------------*/

// parameter  sing_name ViYeuCuDamDau
void File_Processed(char *song_name)
{
   int read = 0;
   if (f_mount(&FatFs, "", 1) == FR_OK)
    {
        /** Open a file */
        if (f_open(&pFile, song_name, FA_READ) == FR_OK)
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
                
                char data_buffer[512];
                int size_is_correct = TRUE;

                // make sure that the bytes-per-sample is completely divisible by num.of channels
                long bytes_in_each_channel = (size_of_each_sample / header.channels);
                if ((bytes_in_each_channel * header.channels) != size_of_each_sample)
                {
                    size_is_correct = FALSE;
                }
            
                if (size_is_correct)
                {
                    TIM6_Trigger_DAC_Init((PCLK1/header.sample_rate - 1), 0);     
                    printf("start\n");    
                    while(pFile.fsize != pFile.fptr)
                    {
                        if((Flag_next_song == 1)|| (Flag_back_song ==1)) break;
                        
                        read = f_read(&pFile, data_buffer, sizeof(data_buffer), &br);

                        if (read == 0)
                        {
                            uint16_t index_wav2 = 0;
                            for(int offset = 0; offset < 512 ; offset += bytes_in_each_channel)
                            {
                                int16_t data_in_channel = 0;
                                data_in_channel = data_buffer[offset] | (data_buffer[offset + 1] << 8); 
                                //printf("%x%x   ", data_buffer[offset] , data_buffer[offset + 1]);
                                float a = (((float)data_in_channel / 0x10000) + 1) / 2;
                                int width = a * 1500;              
                                wavBuffer2[index_wav2++] = width;
                            }
                        }
                        else{
                            printf("read error");
                            break;
                        }
                        while(DMA_GetFlagStatus(DMA1_Stream5, DMA_FLAG_TCIF5) == RESET);
                        DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);  
                        
                        DMA_Cmd(DMA1_Stream5, DISABLE);
                        DACDMA_Initial(wavBuffer2); 

                        read = f_read(&pFile, data_buffer, sizeof(data_buffer), &br);

                        if (read == 0)
                        {
                            uint16_t index_wav1 = 0;
                            for(int offset = 0; offset < 512 ; offset += bytes_in_each_channel)
                            {
                                int16_t data_in_channel = 0;
                                data_in_channel = data_buffer[offset] | (data_buffer[offset + 1] << 8);                       
                                float a = (((float)data_in_channel / 0x10000) + 1) / 2;
                                int width = a * 1500;              
                                wavBuffer1[index_wav1++] = width;
                           //     printf("%x%x,%d, wavBuffer1[%d] = %d \n", data_buffer[offset], data_buffer[offset + 1],data_in_channel, index_wav1, wavBuffer1[index_wav1]);
                            }
                        }
                        else{
                            printf("read error");
                            break;
                        } 
                        while(DMA_GetFlagStatus(DMA1_Stream5, DMA_FLAG_TCIF5) == RESET);
                        DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
                        
                        DMA_Cmd(DMA1_Stream5, DISABLE);
                        DACDMA_Initial(wavBuffer1);                         

                    } //     while(!read) {
                    printf("stop\n");
                } //     if (size_is_correct) {

            } //  if (header.format_type == 1) {

        } //  if (f_open(&pFile, "data.wav", FA_READ) == FR_OK){
        f_close(&pFile);
    }
    DMA_Cmd(DMA1_Stream5, DISABLE);
}

void Initial_Structure_List()
{
//    list = (LIST*)malloc(sizeof(LIST));   
    res = f_mount(&fs, "", 1); // register work area

    if (res == FR_OK)
    {
        FRESULT res;

        static TCHAR lfname[_MAX_LFN]; 
        char LFNBuf[_MAX_LFN];
        
        res = f_opendir(&pdir, "//music"); /* Open the directory */
        if (res == FR_OK)
        {
            printf("Directory listing...\r\n"); 
            for ( ; ; ) 
            { 
                fno.lfname = lfname; 
                fno.lfsize = _MAX_LFN - 1; 
               
                res = f_readdir(&pdir, &fno); /* Read a directory item */ 
                if (res || !fno.fname[0])   break;                         
                if ( !(fno.fattrib & AM_DIR)) { /* It is a file. */
                    sprintf(LFNBuf, "   %8lu  %s\r\n", fno.fsize, fno.lfname[0] ? fno.lfname : fno.fname);
                    number_of_song++;  
                    printf("%s\n", fno.lfname);
                }               
            }
        }
        f_closedir(&pdir);
        res = f_opendir(&pdir, "//music"); /* Open the directory */
        if (res == FR_OK)
        {        
            for ( int index = 0; index < number_of_song ; index++) 
            { 
                fno.lfname = lfname; 
                fno.lfsize = _MAX_LFN - 1; 

                do{
                    res = f_readdir(&pdir, &fno);
                        
//                    printf("%d - %s", fno.lfname[0]=='.', fno.lfname);
                }while(fno.lfname[0]=='.' || fno.lfname[0]=='\0');  // neu ma doc duoc "." or ".." f_readdir() de bo qua          
                
                if (res || !fno.fname[0])   break;                      
                if (!(fno.fattrib & AM_DIR)) {  /* It is a file. */
                    sprintf(LFNBuf, "//music/%s", fno.lfname[0] ? fno.lfname : fno.fname);  
                } 
                //printf("%s\n", LFNBuf); 
                if(index == 0) {
                    strcpy(list.ptr_current_song, LFNBuf);
                    printf("list.ptr_current_song : %s\n", list.ptr_current_song);                      
                }
                if(index == 1) {
                    strcpy(list.ptr_next_song, LFNBuf);
                    printf("list.ptr_next_song : %s\n", list.ptr_next_song);     
                }
                if(index == number_of_song - 1) 
                {
                    strcpy(list.ptr_previous_song, LFNBuf);
                    printf("list.ptr_previous_song : %s\n", list.ptr_previous_song);
                }
            }
        }
        f_closedir(&pdir);
    }    
}

uint8_t next_update_strucre_list()
{
    res = f_mount(&fs, "", 1); // register work area

    if (res == FR_OK)
    {
        FRESULT res;

        static TCHAR lfname[_MAX_LFN]; 
        char LFNBuf[_MAX_LFN];
        
        res = f_opendir(&pdir, "//music"); /* Open the directory */
        if (res == FR_OK)
        {
            printf("Directory listing...\r\n"); 
            for (; ; ) 
            { 
                fno.lfname = lfname; 
                fno.lfsize = _MAX_LFN - 1; 
                res = f_readdir(&pdir, &fno);  /* Read a directory item */ 

                if (!(fno.fattrib & AM_DIR)) { /* It is a file. */
                    sprintf(LFNBuf, "//music/%s", fno.lfname[0] ? fno.lfname : fno.fname); 
                    //printf("LFNBuf : %s \n", LFNBuf);
                    if(!strcmp(LFNBuf, list.ptr_current_song))
                    {
                        printf("vao na\n");
                        printf("dau vao hien tai %s\n", LFNBuf);
                        strcpy(list.ptr_previous_song, list.ptr_current_song);
                        strcpy(list.ptr_current_song, list.ptr_next_song);
                        
                        res = f_readdir(&pdir, &fno); 
                        res = f_readdir(&pdir, &fno); 
                        sprintf(LFNBuf, "//music/%s", fno.lfname[0] ? fno.lfname : fno.fname); 
                        printf("doc them 2 lan nua %s\n", LFNBuf); 
                        
                        if (!fno.fname[0] && !lfname[0]){
                            dir_sdi(&pdir, 0);
                            printf("dao chieu ne \n");
                            do{
                                res = f_readdir(&pdir, &fno); 
                                sprintf(LFNBuf, "//music/%s", fno.lfname[0] ? fno.lfname : fno.fname); 
                                strcpy(list.ptr_next_song, LFNBuf);
                                if(!strcmp(LFNBuf, list.ptr_current_song))
                                {
                                    res = f_readdir(&pdir, &fno); 
                                    sprintf(LFNBuf, "//music/%s", fno.lfname[0] ? fno.lfname : fno.fname); 
                                    strcpy(list.ptr_next_song, LFNBuf);
                                }
                                printf("xx : %s\n", LFNBuf);
                              }while((fno.fname[0] == '\0' )|| (fno.fname[0] == '.'));
                             
                        }
                        if (!(fno.fattrib & AM_DIR)) {
                            sprintf(LFNBuf, "//music/%s", fno.lfname[0] ? fno.lfname : fno.fname); 
                            strcpy(list.ptr_next_song, LFNBuf);
                        }
                        
                        printf("list.ptr_next_song %s \n", list.ptr_next_song);
                        printf("list.ptr_current_song %s \n", list.ptr_current_song);
                        printf("list.ptr_previous_song %s \n", list.ptr_previous_song);
                        return 0;
                    }
                }// end   if (!(fno.fattrib & AM_DIR)) {     
                
            }// end for(; ; ) {
        }// end if
        f_closedir(&pdir);
    }   
    return 0;
}

uint8_t back_update_strucre_list()
{
    res = f_mount(&fs, "", 1); // register work area

    if (res == FR_OK)
    {
        FRESULT res;

        static TCHAR lfname[_MAX_LFN]; 
        char LFNBuf[_MAX_LFN];
        
        res = f_opendir(&pdir, "//music"); /* Open the directory */
        if (res == FR_OK)
        {
            printf("Directory listing...\r\n"); 
            for (; ; ) 
            { 
                fno.lfname = lfname; 
                fno.lfsize = _MAX_LFN - 1; 
                res = f_readdir(&pdir, &fno);  /* Read a directory item */ 
                
                if (!(fno.fattrib & AM_DIR)) { /* It is a file. */
                    sprintf(LFNBuf, "//music/%s", fno.lfname[0] ? fno.lfname : fno.fname); 

                    if(!strcmp(LFNBuf, list.ptr_current_song))
                    {
                        for(int k = 0; k < number_of_song-1; k++)
                        {
                            do{
                                res = f_readdir(&pdir, &fno); 
                                sprintf(LFNBuf, "//music/%s", fno.lfname[0] ? fno.lfname : fno.fname); 
                                if (!fno.fname[0] && !lfname[0]) dir_sdi(&pdir, 0);
                                                           
 //                               printf("xx : %s\n", LFNBuf);
                              }while((fno.fname[0] == '\0' )|| (fno.fname[0] == '.'));                            
                        }
                        printf("vao na\n");
                        printf("dau vao hien tai %s\n", LFNBuf);
                        strcpy(list.ptr_next_song, list.ptr_current_song);
                        strcpy(list.ptr_current_song, list.ptr_previous_song);
                        
                        for(int k = 0; k < number_of_song-1; k++)
                        {
                            do{
                                res = f_readdir(&pdir, &fno); 
                                sprintf(LFNBuf, "//music/%s", fno.lfname[0] ? fno.lfname : fno.fname); 
                                if (!fno.fname[0] && !lfname[0]) dir_sdi(&pdir, 0);
                                                           
//                                printf("xx : %s\n", LFNBuf);
                              }while((fno.fname[0] == '\0' )|| (fno.fname[0] == '.'));                            
                        }
                        strcpy(list.ptr_previous_song, LFNBuf);
 //                       sprintf(LFNBuf, "//music/%s", fno.lfname[0] ? fno.lfname : fno.fname); 
                        printf("doc them 2 lan nua %s\n", LFNBuf); 
                        

                        printf("list.ptr_previous_song %s \n", list.ptr_previous_song);
                        printf("list.ptr_current_song %s \n", list.ptr_current_song);                        
                        printf("list.ptr_next_song %s \n", list.ptr_next_song);

                        return 0;
                    }
                }// end   if (!(fno.fattrib & AM_DIR)) {     
                
            }// end for(; ; ) {
        }// end if
        f_closedir(&pdir);
    }
    return 0;
}



int main()
{
    System_Initial();
    SystemCoreClockUpdate();
    SysTick_Init();
    button_Initial();
    UART1_Config();
    DAC_Channel1_Initial();
    DACDMA_Initial(wavBuffer1);
    
    Initial_Structure_List();
    printf("structure %s - %s - %s\n",list.ptr_previous_song, list.ptr_current_song, list.ptr_next_song);
    LCD_Init();

/*timer update LCD_scrollDisplayRight() => chay chu*/
//    while(1)
//    {
//        LCD_Gotoxy(0,0);
//        LCD_Puts("//music/ViYeuCuDamDau.wav"); 
//        delay_ms(500);
//        LCD_scrollDisplayRight();
// 
//    }
    
    while (1)
    {
        if(Flag_next_song == 1)
        {
            Flag_next_song = 0;
            next_update_strucre_list(); 
            LCD_Clear();
            LCD_Gotoxy(0,0);
            LCD_Puts(list.ptr_current_song);  
            File_Processed(list.ptr_current_song);  
        }
        
        if(Flag_back_song == 1)
        {
            Flag_back_song = 0;
            back_update_strucre_list(); 
            LCD_Clear();
            LCD_Gotoxy(0,0);
            LCD_Puts(list.ptr_current_song); 
            File_Processed(list.ptr_current_song);
        }
    } //end loop
} //end main

/******************* (C) COPYRIGHT 2020 *****END OF FILE****/
