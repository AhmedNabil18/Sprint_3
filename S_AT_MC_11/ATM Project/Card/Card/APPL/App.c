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
static uint8_t gu8_CardMode = CARD_MODE_ADMIN;
static strCardData_t  gstr_userCardData = {0};
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
	/* Initialize Used Modules */
	if(EEPROM_24_STATUS_ERROR_OK != Eeprom_24_init())
		return APP_STATUS_ERROR_NOK;
	if(TERMINAL_STATUS_ERROR_OK != Terminal_init())
		return APP_STATUS_ERROR_NOK;
	if(SPI_STATUS_ERROR_OK != Spi_init())
		return APP_STATUS_ERROR_NOK;
	/**************************/
	/* Only for Testing */
	if(Eeprom_24_writeByte(CARD_INIT_ADDRESS, 0xFF) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	Delay_ms(10);
	/**************************/
	if(Terminal_Out((uint8_t*)"CARD Terminal\r") != TERMINAL_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	uint8_t u8_initData = 0;
	/* Check if the card was previously registered and has its data in the eeprom */
	if(Eeprom_24_readByte(CARD_INIT_ADDRESS, &u8_initData) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
		
	if(u8_initData != CARD_INITIALIZED)
	{
		gu8_CardMode = CARD_MODE_ADMIN;
	}else
	{
		gu8_CardMode = CARD_MODE_USER;
	}
	
	DIO_PORTA_DIR=0xFF;
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
	if(gu8_CardMode == CARD_MODE_ADMIN)
	{ /* PROGRAMMING MODE */
		if(Terminal_Out((uint8_t*)"Programming Mode\rEnter the Following Data\r") != TERMINAL_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		uint8_t au8_tempCardHolderName[10]={0};
		uint8_t au8_tempPAN[10]={0};
		uint8_t au8_tempPinNum[5]={0};
		
		/* Get the Card Holder Name from the ADMIN Terminal */
		if(AppADMIN_getCardName(au8_tempCardHolderName) != APP_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		/* Get the Card Primary Account Number from the ADMIN Terminal */
		if(AppADMIN_getCardPAN(au8_tempPAN) != APP_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
		
		if(AppADMIN_getCardPIN(au8_tempPinNum) != APP_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		
		stringCopy(au8_tempCardHolderName, gstr_userCardData.au8_cardHolderName);
		stringCopy(au8_tempPAN, gstr_userCardData.au8_primaryAccountNumber);
		stringCopy(au8_tempPinNum, gstr_userCardData.au8_pinNum);
		
		if(AppADMIN_saveCardData(&gstr_userCardData) != APP_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		
		gu8_CardMode = CARD_MODE_USER;
		if(Terminal_Out((uint8_t*)"USER Mode\r") != TERMINAL_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
	}else
	{
		
	}
	
	Delay_ms(155);
	return APP_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: App_ReportTerminal
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_data - Pointer to variable to hold the input by terminal.
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to report the terminal's input.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t App_ReportTerminal(uint8_t* pu8_data)
{
	uint8_t au8_inputString[MAX_INPUT_SIZE]={0};
	enuTerminal_Status_t enuTermStatus = Terminal_In(au8_inputString);
	if (enuTermStatus == TERMINAL_STATUS_INPUT_CHANGED)
	{
		stringCopy(au8_inputString, pu8_data);
		return APP_STATUS_ERROR_OK;
	}else if(enuTermStatus != TERMINAL_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	return APP_STATUS_NO_OP;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppADMIN_getCardName
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_data - Pointer to variable to hold the input CARD Holder Name by terminal.
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to get the card holder's name.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppADMIN_getCardName(uint8_t* pu8_data)
{
	enuApp_Status_t App_terminalStatus = APP_STATUS_ERROR_OK;
	do
	{
		if(Terminal_Out((uint8_t*)"Card Holder Name: ") != TERMINAL_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
		do
		{
			App_terminalStatus = App_ReportTerminal(pu8_data);
			if(App_terminalStatus == APP_STATUS_ERROR_OK)
			{
				break;
			}else if((App_terminalStatus != APP_STATUS_ERROR_OK) && (App_terminalStatus != APP_STATUS_NO_OP))
			return APP_STATUS_ERROR_NOK;
		} while (App_terminalStatus == APP_STATUS_NO_OP);
		
		
		if (pu8_data[MAX_NAME_LENGTH] == '\0')
		break;
		if(Terminal_Out((uint8_t*)"Invalid Name, Only 9 characters") != TERMINAL_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
	} while (1);
	return APP_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppADMIN_getCardPAN
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_data - Pointer to variable to hold the input CARD PAN by terminal.
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to get the card pan.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppADMIN_getCardPAN(uint8_t* pu8_data)
{
	enuApp_Status_t App_terminalStatus = APP_STATUS_ERROR_OK;
	do
	{
		if(Terminal_Out((uint8_t*)"Card PAN: ") != TERMINAL_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
		
		do
		{
			App_terminalStatus = App_ReportTerminal(pu8_data);
			if(App_terminalStatus == APP_STATUS_ERROR_OK)
			{
				break;
			}else if((App_terminalStatus != APP_STATUS_ERROR_OK) && (App_terminalStatus != APP_STATUS_NO_OP))
			return APP_STATUS_ERROR_NOK;
		} while (App_terminalStatus == APP_STATUS_NO_OP);
		
		if (pu8_data[9] == '\0')
		break;
		if(Terminal_Out((uint8_t*)"Invalid PAN, Only 9 characters") != TERMINAL_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	} while (1);
	return APP_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppADMIN_getCardPIN
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_data - Pointer to variable to hold the input CARD PIN by terminal.
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to get the card pin.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppADMIN_getCardPIN(uint8_t* pu8_data)
{
	enuApp_Status_t App_terminalStatus = APP_STATUS_ERROR_OK;
	Terminal_enablePasswordMode();
	do
	{
		if(Terminal_Out((uint8_t*)"Card PIN: ") != TERMINAL_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
		
		do
		{
			App_terminalStatus = App_ReportTerminal(pu8_data);
			if(App_terminalStatus == APP_STATUS_ERROR_OK)
			{
				break;
			}else if((App_terminalStatus != APP_STATUS_ERROR_OK) && (App_terminalStatus != APP_STATUS_NO_OP))
			return APP_STATUS_ERROR_NOK;
		} while (App_terminalStatus == APP_STATUS_NO_OP);
		
		if (pu8_data[4] == '\0')
		break;
		if(Terminal_Out((uint8_t*)"Invalid PIN, Only 4 characters") != TERMINAL_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	} while (1);
	Terminal_disablePasswordMode();
	return APP_STATUS_ERROR_OK;
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppADMIN_saveCardData
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): pstr_CardData - Structure of data to be saved in EEPROM.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to save the Name, PAN and the PIN of the Card in the EEPROM
*			   Also this function sets the INIT Flag in the memory.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppADMIN_saveCardData(strCardData_t* pstr_CardData)
{
	if(Eeprom_24_writePage(CARD_NAME_PAGE_NUM, (pstr_CardData->au8_cardHolderName)) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	Delay_ms(10);
	if(Eeprom_24_writePage(CARD_PAN_PAGE_NUM, (pstr_CardData->au8_primaryAccountNumber)) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	Delay_ms(10);
	if(Eeprom_24_writePage(CARD_PIN_PAGE_NUM, (pstr_CardData->au8_pinNum)) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	Delay_ms(10);
	if(Eeprom_24_writeByte(CARD_INIT_ADDRESS, CARD_INITIALIZED) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	
	return APP_STATUS_ERROR_OK;
}