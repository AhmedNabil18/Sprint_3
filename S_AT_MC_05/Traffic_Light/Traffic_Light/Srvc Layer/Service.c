/*
 * Service.c
 *
 * Created: 7/16/2021 2:09:18 PM
 *  Author: Ahmed Nabil
 */ 
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Service.h"

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- GLOBAL STATIC VARIABLES *-*-*-*-*-*/
enuSrvc_Status_t genu_SrvcStatus = SRVC_STATUS_UNINITIALIZED;
extern  uint8_t gau8_leds[LEDS_USED_NUM];
const uint8_t cgau8_startString[6]  = "START";
const uint8_t cgau8_waitString[6] = "WAIT";
const uint8_t cgau8_stopString[6] = "STOP";
const uint8_t cgau8_atString[6]  = "AT";
uint8_t gu8_terminalInput = TERM_SRVC_OFF;
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*- FUNCTIONS IMPLEMENTATION -*-*-*-*-*-*/

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Service_init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSrvc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Initialize the Service layer -- Initialize all the below layers.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuSrvc_Status_t Service_init(void)
{
	/* Check if the Service Layer has been already initialized */
	if(genu_SrvcStatus == SRVC_STATUS_INITIALIZED)
		return SRVC_STATUS_INITIALIZED;
		
	/* Call the initializer in Button and Motor*/
	if(LED_STATUS_ERROR_OK != Led_init())
		return SRVC_STATUS_ERROR_NOK;
	if(TERMINAL_STATUS_ERROR_OK != Terminal_init())
		return SRVC_STATUS_ERROR_NOK;
	
	genu_SrvcStatus = SRVC_STATUS_INITIALIZED;
	return SRVC_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Service_ReportButton
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_button - Pointer to variable to hold the button pressed
* Return value: enuSrvc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Start the Application.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuSrvc_Status_t Service_ReportTerminal(uint8_t* pu8_terminal)
{
	uint8_t au8_inputString[MAX_INPUT_SIZE]={0};
	enuTerminal_Status_t enuTermStatus = Terminal_In(au8_inputString);
	
	if( (enuTermStatus != TERMINAL_STATUS_ERROR_OK) && (enuTermStatus != TERMINAL_STATUS_INPUT_CHANGED))
		return SRVC_STATUS_ERROR_NOK;
	if (stringCompare((uint8_t*)au8_inputString,(uint8_t*)cgau8_startString) == 1)
	{
		*pu8_terminal = TERM_SRVC_START;
		gu8_terminalInput = TERM_SRVC_START;
		return SRVC_STATUS_ERROR_OK;
	}
	else if (stringCompare((uint8_t*)au8_inputString,(uint8_t*)cgau8_waitString) == 1)
	{
		*pu8_terminal = TERM_SRVC_WAIT;
		gu8_terminalInput = TERM_SRVC_WAIT;
		return SRVC_STATUS_ERROR_OK;
	}
	else if (stringCompare((uint8_t*)au8_inputString,(uint8_t*)cgau8_stopString) == 1)
	{
		*pu8_terminal = TERM_SRVC_STOP;
		gu8_terminalInput = TERM_SRVC_STOP;
		return SRVC_STATUS_ERROR_OK;
	}
	else if (stringCompare((uint8_t*)au8_inputString,(uint8_t*)cgau8_atString) == 1)
	{
		*pu8_terminal = TERM_SRVC_AT;
		gu8_terminalInput = TERM_SRVC_AT;
		return SRVC_STATUS_ERROR_OK;
	}else
	{
		if (gu8_terminalInput != TERM_SRVC_AT)
		{
			*pu8_terminal = gu8_terminalInput;
		}
		return SRVC_STATUS_ERROR_OK;
	}
	return SRVC_STATUS_ERROR_NOK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Service_UpdateLeds
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_ledAction - The action should be taken for the Leds
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSrvc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Update the the terminal of the output.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuSrvc_Status_t Service_UpdateLeds(uint8_t u8_ledAction)
{
	
	switch (u8_ledAction)
	{
		case SRVC_ACTION_IDLE:
			Led_Off(LED_GREEN);
			Led_Off(LED_YELLOW);
			Led_Off(LED_RED);
			break;
		case SRVC_ACTION_OK:
			Led_Off(LED_GREEN);
			Led_Off(LED_YELLOW);
			Led_Off(LED_RED);
			break;
		case SRVC_ACTION_GREEN:
			Led_On(LED_GREEN);
			Led_Off(LED_YELLOW);
			Led_Off(LED_RED);
			break;
		case SRVC_ACTION_YELLOW:
			Led_Off(LED_GREEN);
			Led_On(LED_YELLOW);
			Led_Off(LED_RED);
			break;
		case SRVC_ACTION_RED:
			Led_Off(LED_GREEN);
			Led_Off(LED_YELLOW);
			Led_On(LED_RED);
			break;
		default:
			return SRVC_STATUS_ERROR_NOK;
	}
	return  SRVC_STATUS_ERROR_OK;
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Service_UpdateTerminal
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_terminalAction - The action should be taken for the terminal.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSrvc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Update the the output on the terminal.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuSrvc_Status_t Service_UpdateTerminal(uint8_t u8_terminalAction)
{
	switch (u8_terminalAction)
	{
		case SRVC_ACTION_IDLE:
			break;
		case SRVC_ACTION_OK:
			Terminal_Out((uint8_t*)"OK\r");
			break;
		case SRVC_ACTION_GREEN:
			Terminal_Out((uint8_t*)"Green Led is On\r");
			break;
		case SRVC_ACTION_YELLOW:
			Terminal_Out((uint8_t*)"Yellow Led is On\r");
			break;
		case SRVC_ACTION_RED:
			Terminal_Out((uint8_t*)"Red Led is On\r");
			break;
		default:
			return SRVC_STATUS_ERROR_NOK;
	}
	return  SRVC_STATUS_ERROR_OK;
}