 /******************************************************************************
 *
 * Module: Gpt
 *
 * File Name: Gpt.c
 *
 * Description: Source file for Gpt Module.
 *
 * Author: Ahmed Nabil
 ******************************************************************************/
 
 #include "Gpt.h"
#include "..\ATMega32_Registers.h"


static void (*Gpt_Timer0_CallBackPtr)(void);
/*******************************************************************************
 *                          Global Variables                                   *
 *******************************************************************************/
enuGpt_State_t enuChannelsState[GPT_CONFIGURED_CHANNELS] = {GPT_UNINITIALIZED};
uint32_t u32_Channels_ISR_Count[GPT_CONFIGURED_CHANNELS] = {0};
enuGpt_State_t genu_GPTModStatus = GPT_UNINITIALIZED;
/*******************************************************************************
 *                      Function Definitions                                   *
 *******************************************************************************/
/************************************************************************************
* Service Name: GptInit
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuGpt_Status_t - Returns the status of the function (if there is any error)
* Description: Function to Initialize the Gpt module.
************************************************************************************/
enuGpt_Status_t GptInit( void )
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the module was already initialized */
	if (genu_GPTModStatus == GPT_INITIALIZED)
	{
		return GPT_STATUS_ERROR_ALREADY_INIT;
	}else{/*Nothing to here*/}
		
	/* Check if the input parameter is NULL */
	if (NULL_PTR == Timers_Configurations)
	{
		return GPT_STATUS_ERROR_NULL_ARG;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint8_t u8_loopIndex=0;
	
	for(u8_loopIndex = 0;u8_loopIndex<GPT_CONFIGURED_CHANNELS;u8_loopIndex++)
	{
		switch(Timers_Configurations[u8_loopIndex].u8Timer_Id)
		{
			/************************************************************************/
			/************************************************************************/
			case GPT_TIMER_0: /* In case of Timer 0 */
				if(Timers_Configurations[u8_loopIndex].enuMode == GPT_CH_MODE_NORMAL) /* In case of Normal Mode */
				{
					GPT_TCCR0_REG = 0x00;
					GPT_TIMSK_REG &= ~(3 << GPT_TIMSK_TOIE0);
					GPT_TCNT0_REG = 0x00;
				}else
				{
					
				}
				break;
			/************************************************************************/
			/************************************************************************/
			case GPT_TIMER_1: /* In case of Timer 1 */
				break;
			/************************************************************************/
			/************************************************************************/
			case GPT_TIMER_2: /* In case of Timer 2 */
				break;
			/************************************************************************/
			/************************************************************************/
			default:
				return GPT_STATUS_ERROR_INVALID_TIMER_NUM;
		}
		enuChannelsState[u8_loopIndex] = GPT_INITIALIZED;
	}
	genu_GPTModStatus = GPT_INITIALIZED;
	return GPT_STATUS_ERROR_OK;
}


/************************************************************************************
* Service Name: Gpt_GetTimeElapsed
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Channel - The timer channel ID.
* Parameters (inout): None
* Parameters (out): None
* Return value: Gpt_ValueType - Data type for the value of timer counter in ticks
* Description: Function to return the elapsed time from start of counting until now.
************************************************************************************/
/*
enuGpt_Status_t Gpt_GetTimeElapsed( u8Gpt_Channel_t Channel, u16Gpt_Value_t* pu16Value )
{
}
*/

/************************************************************************************
* Service Name: Gpt_GetTimeRemaining
* Service ID[hex]: 0x04
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Channel - The timer channel ID.
* Parameters (inout): None
* Parameters (out): None
* Return value: Gpt_ValueType - Data type for the value of timer counter in ticks
* Description: Function to return the remaining time from now until the reaching the time target.
************************************************************************************/
/*enuGpt_Status_t Gpt_GetTimeRemaining( u8Gpt_Channel_t Channel, u16Gpt_Value_t* pu16Value )
{
	
}*/

