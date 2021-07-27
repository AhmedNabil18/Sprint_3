 /******************************************************************************
 *
 * Module: Gpt
 *
 * File Name: Gpt.h
 *
 * Description: Header file for Gpt Module.
 *
 * Author: Ahmed Nabil
 ******************************************************************************/


#ifndef GPT_H_
#define GPT_H_


/* GPT Pre-Compile Configuration Header file */
#include "Gpt_Cfg.h"
#include "Gpt_Symbols.h"
#include "..\Platform_Types.h"
#include "..\ATMega32_Interrupts.h"

/*******************************************************************************
 *                                 MACROS                                      *
 *******************************************************************************/
#define GPT_MAX_VALUE_8BIT      (u32Gpt_Value_t)0xFF
#define GPT_MAX_VALUE_16BIT		(u32Gpt_Value_t)0xFFFF


/*******************************************************************************
 *                          Module Data Types                                  *
 *******************************************************************************/

/*
 * Data Type for GPT return status
 */
typedef enum
{
	GPT_STATUS_ERROR_NOK=0,
	GPT_STATUS_ERROR_OK,
	GPT_STATUS_ERROR_NOT_INIT,
	GPT_STATUS_ERROR_ALREADY_INIT,
	GPT_STATUS_ERROR_NULL_ARG,
	GPT_STATUS_ERROR_INVALID_TIMER_NUM,
	GPT_STATUS_ERROR_INVALID_TIMER_ID,
	GPT_STATUS_ERROR_TIMER_BUSY,
	GPT_STATUS_ERROR_TIMER_STOPPED
}enuGpt_Status_t;

/*
 * Data Type for GPT state
 */
typedef enum
{
	GPT_UNINITIALIZED = 0,
	GPT_INITIALIZED,
	GPT_RUNNING,
	GPT_STOPPED,
	GPT_EXPIRED
}enuGpt_State_t;


/*
 * Data Type for a pointer to notification call back function
 * Parameter (In) : None
 * Parameter (Out): None
 * Return Values : None
 */
typedef void (*pfGpt_CallBack_t)(void);

/* 
 * Data Type for the timer value (in number of ticks) 
 */
typedef uint32_t u32Gpt_Value_t;

/*
 * Data Type for the numeric ID for a GPT Channel 
 */
typedef uint8_t u8Gpt_Channel_t;

/*
 * Data Type for the prescaler state
 */
typedef enum
{
    GPT_CLK_SRC_OFF=0,
	GPT_CLK_SRC_PRSCLR_1,
	GPT_CLK_SRC_PRSCLR_8,
	GPT_CLK_SRC_PRSCLR_64,
	GPT_CLK_SRC_PRSCLR_256,
	GPT_CLK_SRC_PRSCLR_1024,
	GPT_CLK_SRC_PRSCLR_EXT_FALL,
	GPT_CLK_SRC_PRSCLR_EXT_RISE
}enuGpt_ClockSource_t;

/*
 * Data Type for the The modes of the channel
 */
typedef enum
{
	GPT_CH_MODE_NORMAL=0,
	GPT_CH_MODE_CTC_NORMAL,
	GPT_CH_MODE_CTC_TOGGLE,
	GPT_CH_MODE_CTC_SET,
	GPT_CH_MODE_CTC_CLEAR,
}enuGpt_ChannelMode_t;
/*
 * Data Type for configurations of a timer's channel
 */
typedef struct
{
	u8Gpt_Channel_t			u8Timer_Id;
	enuGpt_ClockSource_t	enuClock_Src;
	enuGpt_ChannelMode_t	enuMode;
}strGpt_ChannelConfig_t;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/* Initializes hardware timer modules configured in PBCfg.c */
//enuGpt_Status_t Gpt_Init( const strGpt_ChannelConfig_t* ConfigPtr );
enuGpt_Status_t GptInit(void);

/* Make this timer channel starts counting synchronous (Polling) */
enuGpt_Status_t GptStart_Sync(u8Gpt_Channel_t ChannelId, u32Gpt_Value_t u16_Ticks);

/* Make this timer channel starts counting Asynchronous (Interrupt) */
enuGpt_Status_t GptStart_aSync(u8Gpt_Channel_t ChannelId, u32Gpt_Value_t u16_Ticks, pfGpt_CallBack_t CallBack);

/* Stop this timer channel from counting */
enuGpt_Status_t GptStop(u8Gpt_Channel_t ChannelId);


/* Returns the elapsed time from the current period */
//enuGpt_Status_t Gpt_GetTimeElapsed( u8Gpt_Channel_t Channel, u16Gpt_Value_t* pu16Value );

/* Returns the remaining time until the target time is reached */
//enuGpt_Status_t Gpt_GetTimeRemaining( u8Gpt_Channel_t Channel, u16Gpt_Value_t* pu16Value );

/* Enables the interrupt notification for a channel (relevant in normal mode) */
//enuGpt_Status_t Gpt_EnableNotification( u8Gpt_Channel_t Channel );

/* Disables the interrupt notification for a channel (relevant in normal mode) */
//enuGpt_Status_t Gpt_DisableNotification( u8Gpt_Channel_t Channel );

/* Changes the state of the timer channel */
//enuGpt_Status_t Gpt_StateChange(u8Gpt_Channel_t Channel, enuGpt_State_t State);


/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/
extern const strGpt_ChannelConfig_t Timers_Configurations[GPT_CONFIGURED_CHANNELS] ;




#endif
