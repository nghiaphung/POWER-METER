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
typedef struct {
	uint8_t DisplayControl;
	uint8_t DisplayFunction;
	uint8_t DisplayMode;
	uint8_t Rows;
	uint8_t Cols;
	uint8_t currentX;
	uint8_t currentY;
} LCD_Options_t;

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

/* Commands*/
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80

/* Flags for display entry mode */
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

/* Flags for display on/off control */
#define LCD_DISPLAYON           0x04
#define LCD_CURSORON            0x02
#define LCD_BLINKON             0x01

/* Flags for display/cursor shift */
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

/* Flags for function set */
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00
/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/

/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/

/******************************************************************************/
/**!                          LOCAL VARIABLES                                 */
/******************************************************************************/
static LCD_Options_t    LCD_Opts;
/******************************************************************************/
/**!                    LOCAL FUNCTIONS PROTOTYPES                            */
/******************************************************************************/
void LCD_DelayUs (uint32_t pUs);
void LCD_Cmd4bit (uint8_t pCmd);
void LCD_Cmd (uint8_t pCmd);
void LCD_ENBlink (void);
void LCD_GPIOInit (void);
void LCD_DisplayOn (void);
void LCD_SetCursor (uint8_t pRow, uint8_t pCol);
void LCD_Data (uint8_t data);
void delay1 (uint32_t us);
/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/


void LCD_Init (void)
{
    LCD_GPIOInit();
	/* At least 40ms */    
    //LCD_DelayUs(45000);
    delay1(9000);
    /* Set LCD width and height */
    LCD_Opts.Cols = 20;
    LCD_Opts.Rows = 4;
	/* Set cursor pointer to beginning for LCD */
    LCD_Opts.currentX = 0;
    LCD_Opts.currentY = 0;
    
    LCD_Opts.DisplayFunction = LCD_4BITMODE | LCD_5x8DOTS | LCD_2LINE;
    
	/* Try to set 4bit mode */
    LCD_Cmd4bit(0x03);
    //LCD_DelayUs(4500);
    delay1(900);
    LCD_Cmd4bit(0x03);
    //LCD_DelayUs(4500);
    delay1(900);
    
    LCD_Cmd4bit(0x03);
    //LCD_DelayUs(4500);
    delay1(900);
	/* Set 4-bit interface */
    LCD_Cmd4bit(0x02);
    //LCD_DelayUs(100);
    delay1(20);
	/* Set # lines, font size, etc. */
    LCD_Cmd(LCD_FUNCTIONSET | LCD_Opts.DisplayFunction);
 	/* Turn the display on with no cursor or blinking default */
    LCD_Opts.DisplayControl = LCD_DISPLAYON;
    LCD_DisplayOn();
	/* Clear lcd */
    LCD_Clear();
	/* Default font directions */
    LCD_Opts.DisplayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    LCD_Cmd(LCD_ENTRYMODESET | LCD_Opts.DisplayMode);
    //LCD_DelayUs(4500);
    delay1(900);
}

void LCD_Clear(void)
{
    LCD_Cmd(LCD_CLEARDISPLAY);
    //LCD_DelayUs(3000);
    delay1(600);
}

void LCD_Puts(uint8_t x, uint8_t y, char* str)
{
    LCD_SetCursor(x, y);
    while(*str)
    {
        if(LCD_Opts.currentX >= LCD_Opts.Cols)
        {
            LCD_Opts.currentX = 0;
            LCD_Opts.currentY++;
            LCD_SetCursor(LCD_Opts.currentX, LCD_Opts.currentY);
        }
        if (*str == '\n')
        {
            LCD_Opts.currentY++;
            LCD_SetCursor(LCD_Opts.currentX, LCD_Opts.currentY);
        }
        else if (*str == '\r')
        {
            LCD_SetCursor(0, LCD_Opts.currentY);
        }
        else {
            LCD_Data(*str);
            LCD_Opts.currentX++;
        }
        str++;
    }
}
/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/
void LCD_DelayUs (uint32_t pUs)
{
    RCC_ClocksTypeDef           RCC_ClockStatus    = {0};
    
    /* get clock status */
    RCC_GetClocksFreq(&RCC_ClockStatus);
    uint32_t temp;
    temp = (RCC_ClockStatus.SYSCLK_Frequency / (SYS_TIMER_CLK))*pUs;
    temp = SystemCoreClock;
    while(temp--);
}

