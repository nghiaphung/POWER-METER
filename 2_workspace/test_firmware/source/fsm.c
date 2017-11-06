
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "fsm.h"
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/
#define STATE_CMD             0
#define STATE_IDLE            1
#define STATE_ERROR           2
#define STATE_INIT            3

typedef enum {
    TYPE_REQUEST   = 1,
    TYPE_RESPOND   = 2,
    TYPE_PING      = 3,
    TYPE_EMERGENCY = 4,
}frame_type_t;

typedef enum {
    SFD      = 0,
    DEVICEID = SFD + 1,
    SEQ      = DEVICEID + 1,
    TYPE     = SEQ + 2,
    CMD      = TYPE + 1,
    ERR      = CMD + 1,
    PARAM    = ERR + 2,
    CRC16    = PARAM + 18,
}frame_index_t;

typedef enum {
    CMD_SET_ONOFF      = 0x41,
    CMD_GET_ONOFF      = 0x42,
    CMD_GET_UI         = 0x43,
    CMD_GET_PQ         = 0x44,
    CMD_SET_RTC        = 0x49,
}cmd_t;
/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/

/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/

/******************************************************************************/
/**!                          LOCAL VARIABLES                                 */
/******************************************************************************/
uint8_t gState = STATE_INIT;
uint8_t RxBuffer[128];
uint8_t ProcBuffer[28];
static uint16_t vTimeoutCount;
uint8_t Timeout_Run = 0;
static uint32_t LCD_TimeCount = 0;
uint16_t Vol_data[10] = {22243, 22167, 22253, 22253, 22012, 22194, 22214, 22058, 22081, 21949};
uint16_t Cur_data[10] = {210, 202, 205, 204, 204, 205, 204, 201, 203, 206};
uint32_t Power_data[10] = {45793, 45793, 44051, 45560, 45105, 46630, 44031, 45120, 45120, 46368};
uint16_t RePower_data[10] = {0, 0, 1, 0, 0, 0, 1, 0, 0, 0};
uint8_t a = 0;
/******************************************************************************/
/**!                    LOCAL FUNCTIONS PROTOTYPES                            */
/******************************************************************************/
void fsm_SerialCmdProc (void);
void LCD_Update (void);
/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/

void fsm_Update (void)
{
    LCD_Update();
    if (Timeout_Run == 1)
    {
        vTimeoutCount++;
    }

    if (vTimeoutCount > 50000)
    {
        /* stop timeout */
        Timeout_Run = 0;
        vTimeoutCount = 0;
        if (RxBuffer[0] == 28)
        {
            memcpy(&ProcBuffer, &RxBuffer[1], 28);
            /* set state for fsm */
            gState = STATE_CMD;
        }
        else
        {
            gState = STATE_ERROR;
        }
        /* reset buffer */
        RxBuffer[0] = 0;
    }
}
void fsm_Run (void)
{
    switch (gState)
    {
        case STATE_CMD:
            fsm_SerialCmdProc();
            break;
        case STATE_ERROR:
            
            break;
        
        case STATE_IDLE:
            
            break;
        default: break;
    }
}

