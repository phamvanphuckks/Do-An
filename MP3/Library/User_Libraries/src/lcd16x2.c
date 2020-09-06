#include "lcd16x2.h"

/*--------------- Initialize LCD ------------------*/
void LCD_Init(void)
{
  GPIO_LCD_Init();
    
  LCD_Send4Bit(0x00);
  GPIO_WriteBit(RS_PORT, RS_PIN, GPIO_PIN_RESET);
  LCD_Send4Bit(0x03);
  LCD_Enable();
  LCD_Enable();
  LCD_Enable();
  LCD_Send4Bit(0x02);
  LCD_Enable();
  LCD_SendCommand(0x28); // giao thuc 4 bit, hien thi 2 hang, ki tu 5x8
  LCD_SendCommand(0x0C); // cho phep hien thi man hinh
  LCD_SendCommand(0x06); // tang ID, khong dich khung hinh
  LCD_SendCommand(0x01); // xoa toan bo khung hinh
}
/*--------------- enable To LCD ---------------*/
void LCD_Enable()
{
    GPIO_WriteBit(EN_PORT, EN_PIN, GPIO_PIN_SET);
    delay_ms(1);
    GPIO_WriteBit(EN_PORT, EN_PIN, GPIO_PIN_RESET);  
    delay_ms(1); 
}
/*--------------- LCD Send 4Bit ---------------*/
void LCD_Send4Bit(unsigned char Data)
{
    GPIO_WriteBit(D4_PORT,D4_PIN,Data&0x01);
    GPIO_WriteBit(D5_PORT,D5_PIN,(Data>>1)&0x01);
    GPIO_WriteBit(D6_PORT,D6_PIN,(Data>>2)&0x01);
    GPIO_WriteBit(D7_PORT,D7_PIN,(Data>>3)&0x01); 
}
/*--------------- LCD Send 8Bit ---------------*/
void LCD_SendCommand(unsigned char command)
{
  LCD_Send4Bit(command >>4);/* Gui 4 bit cao */
  LCD_Enable();
  LCD_Send4Bit(command);  /* Gui 4 bit thap*/
  LCD_Enable();
}
/*--------------- Write To LCD ---------------*/
void lcd_write(uint8_t type,uint8_t data)
{
	delay_ms(2);
	if(type)
	{
		PIN_HIGH(RS_PORT,RS_PIN);
	}else
	{
		PIN_LOW(RS_PORT,RS_PIN);
	}
	
	//Send High Nibble
	if(data&0x80)
	{
		PIN_HIGH(D7_PORT,D7_PIN);
	}else
	{
		PIN_LOW(D7_PORT,D7_PIN);
	}
	
	if(data&0x40)
	{
		PIN_HIGH(D6_PORT,D6_PIN);
	}else
	{
		PIN_LOW(D6_PORT,D6_PIN);
	}
	
	if(data&0x20)
	{
		PIN_HIGH(D5_PORT,D5_PIN);
	}else
	{
		PIN_LOW(D5_PORT,D5_PIN);
	}
	
	if(data&0x10)
	{
		PIN_HIGH(D4_PORT,D4_PIN);
	}else
	{
		PIN_LOW(D4_PORT,D4_PIN);
	}
	PIN_HIGH(EN_PORT,EN_PIN);
	PIN_LOW(EN_PORT,EN_PIN);
	

	//Send Low Nibble
	if(data&0x08)
	{
		PIN_HIGH(D7_PORT,D7_PIN);
	}else
	{
		PIN_LOW(D7_PORT,D7_PIN);
	}
	
	if(data&0x04)
	{
		PIN_HIGH(D6_PORT,D6_PIN);
	}else
	{
		PIN_LOW(D6_PORT,D6_PIN);
	}
	
	if(data&0x02)
	{
		PIN_HIGH(D5_PORT,D5_PIN);
	}else
	{
		PIN_LOW(D5_PORT,D5_PIN);
	}
	
	if(data&0x01)
	{
		PIN_HIGH(D4_PORT,D4_PIN);
	}else
	{
		PIN_LOW(D4_PORT,D4_PIN);
	}
	PIN_HIGH(EN_PORT,EN_PIN);
	PIN_LOW(EN_PORT,EN_PIN);
}

