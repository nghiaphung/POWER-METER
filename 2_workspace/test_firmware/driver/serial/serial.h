/*******************************************************************************
 ******************************************************************************/
#ifndef SERIAL_H_
#define SERIAL_H_

/******************************************************************************/
/*                               INCLUDE                                      */
/******************************************************************************/
#include "stm32f10x.h"
/******************************************************************************/
/*                             PUBLIC TYPEDEF                                 */
/******************************************************************************/

typedef void (*serial_callback_t)(uint8_t byte);


/******************************************************************************/
/**!                            PUBLIC SYMBOLS                                */
/******************************************************************************/

/******************************************************************************/
/**!                          PUBLIC VARIABLES                                */
/******************************************************************************/

/******************************************************************************/
/**!                          INLINE FUNCTIONS                                */
/******************************************************************************/

/******************************************************************************/
/**!                    PUBLIC FUNCTIONS PROTOTYPES                           */
/******************************************************************************/
void Serial_Init(void);
void Serial_SendByte(uint8_t byte);
void Serial_Send(uint8_t* ptr, int len);
int Serial_Available(void);
uint8_t Serial_ReadRxBuffer(void);
#endif 
/******************************************************************************/
/**!                           END OF FILE                                    */
/******************************************************************************/
