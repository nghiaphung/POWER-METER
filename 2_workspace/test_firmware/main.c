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
#include "driver/led/led.h"
#include "system_stm32f10x.h"
#include "driver/clock/clock.h"
#include "driver/stpm33/stpm33.h"
/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/
int main (int argc, char* argv[])
{
    /* To avoid compiler error/warning */
    int temp = argc + (uint32_t)argv; temp++;
    uint32_t tempp;
    uint8_t a;
    SystemInit();
    hwClockConfig ();
    Led_Init();
    Led_SetLevel(LED_R, LED_LEVEL_ENABLE);
    Led_SetLevel(LED_G, LED_LEVEL_ENABLE);
    Led_SetLevel(LED_Y, LED_LEVEL_ENABLE);
    Stpm33_Init();
    tempp = Stpm33_ReadRegister(0x00);
    a = tempp & 0xFF;
    if (a != 0x04)
        Led_SetLevel(LED_R, LED_LEVEL_DISABLE);
    while(1)
    {}
    
}