/*
 * @filename: timer.c
 * @Author	: 
 * @Date	: 30/9/2017
 * @Email	: 
 */
 
/******************************************************************************/
/**!                               INCLUDE                                    */
/******************************************************************************/
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "timer.h"
/******************************************************************************/
/**!                            LOCAL TYPEDEF                                 */
/******************************************************************************/
/**
 * Software timer
 */
typedef struct
{
    uint32_t Interval;
    uint32_t Timeout;
    uint8_t  Started;
    void     (*callback)(void*);
    void*    Param;
}SWTimer_t;

#define  SYS_TIMER_UNIT              TIM4
#define  SYS_TIMER_CLK               (1000000UL)
#define  SYS_TIMER_IRQ               TIM4_IRQn
#define  SYS_TIMER_MAX_SW_TIMER      10
#define  NULL                        ((void*)0)
/******************************************************************************/
/**!                            LOCAL SYMBOLS                                 */
/******************************************************************************/

/******************************************************************************/
/**!                         EXPORTED VARIABLES                               */
/******************************************************************************/

/******************************************************************************/
/**!                          LOCAL VARIABLES                                 */
/******************************************************************************/
static volatile uint32_t _xSystemTimerMsCounter = 0;
static volatile uint8_t  _xSystemSWTimersNum = 0;
static SWTimer_t         _xTimers[SYS_TIMER_MAX_SW_TIMER] = {{0}};
/******************************************************************************/
/**!                    LOCAL FUNCTIONS PROTOTYPES                            */
/******************************************************************************/

/******************************************************************************/
/**!                        EXPORTED FUNCTIONS                                */
/******************************************************************************/
void SysTimer_Init (void)
{
    TIM_TypeDef*                TIMx = NULL;   
    uint16_t                    TIM_Prescaler = 0;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStruct ={0};
    NVIC_InitTypeDef            NVIC_InitStruct    = {0};
    RCC_ClocksTypeDef           RCC_ClockStatus    = {0};
    
    /* get clock status */
    RCC_GetClocksFreq(&RCC_ClockStatus);
    /* Calculate pre-scaler */
    TIM_Prescaler = (uint16_t)((RCC_ClockStatus.PCLK1_Frequency)/SYS_TIMER_CLK);
    /* setup timer's param */
    TIMx  = SYS_TIMER_UNIT;
    TIM_TimeBaseInitStruct.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period            = 1000;   // 1000us
    TIM_TimeBaseInitStruct.TIM_Prescaler         = TIM_Prescaler;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct);
    TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
    /*  initialize NVIC */
    NVIC_InitStruct.NVIC_IRQChannel             = SYS_TIMER_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelCmd          = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority  = 2;
    NVIC_Init(&NVIC_InitStruct);
    /* reset all variable to default */
    _xSystemTimerMsCounter = 0;
    _xSystemSWTimersNum    = 0;
}



/**
 * @brief Create software timer unit
 * @param xInterval
 * @param Callback
 * @param xParam
 * @return
 *          -1: fail to create software timer
 *          0..SYS_TIMER_NUM_SW_TIMER_MAX: success
 */
int Timer_CreateSWTimer (uint32_t pInterval, void (*callback)(void*), void* pParam)
{
    if ((0 == pInterval) || (NULL == callback))
        return -1;
    int i = 0;
    for (i = 0; i < SYS_TIMER_MAX_SW_TIMER; i++)
    {
        if (_xTimers[i].callback == NULL)
        {
            _xTimers[i].Interval  = pInterval;
            _xTimers[i].Timeout   = 0;
            _xTimers[i].Param     = pParam;
            _xTimers[i].callback  = callback;
            _xSystemSWTimersNum++;
            return i;
        }
    }
    return -1;
}

/**
 * @brief Delete indicated software timer
 * @param pTimerID
 * @return
 */

int Timer_DeleteSWTimer (int pTimerID)
{
    if ((pTimerID >= SYS_TIMER_MAX_SW_TIMER) || (pTimerID < 0))
        return -1;
    if (_xTimers[pTimerID].callback != NULL)
    {
        _xTimers[pTimerID].callback = NULL;
        _xTimers[pTimerID].Param    = NULL;
    }
    return 0;
}

/**
 * @brief Start indicated software timer
 * @param xTimerId
 * @return
 *          0: success
 *          -1 : fail
 */
int Tiemr_RunSWTimer (int pTimerID)
{
    if ((pTimerID >= SYS_TIMER_MAX_SW_TIMER) || (pTimerID < 0))
        return -1;
    if (_xTimers[pTimerID].callback != NULL)
    {
        /* Get current time-stamp */        
        _xTimers[pTimerID].Timeout = Timer_GetSysMsTimer();
        /* Calculate timeout point */        
        _xTimers[pTimerID].Timeout += _xTimers[pTimerID].Interval;
        /* Indicate software timer has been started */        
        _xTimers[pTimerID].Started = 1;
    }
    return 0;
}

/**
 * @brief To stop indicated timer
 * @param xTimerId
 * @return
 *          0: success
 *          -1 : fail
 */
int Timer_HaltSWTimer(int pTimerID)
{
    /* Check error */
    if ((pTimerID >= SYS_TIMER_MAX_SW_TIMER) || (pTimerID < 0))
        return -1;
    /* Query timers list */
    if (_xTimers[pTimerID].callback != NULL) 
    {
        /* Indicate software timer has been started */
        _xTimers[pTimerID].Started  = 0;
        return 0;
    }
    return -1;
}

/**
 * @brief Get system timer counter value in milliseconds
 * @return
 */
uint32_t Timer_GetSysMsTimer(void)
{
    return _xSystemTimerMsCounter;
}
/******************************************************************************/
/**!                          LOCAL FUNCTIONS                                 */
/******************************************************************************/