void Serial_Debug_Update(uint8_t byte)
{
    vTimeoutCount = 0;
    RxBuffer[0] += 1;
    RxBuffer[RxBuffer[0]] = byte;
    /* start timeout counter */
    Timeout_Run = 1;
}
/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/
void fsm_SerialCmdProc (void)
{
    gState = STATE_IDLE;
    uint8_t TxBuffer[28];
    for (int i = 0; i< 28;i++)
        TxBuffer[i]=0;
    uint32_t Data = 0;
    /* prepare buffer */
    TxBuffer[SFD]  = 0x7F;
    TxBuffer[TYPE] = TYPE_RESPOND;
    TxBuffer[DEVICEID] = 0;
    TxBuffer[DEVICEID+1] = 0;
    TxBuffer[CMD] = ProcBuffer[CMD];
    TxBuffer[6] = 0;
    TxBuffer[7] = 0;
    if (ProcBuffer[SFD] != 0x7F)
    {
        TxBuffer[ERR] = 0x01;
    }
    else
    {
        switch (ProcBuffer[CMD])
        {
            case CMD_SET_ONOFF:
                break;
            case CMD_GET_ONOFF:
                break;
            case CMD_GET_UI:
                Data = Vol_data[a];
                TxBuffer[8]   = Data & 0xFF;
                TxBuffer[9] = (Data >> 8) & 0xFF;
                TxBuffer[10] = (Data >> 16) & 0xFF;
            
                Data = Cur_data[a];
                TxBuffer[17]  = Data & 0xFF;
                TxBuffer[18] = (Data >> 8) & 0xFF;
                TxBuffer[19] = (Data >> 16) & 0xFF;   

            
//                Data = Stpm33_ReadVol();
//                Data = Data / 10; // chi lay 2 chu so thap phan
//                TxBuffer[PARAM]   = Data & 0xFF;
//                TxBuffer[PARAM+1] = (Data >> 8) & 0xFF;
//                TxBuffer[PARAM+2] = (Data >> 16) & 0xFF;
//                
//                Data = Stpm33_ReadCur();
//                TxBuffer[PARAM+9]  = Data & 0xFF;
//                TxBuffer[PARAM+10] = (Data >> 8) & 0xFF;
//                TxBuffer[PARAM+11] = (Data >> 16) & 0xFF;
            
  
                break;
            
            case CMD_GET_PQ:
                Data = Power_data[a];
                TxBuffer[8]   = Data & 0xFF;
                TxBuffer[9] = (Data >> 8) & 0xFF;
                TxBuffer[10] = (Data >> 16) & 0xFF;  

                Data = RePower_data[a];
                TxBuffer[17]  = Data & 0xFF;
                TxBuffer[18] = (Data >> 8) & 0xFF;
                TxBuffer[19] = (Data >> 16) & 0xFF;                 
            
//                Data = Stpm33_ReadPowerActive();
//                TxBuffer[PARAM]   = Data & 0xFF;
//                TxBuffer[PARAM+1] = (Data >> 8) & 0xFF;
//                TxBuffer[PARAM+2] = (Data >> 16) & 0xFF;
//                
//                Data = Stpm33_ReadPowerReactive();
//                TxBuffer[PARAM+9]  = Data & 0xFF;
//                TxBuffer[PARAM+10] = (Data >> 8) & 0xFF;
//                TxBuffer[PARAM+11] = (Data >> 16) & 0xFF;                
                break;
            
            case CMD_SET_RTC:
                break;
            default:
                break;
        }
    }
    Serial_Send(TxBuffer, 28);
}

void LCD_Update (void)
{
    uint32_t temp;
    LCD_TimeCount++;
    if (LCD_TimeCount > 650000)
    { 
        LCD_TimeCount = 0;
        
        LCD_Puts(0, 0, "U = ");
        temp = Stpm33_ReadVol();
        LCD_DisplayNum(4,0, temp);
        
        LCD_Puts(0, 1, "I = ");
        temp = Stpm33_ReadCur();
        LCD_DisplayNum(4, 1, temp);
        
        LCD_Puts(0,2, "P=");
        temp = Stpm33_ReadPowerActive();
        LCD_DisplayNum(4,2, temp);
        
//        LCD_DisplayNum(4, 0, (Vol_data[a]/100));
//        LCD_Puts(12, 0, "V");
//        
//        LCD_Puts(0, 1, "I = ");
//        LCD_Puts(4, 1, "0.");
//        LCD_DisplayNum(6, 1, Cur_data[a]);
//        LCD_Puts(12, 1, "A"); 
//        
//        LCD_Puts(0, 2, "P = ");
//        LCD_DisplayNum(4, 2, (Power_data[a]/1000));
//        LCD_Puts(12, 2, "W");

//        LCD_Puts(0, 3, "Q = ");
//        LCD_DisplayNum(4, 3, RePower_data[a]);
//        LCD_Puts(12, 3, "VAr");
        
        
        a++;
        if (a > 9)
            a = 0;
    }
}
