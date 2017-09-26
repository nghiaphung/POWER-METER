/*
 * @filename: serial.c
 * @Author	: nghiaphung
 * @Date	: 24/9/2017
 * @Email	: ducnghia318@gmail.com
 */
 
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "serial.h"
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/
#define BR115200		       115200
#define SERIAL_TX_PIN          GPIO_Pin_9
#define SERIAL_RX_PIN          GPIO_Pin_10
#define SERIAL_PORT            GPIOA
/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/

/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/
extern void Serial_Init()
{
    GPIO_InitTypeDef       GPIO_InitStruct;
    USART_InitTypeDef      UART_InitStruct;
    /* initialize PA9 as TX */
    GPIO_InitStruct.GPIO_Pin     = SERIAL_TX_PIN;
    GPIO_InitStruct.GPIO_Mode    = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(SERIAL_PORT, &GPIO_InitStruct);
    /* initialize PA10 as RX */
    GPIO_InitStruct.GPIO_Pin     = SERIAL_TX_PIN;
    GPIO_InitStruct.GPIO_Mode    = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(SERIAL_PORT, &GPIO_InitStruct);
    /* initialize UART1 */
    UART_InitStruct.USART_BaudRate             = BR115200;
    UART_InitStruct.USART_WordLength           = USART_WordLength_8b;
    UART_InitStruct.USART_StopBits             = USART_StopBits_1;
    UART_InitStruct.USART_Parity               = USART_Parity_No;
    UART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
    UART_InitStruct.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;
    
    USART_Init(USART1, & UART_InitStruct);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd( USART1, ENABLE);
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
