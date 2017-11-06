/*******************************************************************************
 ******************************************************************************/
 
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
/* driver includes */
#include "stm32f10x_conf.h"
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
void Init_Hardware(void);

/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/

int main (void)
{
    uint32_t data;

    Clock_Config();
    Init_Hardware();
    Stpm33_WriteRegister(0x05, 0x0080);
    data = Stpm33_ReadRegister(0x04);
    if ((data & 0x00800000) == 0x00800000)
        Led_SetLevel(LED_G, LED_LEVEL_DISABLE);
      
    while(1)
    {   
        fsm_Update();
        fsm_Run();
    }
}

/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/
void Init_Hardware(void)
{
    /* Initialize STPM33 driver */
    Stpm33_Init();
    
    /* Initialize LEDs driver */
    Led_Init();

    /* calib stpm33 to read right value */
    Stpm33_Calib();
    /* Initialize Serial drivers */
    Serial_Init();
    /* Initialized LCD */
    LCD_Init();  
}


