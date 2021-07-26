/*
 * SWPwm_Cfg.c
 *
 * Created: 7/15/2021 3:33:40 PM
 *  Author: Ahmed Nabil
 */ 

#include "SWPwm.h"

/* Array of Structures holds the DIO Channel and Timer Channel Conntected to each PWM Channel */
const strSWPwm_ChannelConfig_t SWPwm_Channels[SWPWM_CHANNELS_NUM] = 
{
	{SWPWM_CHANNEL_0_DIO, SWPWM_CHANNEL_0_TIMER}
};