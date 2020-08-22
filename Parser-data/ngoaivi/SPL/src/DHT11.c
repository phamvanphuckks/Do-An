#include "DHT11.h"
int    h,t;
/*!
 * @brief           function configuration pin connect sensor is pin output
 */
static void output(void){

    GPIO_InitTypeDef        GPIO_PinLCD_InitStruction;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_PinLCD_InitStruction.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_PinLCD_InitStruction.GPIO_OType = GPIO_OType_PP;
    GPIO_PinLCD_InitStruction.GPIO_Pin = pin_dht;
    GPIO_PinLCD_InitStruction.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_PinLCD_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_PinLCD_InitStruction);
}
/*!
 * @brief           function configuration pin connect sensor is pin input
 */
static void input(void){
    GPIO_InitTypeDef        GPIO_PinLCD_InitStruction;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_PinLCD_InitStruction.GPIO_Mode = GPIO_Mode_IN;
    GPIO_PinLCD_InitStruction.GPIO_OType = GPIO_OType_PP;
    GPIO_PinLCD_InitStruction.GPIO_Pin = pin_dht;
    GPIO_PinLCD_InitStruction.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_PinLCD_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_PinLCD_InitStruction);
}

void read_dht11_dat(void)
{
    int dht11_dat[5] = { 0, 0, 0, 0, 0 };

    uint8_t laststate    = 1;
    uint8_t counter        = 0;
    uint8_t j= 0, i= 0;
     /** fahrenheit */
    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

    /** pull pin down for 18 milliseconds */
    output();
    GPIO_ResetBits(Port_dht,pin_dht);
    delay_ms( 20 );
    /** then pull it up for 40 microseconds */
    GPIO_SetBits(Port_dht,pin_dht);
    delay_us(40 );
    /** prepare to read the pin */
    input();

    /** detect change and read data */
    for ( i = 0; i < MAXTIMINGS; i++ )
    {
        counter = 0;
        while ( GPIO_ReadInputDataBit(Port_dht,pin_dht) == laststate )
        {
            counter++;
            delay_us( 1 );
            if ( counter == 255 )
            {
                break;
            }
        }
        laststate = GPIO_ReadInputDataBit(Port_dht,pin_dht);

        if ( counter == 255 )
            break;

        /** ignore first 3 transitions */
        if ( (i >= 4) && (i % 2 == 0) )
        {
            /** shove each bit into the storage bytes */
            dht11_dat[j / 8] <<= 1;
            if ( counter > 16 )
                dht11_dat[j / 8] |= 1;
            j++;
        }
    }

    /*
     * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
     * print it out if data is good
     */
    if ( (j >= 40) &&
         (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3])& 0xFF) ) )
    {
        h = dht11_dat[0];
        t = dht11_dat[2];
    }else  {

    }
}
/******************* (C) COPYRIGHT 2020 nguyendonbg@gmail.com*****END OF FILE****/
