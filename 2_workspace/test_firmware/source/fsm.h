/*******************************************************************************
 ******************************************************************************/
#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_
 
/******************************************************************************/
/*                               INCLUDE                                      */
/******************************************************************************/
#include "string.h"
#include "../driver/serial/serial.h"
#include "../driver/stpm33/stpm33.h"
#include "../driver/led/led.h"
#include "../driver/lcd/lcd.h"
/******************************************************************************/
/*                             PUBLIC TYPEDEF                                 */
/******************************************************************************/

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
void Serial_Debug_Update(uint8_t byte);
void fsm_Run (void);
void fsm_Update (void);
#endif 
/******************************************************************************/
/**!                           END OF FILE                                    */
/******************************************************************************/
