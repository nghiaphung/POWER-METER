/*******************************************************************************
 * @filename: lcd.h
 * @Author	: nghiaphung
 * @Date	: 24/9/2017
 * @Email	: ducnghia318@gmail.com
 ******************************************************************************/
#ifndef LCD_H_
#define LCD_H_

/******************************************************************************/
/*                               INCLUDE                                      */
/******************************************************************************/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
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
void LCD_Clear(void);
void LCD_Init (void);
void LCD_Puts(uint8_t x, uint8_t y, char* str);
void LCD_DelayUs (uint32_t pUs);
void LCD_DisplayNum (uint8_t x, uint8_t y, uint32_t pNum);
void delay1 (uint32_t us);
#endif 
/******************************************************************************/
/**!                           END OF FILE                                    */
/******************************************************************************/
