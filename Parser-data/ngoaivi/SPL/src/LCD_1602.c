#include "LCD_1602.h"
/*!
 * @brief           Configure the communication pins with the LCD
 */
const char hump[]= {4, 4, 10, 10, 17, 17, 17, 14 };//hump
const char temp[]= {4, 10, 10, 10, 14, 31, 31, 14 };// temp
const char Temp_C[]= {12, 18, 18, 12, 0, 0, 0, 0 };
/*!
 * @brief    function configuration pin lcd is pin output
 */

static void GPIO_PinLCD_Config(void)
{
    GPIO_InitTypeDef        GPIO_PinLCD_InitStruction;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_PinLCD_InitStruction.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_PinLCD_InitStruction.GPIO_OType = GPIO_OType_PP;
    GPIO_PinLCD_InitStruction.GPIO_Pin = LCD_RS|LCD_RW|LCD_EN|LCD_D4|LCD_D5|LCD_D6|LCD_D7;
    GPIO_PinLCD_InitStruction.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_PinLCD_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_PinLCD_InitStruction);
}
/*!
 * @brief    function enable lcd
 */
static void LCD_Enable(void)
{
    GPIO_SetBits(GPIOA, LCD_EN);
    delay_us(3);
    GPIO_ResetBits(GPIOA, LCD_EN);
    delay_us(50);
}
/*!
 * @brief         function  Send 4 bit to lcd
 * @param  [IN]unsigned char Data      data send to lcd
 */
static void LCD_Send4Bit(unsigned char Data)
{
    GPIO_WriteBit(GPIOA, LCD_D4, Data & 0x01);
    GPIO_WriteBit(GPIOA, LCD_D5, (Data>>1)&1);
    GPIO_WriteBit(GPIOA, LCD_D6, (Data>>2)&1);
    GPIO_WriteBit(GPIOA, LCD_D7, (Data>>3)&1);
}
/*!
 * @brief          function command to lcd
 * @param  [IN]nsigned char command      read datasheet lcd
 */
static void LCD_SendCommand(unsigned char command)
{
    /** send 4 bits high */
    LCD_Send4Bit(command >>4);
    /**enable lcd*/
    LCD_Enable();
    /** send 4 bits high low*/
    LCD_Send4Bit(command);
    /**enable lcd*/
    LCD_Enable();
}
/*!
 * @brief          function clear lcd
 */
void LCD_Clear(void)
{
    LCD_SendCommand(0x01);
    delay_us(10);
}
/*!
 * @brief          function init lcd
 */
void LCD_Init(void)
{
    GPIO_PinLCD_Config();
    LCD_Send4Bit(0x00);
    delay_ms(20);
    GPIO_WriteBit(GPIOA, LCD_RS, 0);
    GPIO_WriteBit(GPIOA, LCD_RW, 0);
    LCD_Send4Bit(0x03);
    LCD_Enable();
    delay_ms(5);
    LCD_Enable();
    delay_us(100);
    LCD_Enable();
    LCD_Send4Bit(0x02);
    LCD_Enable();
    LCD_SendCommand( 0x28 );
    LCD_SendCommand( 0x0c);
    LCD_SendCommand( 0x06 );
    LCD_SendCommand(0x01);
}

void LCD_Gotoxy(unsigned char x, unsigned char y)
{
    unsigned char address;
    if(!y)address=(0x80+x);
    else address=(0xc0+x);
    delay_us(1000);
    LCD_SendCommand(address);
    delay_us(50);
}
/*!
 * @brief          function sen char on the lcd
 * @param  [IN]unsigned char Data     data is character want show on the lcd value ascii
 */
static void LCD_PutChar(unsigned char Data)
{
    GPIO_WriteBit(GPIOA, LCD_RS, 1);
    LCD_SendCommand(Data);
    GPIO_WriteBit(GPIOA, LCD_RS, 0);
}

void LCD_Puts (char *s)
{
    while (*s)
    {
        LCD_PutChar(*s);
        s++;
    }
}
void LCD_Show_IMGTemp(void)
{
    uint8_t i;
    LCD_SendCommand(0x40);
    for(i = 0; i <= 7; i++)
    {
        LCD_PutChar(temp[i]);
    }
     LCD_SendCommand(0x80);
     LCD_PutChar(0x00);
}

void LCD_Show_IMGHump(void)
{
    uint8_t i;
    LCD_SendCommand(0x40+8);
    for(i = 0; i <= 7; i++)
    {
        LCD_PutChar(hump[i]);
    }
     LCD_SendCommand(0x87);
     LCD_PutChar(0x01);
}

void LCD_Show_Temp_C(void)
{
    uint8_t i;
    LCD_SendCommand(0x40+16);
    for(i = 0; i <= 7; i++)
    {
        LCD_PutChar(Temp_C[i]);
    }
     LCD_SendCommand(0x80+4);
     LCD_PutChar(0x02);
}
/******************* (C) COPYRIGHT 2020 nguyendonbg@gmail.com*****END OF FILE****/
