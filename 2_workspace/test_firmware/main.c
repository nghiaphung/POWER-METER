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
/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/
int main (int argc, char* argv[])
{
    /* To avoid compiler error/warning */
    int temp = argc + (uint32_t)argv; temp++;
    SystemInit();
    hwClockConfig ();
    Led_Init();
    Led_SetLevel(LED_R, LED_LEVEL_ENABLE);
    
    while(1)
    {}
    
}