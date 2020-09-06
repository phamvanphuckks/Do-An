#ifndef		__LCD16X2_H__
#define		__LCD16X2_H__

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "delay.h"

/*------------- Define LCD Use -----------------*/

/*Note: Comment which not use */

//#define LCD16xN //For lcd16x2 or lcd16x4
#define LCD16xN //For lcd20x4

/*------------- Define For Connection -----------------*/

#define GPIO_PIN_SET    1
#define GPIO_PIN_RESET  0

#define RS_PORT		GPIOE
#define RS_PIN		GPIO_Pin_7

#define EN_PORT		GPIOE
#define EN_PIN		GPIO_Pin_8

#define D7_PORT		GPIOE
#define D7_PIN		GPIO_Pin_15

#define D6_PORT		GPIOE
#define D6_PIN		GPIO_Pin_13

#define D5_PORT		GPIOE
#define D5_PIN		GPIO_Pin_11

#define D4_PORT		GPIOE
#define D4_PIN		GPIO_Pin_9


/*------------ Declaring Private Macro -----------------*/

#define PIN_LOW(PORT,PIN)	GPIO_WriteBit(PORT, PIN, GPIO_PIN_RESET);
#define PIN_HIGH(PORT,PIN)	GPIO_WriteBit(PORT, PIN, GPIO_PIN_SET);

/*------------ Declaring Function Prototype -------------*/
void LCD_Init(void);
void lcd_write(uint8_t type,uint8_t data);
void lcd_puts(uint8_t x, uint8_t y, int8_t *string);
void LCD_Clear(void);
void LCD_Enable(void);
void LCD_Send4Bit(unsigned char Data);
void LCD_SendCommand(unsigned char command);
void LCD_Gotoxy(unsigned char x, unsigned char y);
void LCD_PutChar(unsigned char Data);
void LCD_Puts(char *s);
void LCD_scrollDisplayRight(void);
void LCD_scrollDisplayLeft(void);
void LDC_cursor(void);
void LCD_noCursor(void);
void LCD_autoscroll(void);
void LCD_noAutoscroll(void); 
void LCD_leftToRight(void);
void LCD_rightToLeft(void);

    
static void GPIO_LCD_Init(void);
#endif

