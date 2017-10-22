/*******************************************************************************
 * @filename: ir.h
 * @Author	: nghiaphung
 * @Date	: 24/9/2017
 * @Email	: ducnghia318@gmail.com
 ******************************************************************************/
 
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
/* driver includes */
#include "stm32f10x_conf.h"
//#include "stm32f10x_map.h"
#include "stm32f10x_rcc.h"
#include "driver/led/led.h"
#include "system_stm32f10x.h"
#include "driver/clock/clock.h"
#include "driver/stpm33/stpm33.h"
#include "driver/serial/serial.h"
#include "driver/lcd/lcd.h"
#include "source/fsm.h"

/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/

/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/

/******************************************************************************/
/**!                          LOCAL VARIABLES                                 */
/******************************************************************************/

/******************************************************************************/
/**!                    LOCAL FUNCTIONS PROTOTYPES                            */
/******************************************************************************/
void hwSetup(void);

/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/

int main (void)
{
    uint32_t data;
    uint32_t count = 0;
    hwClockConfig();

    hwSetup();

    data = Stpm33_ReadRegister(0x18);
    Stpm33_WriteRegister(0x05, 0x0080);
    data = Stpm33_ReadRegister(0x04);

    data = Stpm33_ReadRegister(0x48);
    data = Stpm33_ReadPowerActive();
    Serial_SendByte((uint8_t)(data&0xFF));
    Serial_SendByte((uint8_t)((data >> 8) &0xFF));
    Serial_SendByte((uint8_t)((data >> 16) &0xFF));
    Serial_SendByte((uint8_t)((data >> 24) &0xFF));       
    while(1)
    {   
        for (int i=0; i < 4000000; i++);
        LCD_Clear();
        data = Stpm33_ReadVol();
        LCD_Puts(0,0, "U= ");
        LCD_DisplayNum(4, 0, data);
        data = Stpm33_ReadCur();
        LCD_Puts(0, 1, "I= ");
        LCD_DisplayNum(4,1, data);
        data = Stpm33_ReadPowerActive();
        LCD_Puts(0, 2, "P= ");
        LCD_DisplayNum(4, 2, data);
        data = Stpm33_ReadPowerReactive();
        LCD_Puts(0, 3, "Q= ");
        LCD_DisplayNum(4, 3, data);
        count++;
     
    }
}

/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/
void hwSetup(void)
{
    serial_t debug_serial;    
    debug_serial.baudrate = SERIAL_BAUDRATE_115200;
    debug_serial.callback = Serial_Debug_Update;
    debug_serial.usart    = Serial_Debug;
    
    SystemInit();
    RCC_HSEConfig(RCC_HSE_ON);
    while (RCC_WaitForHSEStartUp() != SUCCESS);
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    RCC_PLLCmd(ENABLE);
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLK1Config(RCC_HCLK_Div2);
    RCC_PCLK2Config(RCC_HCLK_Div1);
    
    /* Configure clock for peripherals */

    /* Initialize LEDs driver */
    Led_Init();
    /* Initialize STPM33 driver */
    Stpm33_Init();
    Stpm33_Calib();
    /* Initialize Serial drivers */
    Serial_Init(&debug_serial);
    /* Initialized LCD */
    LCD_Init();


    
}