void LCD_Cmd4bit (uint8_t pCmd)
{
    GPIO_WriteBit(LCD_DB7_PORT, LCD_DB7_PIN, (BitAction)(pCmd & 0x08));
    GPIO_WriteBit(LCD_DB6_PORT, LCD_DB6_PIN, (BitAction)(pCmd & 0x04));
    GPIO_WriteBit(LCD_DB7_PORT, LCD_DB5_PIN, (BitAction)(pCmd & 0x02));
    GPIO_WriteBit(LCD_DB4_PORT, LCD_DB4_PIN, (BitAction)(pCmd & 0x01));
    LCD_ENBlink();
}

void LCD_Cmd (uint8_t pCmd)
{
    /* command mode */
    GPIO_ResetBits(LCD_RS_PORT, LCD_RS_PIN);
    /* high nibble */
    LCD_Cmd4bit(pCmd >> 4);
    /* low nibble */
    LCD_Cmd4bit(pCmd & 0x0F);
}

void LCD_ENBlink (void)
{
    GPIO_SetBits(LCD_EN_PORT, LCD_EN_PIN);
    //LCD_DelayUs(50);
    delay1(10);
    GPIO_ResetBits(LCD_EN_PORT, LCD_EN_PIN);
    //LCD_DelayUs(50);
    delay1(10);
}

void LCD_GPIOInit (void)
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
    /* set all pin to low */
    GPIO_ResetBits(LCD_DB0_PORT, LCD_DB0_PIN);
    GPIO_ResetBits(LCD_DB0_PORT, LCD_DB0_PIN);
    GPIO_ResetBits(LCD_DB0_PORT, LCD_DB0_PIN);
    GPIO_ResetBits(LCD_DB0_PORT, LCD_DB0_PIN);
    GPIO_ResetBits(LCD_DB0_PORT, LCD_DB0_PIN);
    GPIO_ResetBits(LCD_DB0_PORT, LCD_DB0_PIN);
    GPIO_ResetBits(LCD_DB0_PORT, LCD_DB0_PIN);
    GPIO_ResetBits(LCD_DB0_PORT, LCD_DB0_PIN);
    GPIO_ResetBits(LCD_RW_PORT, LCD_RW_PIN);
    GPIO_ResetBits(LCD_RS_PORT, LCD_RS_PIN);
    GPIO_ResetBits(LCD_EN_PORT, LCD_EN_PIN);
}

void LCD_DisplayOn (void)
{
    LCD_Opts.DisplayControl |= LCD_DISPLAYON;
    LCD_Cmd(LCD_DISPLAYCONTROL | LCD_Opts.DisplayControl);
}

void LCD_SetCursor (uint8_t pCol, uint8_t pRow)
{
    uint8_t row_offset[4] = {0x00, 0x40, 0x14, 0x54};
    /* go to beggining */
    if (pRow >= LCD_Opts.Rows)
    {
        pRow = 0;
    }
    /* set current col and row */
    LCD_Opts.currentX = pCol;
    LCD_Opts.currentY = pRow;
    /* set location address */
    LCD_Cmd(LCD_SETDDRAMADDR | (pCol + row_offset[pRow]));
}

void LCD_Data (uint8_t data)
{
    /* data mode */
    GPIO_SetBits(LCD_RS_PORT, LCD_RS_PIN);
    /* high nibble */
    LCD_Cmd4bit(data >> 4);
    /* low nibble */
    LCD_Cmd4bit(data & 0x0F);
}

void delay1 (uint32_t us)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    uint32_t temp;
    temp = 72 *us;
    SysTick->LOAD = temp;
    SysTick->CTRL = 0xFFFFFFF5;
    while(SysTick->VAL);
    SysTick->CTRL = 0xFFFFFFF0;
}