/************************************************************************************
* Service Name: GptStart_Sync
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): ChannelId - The timer channel ID.
				   u16_Ticks - The target number of ticks the timer should count.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuGpt_Status_t - Returns the status of the function (if there is any error)
* Description: Function to Start the Timer module counting from 0x00 until (u16_Ticks).
************************************************************************************/
enuGpt_Status_t GptStart_Sync(u8Gpt_Channel_t ChannelId, u32Gpt_Value_t u16_Ticks)
{
	
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
/* Check if the channel id is valid */
	if(GPT_CONFIGURED_CHANNELS <= ChannelId)
	{
		return GPT_STATUS_ERROR_INVALID_TIMER_ID;
		}else{/*Nothing to here*/}
/* Check if the module is not initialized */
	if (GPT_UNINITIALIZED == enuChannelsState[ChannelId])
	{
		return GPT_STATUS_ERROR_NOT_INIT;
	}else{/*Nothing to here*/}
		
/* Check if the module is not initialized */
	if (GPT_RUNNING == enuChannelsState[ChannelId])
	{
		return GPT_STATUS_ERROR_TIMER_BUSY;
	}else{/*Nothing to here*/}

/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint16_t u16_OVFCount = 0, u16_remainderCount=0;;
	uint16_t u16_loopIndex=0;
	
	enuChannelsState[ChannelId] = GPT_RUNNING;
	switch (Timers_Configurations[ChannelId].u8Timer_Id)
	{
		/************************************************************************/
		/************************************************************************/
		case GPT_TIMER_0: /* In case of Timer 0 */
			/* Get Number of loops to overflow the timer counter */
			u16_OVFCount = u16_Ticks / GPT_MAX_VALUE_8BIT;//0xFF
			/* Get Number of extra ticks (remainder) */
			u16_remainderCount = u16_Ticks % GPT_MAX_VALUE_8BIT;
			
			/* Start the clock with the given pre scaler */
			GPT_TCCR0_REG = (Timers_Configurations[ChannelId].enuClock_Src) << GPT_TCCR0_CS00;
			/* Run the remainder ticks */
			/* Put the value of the 0xFF - remainderTicks to start count from it */
			GPT_TCNT0_REG = 256 - u16_remainderCount; 
			/* Wait for the overflow flag to be set */
			while((GPT_TIFR_REG & (1<<GPT_TIFR_TOV0))==0);
			/* Clear the overflow flag */
			REG_SET_BIT(GPT_TIFR_REG,GPT_TIFR_TOV0);
			
			for(u16_loopIndex=0; u16_loopIndex<u16_OVFCount; u16_loopIndex++)
			{
				/* Wait for the overflow flag to be set */
				while((GPT_TIFR_REG & (1<<GPT_TIFR_TOV0))==0);
				/* Clear the overflow flag */
				REG_SET_BIT(GPT_TIFR_REG,GPT_TIFR_TOV0);
			}
			/* Stop the clock from the timer */
			REG_CLR_BIT(GPT_TCCR0_REG, GPT_TCCR0_CS00);
			REG_CLR_BIT(GPT_TCCR0_REG, GPT_TCCR0_CS01);
			REG_CLR_BIT(GPT_TCCR0_REG, GPT_TCCR0_CS02);
			
			break;
		/************************************************************************/
		/************************************************************************/
		case GPT_TIMER_1: /* In case of Timer 1 */
		break;
		/************************************************************************/
		/************************************************************************/
		case GPT_TIMER_2: /* In case of Timer 2 */
		break;
		/************************************************************************/
		/************************************************************************/
		default:
		return GPT_STATUS_ERROR_INVALID_TIMER_NUM;
	}
	enuChannelsState[ChannelId] = GPT_EXPIRED;
	return GPT_STATUS_ERROR_OK;
}

/************************************************************************************
* Service Name: GptStart_aSync
* Sync/Async: ASynchronous
* Reentrancy: Non Reentrant
* Parameters (in): ChannelId - The timer channel ID.
				   u16_Ticks - The target number of ticks the timer should count.
				   CallBack - Callback function for ISR to call
* Parameters (inout): None
* Parameters (out): None
* Return value: enuGpt_Status_t - Returns the status of the function (if there is any error)
* Description: Function to Start the Timer module counting from 0x00 until (u16_Ticks)
               and generate interrupt when OVF.
************************************************************************************/
enuGpt_Status_t GptStart_aSync(u8Gpt_Channel_t ChannelId, u32Gpt_Value_t u16_Ticks, pfGpt_CallBack_t CallBack)
{
	
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
/* Chek if the channel id is valid */
	if(GPT_CONFIGURED_CHANNELS <= ChannelId)
	{
		return GPT_STATUS_ERROR_INVALID_TIMER_ID;
		}else{/*Nothing to here*/}
/* Check if the module is not initialized */
	if (GPT_UNINITIALIZED == enuChannelsState[ChannelId])
	{
		return GPT_STATUS_ERROR_NOT_INIT;
	}else{/*Nothing to here*/}
		
/* Check if the module is not initialized */
	if (GPT_RUNNING == enuChannelsState[ChannelId])
	{
		return GPT_STATUS_ERROR_TIMER_BUSY;
	}else{/*Nothing to here*/}		

/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint16_t u16_remainderCount=0;
	
	enuChannelsState[ChannelId] = GPT_RUNNING;
	switch (Timers_Configurations[ChannelId].u8Timer_Id)
	{
		/************************************************************************/
		/************************************************************************/
		case GPT_TIMER_0: /* In case of Timer 0 */
			/* Get Number of loops to overflow the timer counter */
			u32_Channels_ISR_Count[ChannelId] = u16_Ticks / GPT_MAX_VALUE_8BIT; //0xFF
			/* Get Number of extra ticks (remainder) */
			u16_remainderCount = u16_Ticks % GPT_MAX_VALUE_8BIT;
			/* Set the interrupt enable */
			REG_SET_BIT(GPT_TIMSK_REG, GPT_TIMSK_TOIE0);
			/* Set the callback function pointer */
			Gpt_Timer0_CallBackPtr=CallBack;
			/* Start the clock with the given pre scaler */
			GPT_TCCR0_REG = (Timers_Configurations[ChannelId].enuClock_Src) << GPT_TCCR0_CS00;
			/* Run the remainder ticks */
			/* Put the value of the 0xFF - remainderTicks to start count from it */
			if(u16_remainderCount != 0)
			{
				GPT_TCNT0_REG = 256 - u16_remainderCount;
			}
			break;
		/************************************************************************/
		/************************************************************************/
		case GPT_TIMER_1: /* In case of Timer 1 */
			break;
		/************************************************************************/
		/************************************************************************/
		case GPT_TIMER_2: /* In case of Timer 2 */
			break;
		/************************************************************************/
		/************************************************************************/
		default:
			return GPT_STATUS_ERROR_INVALID_TIMER_NUM;
	}
	return GPT_STATUS_ERROR_OK;
}

