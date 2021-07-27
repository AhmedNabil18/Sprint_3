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
* Description: Function to Initialize the Service layer -- Initialize all the below Modules.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuSrvc_Status_t Service_init(void)
{
	/* Check if the Service Layer has been already initialized */
	if(genu_SrvcStatus == SRVC_STATUS_INITIALIZED)
		return SRVC_STATUS_INITIALIZED;
		
	if(EEPROM_24_STATUS_ERROR_OK != Eeprom_24_init())
	return SRVC_STATUS_ERROR_NOK;
	if(TERMINAL_STATUS_ERROR_OK != Terminal_init())
	return SRVC_STATUS_ERROR_NOK;
	if(SPI_STATUS_ERROR_OK != Spi_init())
	return SRVC_STATUS_ERROR_NOK;
		
	genu_SrvcStatus = SRVC_STATUS_INITIALIZED;
	return SRVC_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Service_ReportTerminal
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_data - Pointer to variable to hold the input by terminal.
* Return value: enuSrvc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to report the terminal's input.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuSrvc_Status_t Service_ReportTerminal(uint8_t* pu8_data)
{
	uint8_t au8_inputString[MAX_INPUT_SIZE]={0};
	enuTerminal_Status_t enuTermStatus = Terminal_In(au8_inputString);
	if (enuTermStatus == TERMINAL_STATUS_INPUT_CHANGED)
	{
		stringCopy(au8_inputString,pu8_data);
		return SRVC_STATUS_ERROR_OK;
	}else if(enuTermStatus != TERMINAL_STATUS_ERROR_OK)
		return SRVC_STATUS_ERROR_NOK;
	return SRVC_STATUS_ERROR_NOK;
}
