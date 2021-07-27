 /******************************************************************************
 *
 * Module: Gpt
 *
 * File Name: Gpt_Cfg.c
 *
 * Description: Source file for Gpt Module Post Build Configurations.
 *
 * Author: Ahmed Nabil
 ******************************************************************************/
 
#include "Gpt.h"
#include "Gpt_Symbols.h"

/* PB structure used with GPT_Init API */
const strGpt_ChannelConfig_t Timers_Configurations[GPT_CONFIGURED_CHANNELS] =
{
	{GPT_TIMER_0, GPT_CLK_SRC_PRSCLR_8, GPT_CH_MODE_NORMAL}
};
/*
typedef struct
{
	u8Gpt_Channel_t			u8Timer_Id;
	enuGpt_ClockSource_t	enuClock_Src;
	enuGpt_ChannelMode_t	enuMode;
}strGpt_ChannelConfig_t;*/







