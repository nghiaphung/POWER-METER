/*
 * @filename: StateMachine.c
 * @Author	: nghiaphung
 * @Date	: 27/9/2017
 * @Email	: ducnghia318@gmail.com
 */
 
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
    CMD_GET_COS        = 0x45,
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
uint8_t gState = STATE_IDLE;
uint8_t RxBuffer[128];
uint8_t ProcBuffer[28];
static uint16_t vTimeoutCount;
uint8_t Timeout_Run;
/******************************************************************************/
/**!                    LOCAL FUNCTIONS PROTOTYPES                            */
/******************************************************************************/

/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/
void fsm_Update (void)
{
    if (Timeout_Run == 1)
    {
        vTimeoutCount++;
    }
    if (RxBuffer[0] == 28)
    {
        /* reset buffer */
        RxBuffer[0] = 0;
        /* stop timeout */
        Timeout_Run = 0;
        memcpy(&ProcBuffer, &RxBuffer[1], 28);
        
        /* set state for fsm */
        gState = STATE_CMD;
    }
    if (vTimeoutCount > 5000)
    {
        Timeout_Run = 0;
        gState = STATE_ERROR;
    }
        
        
    
}
void fsm_Run (void)
{
    switch (gState)
    {
        case STATE_CMD:
            Led_SetLevel(LED_G, LED_LEVEL_ENABLE);
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
    if (ProcBuffer[SFD] != 0x7F)
    {
        return;
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
                break;
            case CMD_GET_PQ:
                break;
            case CMD_GET_COS:
                break;
            case CMD_SET_RTC:
                break;
            default:
                break;
        }
    }

}
