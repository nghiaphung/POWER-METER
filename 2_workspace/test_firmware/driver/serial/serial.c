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
#define SERIAL_DEBUG_TX_PIN          GPIO_Pin_9
#define SERIAL_DEBUG_RX_PIN          GPIO_Pin_10
#define SERIAL_DEBUG_PORT            GPIOA
#define USART_DEBUG                  USART1

#define SERIAL_WIFI_TX_PIN           GPIO_Pin_2
#define SERIAL_WIFI_RX_PIN           GPIO_Pin_3
#define SERIAL_WIFI_PORT             GPIOA
#define USART_WIFI                   USART2

#define SERIAL_PLC_TX_PIN            GPIO_Pin_10
#define SERIAL_PLC_RX_PIN            GPIO_Pin_11
#define SERIAL_PLC_PORT              GPIOC
#define USART_PLC                    USART3
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
serial_callback_t _serial_callback = ((void*)0);
/******************************************************************************/
/**!                    LOCAL FUNCTIONS PROTOTYPES                            */
/******************************************************************************/

/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/
extern void Serial_Init(serial_t* serial)
{
    uint16_t               TX_Pin, Rx_Pin;
    GPIO_TypeDef*          Serial_Port;
    USART_TypeDef*         USARTx;
    GPIO_InitTypeDef       GPIO_InitStruct;
    USART_InitTypeDef      UART_InitStruct;
    NVIC_InitTypeDef       NVIC_InitStruct;
    /* initialize PA9 as TX */
    switch(serial->usart)
    {
        case Serial_Debug:
            TX_Pin      = SERIAL_DEBUG_TX_PIN;
            Rx_Pin      = SERIAL_DEBUG_RX_PIN;
            Serial_Port = SERIAL_DEBUG_PORT;
            USARTx      = USART_DEBUG;
            break;
        case Serial_Wifi:
            TX_Pin      = SERIAL_WIFI_TX_PIN;
            Rx_Pin      = SERIAL_WIFI_RX_PIN;
            Serial_Port = SERIAL_WIFI_PORT;
            USARTx      = USART_WIFI;
            break;
        case Serial_PLC:
            TX_Pin      = SERIAL_PLC_TX_PIN;
            Rx_Pin      = SERIAL_PLC_RX_PIN;
            Serial_Port = SERIAL_PLC_PORT;
            USARTx      = USART_PLC;
            break;
        default: break;
    }
    GPIO_InitStruct.GPIO_Pin     = TX_Pin;
    GPIO_InitStruct.GPIO_Mode    = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(Serial_Port, &GPIO_InitStruct);
    /* initialize PA10 as RX */
    GPIO_InitStruct.GPIO_Pin     = Rx_Pin;
    GPIO_InitStruct.GPIO_Mode    = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(Serial_Port, &GPIO_InitStruct);
    /* initialize UART1 */
    UART_InitStruct.USART_BaudRate             = serial->baudrate;
    UART_InitStruct.USART_WordLength           = USART_WordLength_8b;
    UART_InitStruct.USART_StopBits             = USART_StopBits_1;
    UART_InitStruct.USART_Parity               = USART_Parity_No;
    UART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
    UART_InitStruct.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;
    
    USART_Init(USARTx, & UART_InitStruct);
    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
    USART_Cmd( USARTx, ENABLE);
    NVIC_InitStruct.NVIC_IRQChannel            = USART1_IRQn; //chua tong quat
    NVIC_InitStruct.NVIC_IRQChannelCmd         = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
	//<! Initialize callback function    
    _serial_callback = serial->callback;
    
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

//int Serial_Available(void)
//{
//    return RxBuffer[0];
//}

//uint8_t Serial_ReadRxBuffer(void)
//{
//    uint8_t temp = RxBuffer[0];
//    RxBuffer[0] -= 1;
//    return RxBuffer[temp];
//}
/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/
void USART1_IRQHandler (void)
{
    if (SET == USART_GetFlagStatus(USART1, USART_IT_RXNE))
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        uint8_t byte = (uint8_t) USART_ReceiveData(USART1) & 0xFF;
        //<! Callback for upper layer
        if (_serial_callback)
            _serial_callback(byte);
    }
}
