/*
 * @filename: lcd.c
 * @Author	: nghiaphung
 * @Date	: 24/9/2017
 * @Email	: ducnghia318@gmail.com	
 */
 
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
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

#define  SYS_TIMER_CLK        (1000000UL)


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
void LCD_SetDB0(void);
void LCD_SetDB1(void);
void LCD_SetDB2(void);
void LCD_SetDB3(void);
void LCD_SetDB4(void);
void LCD_SetDB5(void);
void LCD_SetDB6(void);
void LCD_SetDB7(void);
void LCD_ResetDB0 (void);
void LCD_ResetDB1 (void);
void LCD_ResetDB2 (void);
void LCD_ResetDB3 (void);
void LCD_ResetDB4 (void);
void LCD_ResetDB5 (void);
void LCD_ResetDB6 (void);
void LCD_ResetDB7 (void);
void LCD_SetEN (void);
void LCD_ResetEN (void);
void LCD_SetRW (void);
void LCD_ResetRW (void);
void LCD_SetRS (void);
void LCD_ResetRS (void);
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
void LCD_DelayUs (uint32_t pUs)
{
    RCC_ClocksTypeDef           RCC_ClockStatus    = {0};
    
    /* get clock status */
    RCC_GetClocksFreq(&RCC_ClockStatus);
    pUs = RCC_ClockStatus.SYSCLK_Frequency / (SYS_TIMER_CLK*3);
    for (uint32_t i; i < pUs; i++);
}

void LCD_SetDB0(void)
{
    GPIO_WriteBit(LCD_DB0_PORT, LCD_DB0_PIN, Bit_SET);
}

void LCD_SetDB1(void)
{
    GPIO_WriteBit(LCD_DB1_PORT, LCD_DB1_PIN, Bit_SET);
}

void LCD_SetDB2(void)
{
    GPIO_WriteBit(LCD_DB2_PORT, LCD_DB2_PIN, Bit_SET);
}

void LCD_SetDB3(void)
{
    GPIO_WriteBit(LCD_DB3_PORT, LCD_DB3_PIN, Bit_SET);
}

void LCD_SetDB4(void)
{
    GPIO_WriteBit(LCD_DB4_PORT, LCD_DB4_PIN, Bit_SET);
}

void LCD_SetDB5(void)
{
    GPIO_WriteBit(LCD_DB5_PORT, LCD_DB5_PIN, Bit_SET);
}

void LCD_SetDB6(void)
{
    GPIO_WriteBit(LCD_DB6_PORT, LCD_DB6_PIN, Bit_SET);
}

void LCD_SetDB7(void)
{
    GPIO_WriteBit(LCD_DB7_PORT, LCD_DB7_PIN, Bit_SET);
}

void LCD_ResetDB0 (void)
{
    GPIO_WriteBit(LCD_DB0_PORT, LCD_DB0_PIN, Bit_RESET);
}

void LCD_ResetDB1 (void)
{
    GPIO_WriteBit(LCD_DB1_PORT, LCD_DB1_PIN, Bit_RESET);
}

void LCD_ResetDB2 (void)
{
    GPIO_WriteBit(LCD_DB2_PORT, LCD_DB2_PIN, Bit_RESET);
}

void LCD_ResetDB3 (void)
{
    GPIO_WriteBit(LCD_DB3_PORT, LCD_DB3_PIN, Bit_RESET);
}

void LCD_ResetDB4 (void)
{
    GPIO_WriteBit(LCD_DB4_PORT, LCD_DB4_PIN, Bit_RESET);
}

void LCD_ResetDB5 (void)
{
    GPIO_WriteBit(LCD_DB5_PORT, LCD_DB5_PIN, Bit_RESET);
}

void LCD_ResetDB6 (void)
{
    GPIO_WriteBit(LCD_DB6_PORT, LCD_DB6_PIN, Bit_RESET);
}

void LCD_ResetDB7 (void)
{
    GPIO_WriteBit(LCD_DB7_PORT, LCD_DB7_PIN, Bit_RESET);
}

void LCD_SetEN (void)
{
    GPIO_WriteBit(LCD_EN_PORT, LCD_EN_PIN, Bit_SET);
}

void LCD_ResetEN (void)
{
    GPIO_WriteBit(LCD_EN_PORT, LCD_EN_PIN, Bit_RESET);
}

void LCD_SetRW (void)
{
    GPIO_WriteBit(LCD_RW_PORT, LCD_RW_PIN, Bit_SET);
}

void LCD_ResetRW (void)
{
    GPIO_WriteBit (LCD_RW_PORT, LCD_RW_PIN, Bit_RESET);
}

void LCD_SetRS (void)
{
    GPIO_WriteBit(LCD_RS_PORT, LCD_RS_PIN, Bit_SET);
}

void LCD_ResetRS (void)
{
    GPIO_WriteBit(LCD_RS_PORT, LCD_RS_PIN, Bit_RESET);
}
