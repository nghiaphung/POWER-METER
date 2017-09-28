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
#include "driver/serial/serial.h"
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
    hwSetup();
    while(1)
    {
        fsm_Update();
        fsm_Run();
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
    /* Configure clock for peripherals */
    hwClockConfig();
    /* Initialize LEDs driver */
    Led_Init();
    /* Initialize STPM33 driver */
    Stpm33_Init();
    /* Initialize Serial drivers */
    Serial_Init(&debug_serial);
    
}