void lcd_puts(uint8_t x, uint8_t y, int8_t *string)
{
	//Set Cursor Position
	#ifdef LCD16xN	//For LCD16x2 or LCD16x4
	switch(x)
	{
		case 0: //Row 0
			lcd_write(0,0x80+0x00+y);
			break;
		case 1: //Row 1
			lcd_write(0,0x80+0x40+y);
			break;
		case 2: //Row 2
			lcd_write(0,0x80+0x10+y);
			break;
		case 3: //Row 3
			lcd_write(0,0x80+0x50+y);
			break;
	}
	#endif
	
	#ifdef LCD20xN	//For LCD20x4
	switch(x)
	{
		case 0: //Row 0
			lcd_write(0,0x80+0x00+y);
			break;
		case 1: //Row 1
			lcd_write(0,0x80+0x40+y);
			break;
		case 2: //Row 2
			lcd_write(0,0x80+0x14+y);
			break;
		case 3: //Row 3
			lcd_write(0,0x80+0x54+y);
			break;
	}
	#endif
	
	while(*string)
	{
		lcd_write(1,*string);
		string++;
	}
}



void LCD_Gotoxy(unsigned char x, unsigned char y)
{
    unsigned char address;
    if(!y)address=(0x80+x);
    else address=(0xC0+x);
    LCD_SendCommand(address);

}

void LCD_PutChar(unsigned char Data)
{
  GPIO_WriteBit(RS_PORT, RS_PIN, GPIO_PIN_SET);
  LCD_SendCommand(Data);
  GPIO_WriteBit(RS_PORT, RS_PIN, GPIO_PIN_RESET);
}

void LCD_Puts(char *s)
{
    while (*s){
        LCD_PutChar(*s);
      s++;
    }
}

void LCD_Clear(void)
{
    LCD_SendCommand(0x01);  
    delay_ms(1); 
}

void LCD_scrollDisplayRight(void)
{
  for (int positionCounter = 0; positionCounter < 7; positionCounter++) {
    LCD_SendCommand(0x1C);  
    delay_ms(300);
  }    
}
void LCD_scrollDisplayLeft(void)
{
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    LCD_SendCommand(0x18);  
    delay_ms(100);
  }    
}

void LDC_cursor(void)
{   
    LCD_SendCommand(0x0E);  
    delay_ms(1); 
}
void LCD_noCursor(void)
{
    LCD_SendCommand(0x0C);  
    delay_ms(1);     
}

void LCD_autoscroll(void)
{
    LCD_SendCommand(0x01|0x04);  
    delay_ms(1);     
}

void LCD_noAutoscroll(void)
{
    LCD_SendCommand((~0x01)|0x04); 
    delay_ms(1);     
}

void LCD_leftToRight(void)
{
    LCD_SendCommand(0x02|0x04); 
    delay_ms(1);     
}

void LCD_rightToLeft(void)
{
    LCD_SendCommand(0x00|0x04); 
    delay_ms(1); 
}

void GPIO_LCD_Init(void)
{
    GPIO_InitTypeDef   LCD_Initstructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);  

    /*Configure GPIO pin Output Level */
    GPIO_WriteBit(GPIOE, RS_PIN|EN_PIN|D7_PIN|D6_PIN|D5_PIN|D4_PIN, GPIO_PIN_RESET);        

    LCD_Initstructure.GPIO_Pin = RS_PIN|EN_PIN|D7_PIN|D6_PIN|D5_PIN|D4_PIN;
    LCD_Initstructure.GPIO_Mode = GPIO_Mode_OUT;
    LCD_Initstructure.GPIO_PuPd = GPIO_PuPd_UP;
    LCD_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &LCD_Initstructure); 

}
