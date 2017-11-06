/*
 * @filename: clock.c
 * @Author	: nghiaphung
 * @Date	: 24/9/2017
 * @Email	: ducnghia318@gmail.com
 */
 
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "clock.h"
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/

/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/

/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/
extern void Clock_Config (void)
{
    SystemInit();
    /* enable HSE and set clock 72MHZ */
    RCC_HSEConfig(RCC_HSE_ON);
    while (RCC_WaitForHSEStartUp() != SUCCESS);
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    RCC_PLLCmd(ENABLE);
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLK1Config(RCC_HCLK_Div2);
    RCC_PCLK2Config(RCC_HCLK_Div1);
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
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