/************************************************************************************
* Service Name: Gpt_StopTimer
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): ChannelId - The timer channel ID.
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Stop the timer from counting.
************************************************************************************/
enuGpt_Status_t GptStop( u8Gpt_Channel_t ChannelId )
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
/* Chek if the channel id is valid */
	if(GPT_CONFIGURED_CHANNELS <= ChannelId)
	{
		return GPT_STATUS_ERROR_INVALID_TIMER_ID;
	}else{/*Nothing to here*/}
	/* Check if the module is not initialized */
	if (GPT_UNINITIALIZED == enuChannelsState[ChannelId])
	{
		return GPT_STATUS_ERROR_NOT_INIT;
	}else{/*Nothing to here*/}
		
	/* Check if the module is not initialized */
	if (GPT_RUNNING != enuChannelsState[ChannelId])
	{
		return GPT_STATUS_ERROR_TIMER_STOPPED;
	}else{/*Nothing to here*/}

	/**************************************************************************************/
	/*								End of Error Checking								  */
	/**************************************************************************************/

	/**************************************************************************************/
	/*								Function Implementation								  */
	/**************************************************************************************/
	
	switch (Timers_Configurations[ChannelId].u8Timer_Id)
	{
		/************************************************************************/
		/************************************************************************/
		case GPT_TIMER_0: /* In case of Timer 0 */
			/* Stop the clock from the timer */
			GPT_TCCR0_REG &= ~(7 << GPT_TCCR0_CS00);
			GPT_TIMSK_REG &= ~(3 << GPT_TIMSK_TOIE0);
			break;
		/************************************************************************/
		/************************************************************************/
		case GPT_TIMER_1: /* In case of Timer 1 */
		break;
		/************************************************************************/
		/************************************************************************/
		case GPT_TIMER_2: /* In case of Timer 2 */
		break;
		/************************************************************************/
		/************************************************************************/
		default:
		return GPT_STATUS_ERROR_INVALID_TIMER_NUM;
	}
	/* Change the state of the Timer Channel to stopped */
	enuChannelsState[ChannelId] = GPT_STOPPED;
	return GPT_STATUS_ERROR_OK;
}

/************************************************************************************
* Service Name: Gpt_ISRHandler
* Sync/Async: ASynchronous
* Reentrancy: Non Reentrant
* Parameters (in): Timer_id - The Timer number.
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to handle the ISR calling along with the callback functions.
************************************************************************************/
static void Gpt_ISRHandler(u8Gpt_Channel_t Timer_id)
{
	uint8_t u8_loopIndex = 0;
	for (u8_loopIndex=0; u8_loopIndex<GPT_CONFIGURED_CHANNELS; u8_loopIndex++)
	{
		if( Timer_id == Timers_Configurations[u8_loopIndex].u8Timer_Id )
			break;
	}
	if(u32_Channels_ISR_Count[u8_loopIndex] != 0)
	{
		u32_Channels_ISR_Count[u8_loopIndex]--;
		return;
	}
	REG_CLR_BIT(GPT_TCCR0_REG, GPT_TCCR0_CS00);
	REG_CLR_BIT(GPT_TCCR0_REG, GPT_TCCR0_CS01);
	REG_CLR_BIT(GPT_TCCR0_REG, GPT_TCCR0_CS02);
	/* Clear the interrupt enable */
	REG_CLR_BIT(GPT_TIMSK_REG, GPT_TIMSK_TOIE0);
	enuChannelsState[u8_loopIndex] = GPT_EXPIRED;
	/* CallBack the notification function */
	(*Gpt_Timer0_CallBackPtr)();
}

ISR(TIMER0_OVF_IRQ){Gpt_ISRHandler(GPT_TIMER_0);}
ISR(TIMER0_COMP_IRQ){Gpt_ISRHandler(GPT_TIMER_0);}
ISR(TIMER1_OVF_IRQ){Gpt_ISRHandler(GPT_TIMER_1);}
ISR(TIMER1_CAPT_IRQ){Gpt_ISRHandler(GPT_TIMER_1);}
ISR(TIMER1_COMPA_IRQ){Gpt_ISRHandler(GPT_TIMER_1);}
ISR(TIMER1_COMPB_IRQ){Gpt_ISRHandler(GPT_TIMER_1);}
ISR(TIMER2_COMP_IRQ){Gpt_ISRHandler(GPT_TIMER_2);}
ISR(TIMER2_OVF_IRQ){Gpt_ISRHandler(GPT_TIMER_2);}
	