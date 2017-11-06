
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "serial.h"
#include "../../source/fsm.h"
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/
#define SERIAL_TX_PIN          GPIO_Pin_9
#define SERIAL_RX_PIN          GPIO_Pin_10
#define SERIAL_PORT            GPIOA
#define USART                  USART1


#define UART1_TX		GPIO_Pin_9
#define UART1_RX		GPIO_Pin_10
#define UART1_PORT	GPIOA

#define UART3_TX		GPIO_Pin_10
#define UART3_RX		GPIO_Pin_11
#define UART3_PORT	GPIOB

#define UART2_TX		GPIO_Pin_2
#define UART2_RX		GPIO_Pin_3
#define UART2_PORT	GPIOA
/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/

/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/

/******************************************************************************/
/**!                          LOCAL VARIABLES                                 */
/******************************************************************************/
//uint8_t RxBuffer[256];
serial_callback_t Serial_callback = ((void*)0);
uint32_t vDelay = 0;
/******************************************************************************/
/**!                    LOCAL FUNCTIONS PROTOTYPES                            */
/******************************************************************************/

/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/



extern void Serial_Init(void)
{
    uint16_t               TX_Pin, Rx_Pin;
    GPIO_InitTypeDef       GPIO_InitStruct;
    USART_InitTypeDef      UART_InitStruct;
    NVIC_InitTypeDef       NVIC_InitStruct;
    
    /* initialize PA9 as TX */
    GPIO_InitStruct.GPIO_Pin     = SERIAL_TX_PIN;
    GPIO_InitStruct.GPIO_Mode    = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(SERIAL_PORT, &GPIO_InitStruct);
    /* initialize PA10 as RX */
    GPIO_InitStruct.GPIO_Pin     = SERIAL_RX_PIN;
    GPIO_InitStruct.GPIO_Mode    = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(SERIAL_PORT, &GPIO_InitStruct);
    /* initialize UART1 */
    UART_InitStruct.USART_BaudRate             = 115200;
    UART_InitStruct.USART_WordLength           = USART_WordLength_8b;
    UART_InitStruct.USART_StopBits             = USART_StopBits_1;
    UART_InitStruct.USART_Parity               = USART_Parity_No;
    UART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
    UART_InitStruct.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;
    
    USART_Init(USART1, & UART_InitStruct);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd( USART1, ENABLE);
    NVIC_InitStruct.NVIC_IRQChannel            = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd         = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
	//setup callback    
    Serial_callback = Serial_Debug_Update;
}
void Serial_Send(uint8_t* ptr, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
        Serial_SendByte(ptr[i]);
}

void Serial_SendByte(uint8_t byte)
{
    while (SET != USART_GetFlagStatus(USART1, USART_FLAG_TXE));
    USART_SendData(USART1, byte);
}


/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/
void USART1_IRQHandler (void)
{
    if (SET == USART_GetFlagStatus(USART1, USART_IT_RXNE))
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        uint8_t byte = (uint8_t) USART_ReceiveData(USART1) & 0xFF;
        //callback for processing
        if (Serial_callback)
            Serial_callback(byte);
    }
}
