/*
 * @filename: lcd.c
 * @Author	: nghiaphung
 * @Date	: 24/9/2017
 * @Email	: ducnghia318@gmail.com
 */
 
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "led.h"
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/
#define LED_R_PORT        GPIOB    
#define LED_G_PORT        GPIOB 
#define LED_Y_PORT        GPIOC
#define LED_R_PIN         GPIO_Pin_11
#define LED_G_PIN         GPIO_Pin_10
#define LED_Y_PIN         GPIO_Pin_9

/* Symbol to convert active value */
#define LED_ACTIVE_STAGE  1
/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/

/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/
extern void Led_Init (void)
{
    /* user data configuration */
    GPIO_InitTypeDef GPIO_InitStruct;
    /* data initialization for GPIO */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    /* initialize LED_R */
    GPIO_InitStruct.GPIO_Pin   = LED_R_PIN;
    GPIO_Init(LED_R_PORT, &GPIO_InitStruct);
    /* initialize LED_G */
    GPIO_InitStruct.GPIO_Pin   = LED_G_PIN;
    GPIO_Init(LED_G_PORT, &GPIO_InitStruct);
    /* initialize LED_Y */
    GPIO_InitStruct.GPIO_Pin   = LED_Y_PIN;
    GPIO_Init(LED_Y_PORT, &GPIO_InitStruct);
}

extern void Led_SetLevel (led_channel_t pLed, led_level_t pLevel)
{
    /* Local variables */
    GPIO_TypeDef* GPIOx = ((void*)0); // set GPIOx to NULL;
    uint16_t GPIO_Pin = 0;
    uint8_t vOutput   = 0;
    switch (pLed)
    {
        case LED_G:
            GPIOx    =  LED_G_PORT;
            GPIO_Pin = LED_G_PIN;
            break;
        case LED_R:
            GPIOx    = LED_R_PORT;
            GPIO_Pin = LED_R_PIN;
            break;
        case LED_Y:
            GPIOx    = LED_Y_PORT;
            GPIO_Pin = LED_Y_PIN;
            break;
        default:
            break;
    }
    /* Indicate output level */
    if (LED_LEVEL_ENABLE == pLevel)
    {
        vOutput = LED_ACTIVE_STAGE;
    }
    else
        vOutput = LED_ACTIVE_STAGE ^ 0x01;
    /* set to hardware */
    GPIO_WriteBit(GPIOx, GPIO_Pin,  (BitAction)vOutput); 
}
/******************************************************************************/
/**!                          LOCAL VARIABLES                                 */
/******************************************************************************/

/******************************************************************************/
/**!                    LOCAL FUNCTIONS PROTOTYPES                            */
/******************************************************************************/

/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/

/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/
