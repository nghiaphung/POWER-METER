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
    hwClockConfig();

    hwSetup();
    
    while(1)
    {
        

        Led_SetLevel(LED_G, LED_LEVEL_ENABLE);

        Led_SetLevel(LED_G, LED_LEVEL_DISABLE);
    }
    return 0;
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
    /* Configure clock for peripherals */

    /* Initialize LEDs driver */
    Led_Init();
    /* Initialize STPM33 driver */
    Stpm33_Init();
    /* Initialize Serial drivers */
    Serial_Init(&debug_serial);
    /* Initialized LCD */
    LCD_Init();
    LCD_Puts(0,0,"aaa");
    
}


