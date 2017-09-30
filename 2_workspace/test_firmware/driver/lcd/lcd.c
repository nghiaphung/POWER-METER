/*
 * @filename: lcd.c
 * @Author	: nghiaphung
 * @Date	: 24/9/2017
 * @Email	: ducnghia318@gmail.com	
 */
 
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "lcd.h"
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/
#define LCD_DB0_PIN           GPIO_Pin_1
#define LCD_DB1_PIN           GPIO_Pin_2
#define LCD_DB2_PIN           GPIO_Pin_3
#define LCD_DB3_PIN           GPIO_Pin_4
#define LCD_DB4_PIN           GPIO_Pin_5
#define LCD_DB5_PIN           GPIO_Pin_6
#define LCD_DB6_PIN           GPIO_Pin_7
#define LCD_DB7_PIN           GPIO_Pin_13

#define LCD_RS_PIN            GPIO_Pin_12
#define LCD_EN_PIN            GPIO_Pin_5
#define LCD_RW_PIN            GPIO_Pin_6

#define LCD_DB0_PORT          GPIOC
#define LCD_DB1_PORT          GPIOC
#define LCD_DB2_PORT          GPIOC
#define LCD_DB3_PORT          GPIOC
#define LCD_DB4_PORT          GPIOC
#define LCD_DB5_PORT          GPIOC
#define LCD_DB6_PORT          GPIOC
#define LCD_DB7_PORT          GPIOC

#define LCD_RS_PORT           GPIOA
#define LCD_EN_PORT           GPIOB
#define LCD_RW_PORT           GPIOB
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

/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/
void LCD_Init (void)
{
    GPIO_InitTypeDef      GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode    = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed   = GPIO_Speed_50MHz;
    /* initialize DB0 */
    GPIO_InitStruct.GPIO_Pin     = LCD_DB0_PIN;
    GPIO_Init(LCD_DB0_PORT, &GPIO_InitStruct);
    /* initialize DB1 */
    GPIO_InitStruct.GPIO_Pin     = LCD_DB1_PIN;
    GPIO_Init(LCD_DB1_PORT, &GPIO_InitStruct);
    /* initialize DB2 */
    GPIO_InitStruct.GPIO_Pin     = LCD_DB2_PIN;
    GPIO_Init(LCD_DB2_PORT, &GPIO_InitStruct);
    /* initialize DB3 */
    GPIO_InitStruct.GPIO_Pin     = LCD_DB3_PIN;
    GPIO_Init(LCD_DB3_PORT, &GPIO_InitStruct);
    /* initialize DB4 */
    GPIO_InitStruct.GPIO_Pin     = LCD_DB4_PIN;
    GPIO_Init(LCD_DB4_PORT, &GPIO_InitStruct);
    /* initialize DB5 */
    GPIO_InitStruct.GPIO_Pin     = LCD_DB5_PIN;
    GPIO_Init(LCD_DB5_PORT, &GPIO_InitStruct);
    /* initialize DB6 */
    GPIO_InitStruct.GPIO_Pin     = LCD_DB6_PIN;
    GPIO_Init(LCD_DB6_PORT, &GPIO_InitStruct);
    /* initialize DB7 */
    GPIO_InitStruct.GPIO_Pin     = LCD_DB7_PIN;
    GPIO_Init(LCD_DB7_PORT, &GPIO_InitStruct);
    /* initialize RS */
    GPIO_InitStruct.GPIO_Pin     = LCD_RS_PIN;
    GPIO_Init(LCD_RS_PORT, &GPIO_InitStruct);
    /* initialize RW */
    GPIO_InitStruct.GPIO_Pin     = LCD_RW_PIN;
    GPIO_Init(LCD_RW_PORT, &GPIO_InitStruct);
    /* initialize EN */
    GPIO_InitStruct.GPIO_Pin     = LCD_EN_PIN;
    GPIO_Init(LCD_EN_PORT, &GPIO_InitStruct);
    
}
/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/