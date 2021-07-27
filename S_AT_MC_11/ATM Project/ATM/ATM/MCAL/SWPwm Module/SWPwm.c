/*
 * SWPwm.c
 *
 * Created: 7/15/2021 3:10:54 PM
 *  Author: Ahmed Nabil
 */ 

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "SWPwm.h"
/*******************************************************************************
 *                          Global Variables                                   *
 *******************************************************************************/
strSWPwm_ChannelData_t gstr_ChannelsData[SWPWM_CHANNELS_NUM] = {0};
u8SWPwm_Channel_t gu8_currentPwmChannel = 0;
enuSWPwm_State_t genu_SWPwmModStatus = SWPWM_UNINITIALIZED;
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*- FUNCTIONS IMPLEMENTATION -*-*-*-*-*-*/

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: waveFunction
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to be called inside the timer's ISR.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
void waveFunction(void)
{
	/* Toggle the Pin of the PWM Channel */
	if(Dio_togglePin(SWPwm_Channels[gu8_currentPwmChannel].u8_DioChannelID) != DIO_STATUS_ERROR_OK)
		return;
	/* Check if the PWM Channel is still in Running State */
	if (gstr_ChannelsData[gu8_currentPwmChannel].enuChannelState == SWPWM_RUNNING)
	{
		/* Start the Timer again with the new ticks assigned */
		if(GptStart_aSync(SWPwm_Channels[gu8_currentPwmChannel].u8_GptChannelID,\
		                  gstr_ChannelsData[gu8_currentPwmChannel].u32_NxtTicks,\
						   waveFunction) != GPT_STATUS_ERROR_OK)	return;
		
		/* Replace the next ticks to be counted between ONTicks and OFFTicks*/
		if(gstr_ChannelsData[gu8_currentPwmChannel].u32_NxtTicks == gstr_ChannelsData[gu8_currentPwmChannel].u32_OFFTicks)
			gstr_ChannelsData[gu8_currentPwmChannel].u32_NxtTicks = gstr_ChannelsData[gu8_currentPwmChannel].u32_ONTicks;
		else
			gstr_ChannelsData[gu8_currentPwmChannel].u32_NxtTicks = gstr_ChannelsData[gu8_currentPwmChannel].u32_OFFTicks;
	}
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: SWPwm_Init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSWPwm_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Initialize the SWPwm module.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuSWPwm_Status_t SWPwm_Init(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the SWPWM Module was already initialized */
	if(genu_SWPwmModStatus == SWPWM_INITIALIZED)
	{
		return SWPWM_STATUS_ERROR_NOK;
	}else{/*Nothing to do here*/}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Initialize the GPT Module and check if any error returned */
	enuGpt_Status_t Gpt_State = GptInit();
	if((Gpt_State != GPT_STATUS_ERROR_OK) && (Gpt_State != GPT_STATUS_ERROR_ALREADY_INIT)) //GPT_STATUS_ERROR_ALREADY_INIT
		return SWPWM_STATUS_ERROR_NOK;
		
	/* Initialize the DIO Module and check if any error returned */
	enuDio_Status_t Dio_State = Dio_init(strDio_pins);
	if((DIO_STATUS_ERROR_OK != Dio_State) && (DIO_STATUS_ALREADY_INIT != Dio_State))
		return SWPWM_STATUS_ERROR_NOK;
	
	/* Change the state of the SWPWM Module to Initialized */
	genu_SWPwmModStatus = SWPWM_INITIALIZED;
	return SWPWM_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: SWPwm_Start
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_ChannelID - The channel to put the wave on it.
*					u16_Freq - Frequency of the wave to be generated.
*					u8_DutyCycle - Duty Cycle of the wave to be generated.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSWPwm_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Start the pulse wave on u8_ChannelID.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuSWPwm_Status_t SWPwm_Start(u8SWPwm_Channel_t u8_ChannelID, uint32_t u32_Freq, uint8_t u8_DutyCycle)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the SWPwm Module is not initialized */
	if (genu_SWPwmModStatus != SWPWM_INITIALIZED)
	{
		return SWPWM_STATUS_NOT_INIT;
	}
	/* Check if the channelID given is invalid */
	if (u8_ChannelID >= SWPWM_CHANNELS_NUM )
	{
		return SWPWM_STATUS_PWM_ID_INVALID;
	}
	/* Check if the duty cycle given is out of range */
	if((u8_DutyCycle > SWPWM_MAX_DUTYCYCLE) || (u8_DutyCycle < SWPWM_MIN_DUTYCYCLE))
	{
		return SWPWM_STATUS_DC_OUTRANGE;
	}
	if(gstr_ChannelsData[u8_ChannelID].enuChannelState == SWPWM_RUNNING)
	{
		return SWPWM_STATUS_RUNNING;
	}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	if(u8_DutyCycle == 0)
	{
		/* Means that ON time is 0 == No wave */
		return SWPWM_STATUS_ERROR_OK;
	}else if(u8_DutyCycle == 100)
	{
		if(Dio_writePin(SWPwm_Channels[u8_ChannelID].u8_DioChannelID, PIN_HIGH) != DIO_STATUS_ERROR_OK)
			return SWPWM_STATUS_ERROR_NOK;
		return SWPWM_STATUS_ERROR_OK;
	}
	
	float32_t f32_wavePeriodTime = (float32_t)1 / u32_Freq;					/* The Periodic Time for the Generated Pulse in Seconds */
	float32_t f32_waveONTime = (f32_wavePeriodTime*u8_DutyCycle) / 100;		/* The HIGH Level Time in Seconds */
	float32_t f32_waveOFFTime = f32_wavePeriodTime - f32_waveONTime;		/* The LOW Level Time in Seconds */
	float32_t f32_systemPeriodTime = 0.0;
	uint16_t  u16_prescalerValue = 0;
	
	/* Get the Prescaler Value of the used Timer Channel */
	switch(Timers_Configurations[SWPwm_Channels[u8_ChannelID].u8_GptChannelID].enuClock_Src)
	{
		case GPT_CLK_SRC_PRSCLR_1:
			u16_prescalerValue = 1;
			break;
		case GPT_CLK_SRC_PRSCLR_8:
			u16_prescalerValue = 8;
			break;
		case GPT_CLK_SRC_PRSCLR_64:
			u16_prescalerValue = 64;
			break;
		case GPT_CLK_SRC_PRSCLR_256:
			u16_prescalerValue = 256;
			break;
		case GPT_CLK_SRC_PRSCLR_1024:
			u16_prescalerValue = 1024;
			break;
		case GPT_CLK_SRC_PRSCLR_EXT_FALL:
			u16_prescalerValue = 1;
			break;
		case GPT_CLK_SRC_PRSCLR_EXT_RISE:
			u16_prescalerValue = 1;
			break;
		default:
			return SWPWM_STATUS_ERROR_NOK;
	}
	
	f32_systemPeriodTime = (float32_t)u16_prescalerValue/SYS_CLOCK_FREQUENCY;				/* The Controllers Periodic Time in seconds based on the System Clock Frequency */
	gu8_currentPwmChannel = u8_ChannelID;
	gstr_ChannelsData[u8_ChannelID].u32_ONTicks = f32_waveONTime/f32_systemPeriodTime;		/* The HIGH Level Time in Ticks */
	gstr_ChannelsData[u8_ChannelID].u32_OFFTicks = f32_waveOFFTime/f32_systemPeriodTime;	/* The LOW Level Time in Ticks */
	gstr_ChannelsData[u8_ChannelID].u8_ChannelID = u8_ChannelID;

	/* Write the initial state of the pin to start the pulse time on it (HGIH LEVEL in this case) */
	if(Dio_writePin(SWPwm_Channels[u8_ChannelID].u8_DioChannelID, PIN_HIGH) != DIO_STATUS_ERROR_OK)
		return SWPWM_STATUS_ERROR_NOK;
	
	/* Start the Asynchronous Timer Counting and call the waveFunction after ONTicks*/
	if(GptStart_aSync(SWPwm_Channels[u8_ChannelID].u8_GptChannelID,\
					 gstr_ChannelsData[u8_ChannelID].u32_ONTicks,\
					 waveFunction) != GPT_STATUS_ERROR_OK)	return SWPWM_STATUS_ERROR_NOK;
	
	/* Assign the Next Ticks to be counted by the Asynchronous Timer */
	gstr_ChannelsData[u8_ChannelID].u32_NxtTicks = gstr_ChannelsData[u8_ChannelID].u32_OFFTicks;
	/* Change the state of the Channel to Running */
	gstr_ChannelsData[u8_ChannelID].enuChannelState = SWPWM_RUNNING;

	return SWPWM_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: SWPwm_Stop
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_ChannelID - The channel to put the wave on it
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSWPwm_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Stop the pulse wave on u8_ChannelID.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuSWPwm_Status_t SWPwm_Stop(u8SWPwm_Channel_t u8_ChannelID)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the SWPwm Module is not initialized */
	if (genu_SWPwmModStatus != SWPWM_INITIALIZED)
	{
		return SWPWM_STATUS_NOT_INIT;
	}
	/* Check if the channelID given is invalid */
	if (u8_ChannelID >= SWPWM_CHANNELS_NUM )
	{
		return SWPWM_STATUS_PWM_ID_INVALID;
	}
	/* Check if the duty cycle given is out of range */
	if(gstr_ChannelsData[u8_ChannelID].enuChannelState != SWPWM_RUNNING)
	{
		return SWPWM_STATUS_RUNNING;
	}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/
	
/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Change the SWPWM Pin to Low */
	if(Dio_writePin(SWPwm_Channels[u8_ChannelID].u8_DioChannelID, PIN_LOW) != DIO_STATUS_ERROR_OK)
		return SWPWM_STATUS_ERROR_NOK;
	
	/* Change the State of the PWM Channel to STOPPED */
	gstr_ChannelsData[u8_ChannelID].enuChannelState = SWPWM_STOPPED;
	
	/* Stop the Timer Channel */
	if(GptStop(SWPwm_Channels[u8_ChannelID].u8_GptChannelID) != GPT_STATUS_ERROR_OK)
		return SWPWM_STATUS_ERROR_NOK;
	
	return SWPWM_STATUS_ERROR_OK;
}

