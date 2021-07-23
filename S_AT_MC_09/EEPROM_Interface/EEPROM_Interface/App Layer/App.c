/*
 * App.c
 *
 * Created: 7/16/2021 1:22:50 PM
 *  Author: Ahmed Nabil
 */ 

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "App.h"

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- GLOBAL STATIC VARIABLES *-*-*-*-*-*/
static enuApp_Status_t enuCurrentAppStatus = APP_STATUS_UNINITIALIZED;
static const uint8_t cgau8_writeString[6] = "WRITE";
static const uint8_t cgau8_readString[5] = "READ";
static uint8_t gu8_appAction = APP_ACTION_TERMINAL;
static uint8_t gu8_appState = APP_STATE_CMD;
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*- FUNCTIONS IMPLEMENTATION -*-*-*-*-*-*/

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: App_start
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Start the Application.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t App_start(void)
{
	/**************************************************************************************/
	/*								Function Implementation								  */
	/**************************************************************************************/
	/* Initialize the application */
	if(App_init() != APP_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	EnableGlbl_Interrupt();
	/* Application Super Loop */
	if(Terminal_Out((uint8_t*)"Command: ") != TERMINAL_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	while (1)
	{
		/* Update the App Status */
		if(App_update() != APP_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
			
	}
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: App_init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Initialize the Application.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t App_init(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Application was already initialized */
	if (enuCurrentAppStatus == APP_STATUS_INITIALIZED)
	{
		return APP_STATUS_INITIALIZED;
	}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/

	/* Call the initializer of the Terminal */
	if(TERMINAL_STATUS_ERROR_OK != Terminal_init())
	{
		return APP_STATUS_ERROR_NOK;
	}
	/* Call the initializer of the Eeprom */
	if(EEPROM_24_STATUS_ERROR_OK != Eeprom_24_init())
	{
		return APP_STATUS_ERROR_NOK;
	}
	/* Update enuCurrentAppStatus to initialized */
	enuCurrentAppStatus = APP_STATUS_INITIALIZED;
	return APP_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: App_update
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Update the state of the application.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t App_update(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Application was already initialized */
	if (enuCurrentAppStatus != APP_STATUS_INITIALIZED)
	{
		return APP_STATUS_UNINITIALIZED;
	}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	static uint16_t u16_locationAddress=0;
	uint8_t u8_data[2]={0};
	uint8_t au8_inputString[MAX_INPUT_SIZE]={0};
	enuTerminal_Status_t enuTermStatus = Terminal_In(au8_inputString);
	if (enuTermStatus == TERMINAL_STATUS_INPUT_CHANGED)
	{
		if(gu8_appState == APP_STATE_CMD)
		{
			if(stringCompare(au8_inputString,(uint8_t*)cgau8_writeString) == 1)
			{
				gu8_appAction = APP_ACTION_WRITE;
				gu8_appState = APP_STATE_ADDRESS;
				if(Terminal_Out((uint8_t*)"Address: ") != TERMINAL_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
			}else if(stringCompare(au8_inputString,(uint8_t*)cgau8_readString) == 1)
			{
				gu8_appAction = APP_ACTION_READ;
				gu8_appState = APP_STATE_ADDRESS;
				if(Terminal_Out((uint8_t*)"Address: ") != TERMINAL_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
			}else
			{
				if(Terminal_Out((uint8_t*)"INVALID Input!! \rCommand: ") != TERMINAL_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				gu8_appAction = APP_ACTION_TERMINAL;
				gu8_appState = APP_STATE_CMD;
			}
		}else if(gu8_appState == APP_STATE_ADDRESS)
		{
			stringHexToNum(au8_inputString,&u16_locationAddress);
			if(u16_locationAddress > EEPROM_24_WORDS_NUM-1)
			{
				if(Terminal_Out((uint8_t*)"INVALID Address!! \rAddress: ") != TERMINAL_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
			}else
			{
				if(Terminal_Out((uint8_t*)"OK\r") != TERMINAL_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				if(gu8_appAction == APP_ACTION_READ)
				{
					Eeprom_24_readByte(u16_locationAddress,u8_data);
					u8_data[1] = '\0';
					if(Terminal_Out((uint8_t*)"Data Stored is: ") != TERMINAL_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					if(Terminal_Out(u8_data) != TERMINAL_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					if(Terminal_Out((uint8_t*)"\rCommand: ") != TERMINAL_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					gu8_appAction = APP_ACTION_TERMINAL;
					gu8_appState = APP_STATE_CMD;
				}else if(gu8_appAction == APP_ACTION_WRITE)
				{
					if(Terminal_Out((uint8_t*)"Data: ") != TERMINAL_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					gu8_appAction = APP_ACTION_WRITE;
					gu8_appState = APP_STATE_DATA;
				}
			}
		}else if(gu8_appState == APP_STATE_DATA)
		{
			if(Terminal_Out((uint8_t*)"OK\r") != TERMINAL_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
			Eeprom_24_writeByte(u16_locationAddress, au8_inputString[0]);
			if(Terminal_Out((uint8_t*)"Command: ") != TERMINAL_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
			gu8_appAction = APP_ACTION_TERMINAL;
			gu8_appState = APP_STATE_CMD;
		}
	}else if(enuTermStatus != TERMINAL_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	
	return APP_STATUS_ERROR_OK;
}