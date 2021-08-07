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
const uint8_t cgau8_ADMIN_Word[6] = "ADMIN";
static uint8_t gu8_USER_Mode_State = USER_IDLE;
static uint8_t gu8_ADMIN_Request = ADMIN_NOT_REQUESTED;
uint8_t cgu8_ATM_Req = ATM_NOT_REQUESTED;
static strCardData_t  gstr_userCardData = {0};
const uint8_t cgu8_ATM_SPI_CARD_Busy[3] = "CB";
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*- FUNCTIONS IMPLEMENTATION -*-*-*-*-*-*/
#if REQUEST_MODE == INTERRUPT_REQUEST
void ATM_REQ_ISR(void)
{	
	if((gu8_USER_Mode_State == USER_IDLE) && (cgu8_ATM_Req == ATM_NOT_REQUESTED))
	{
		/* ATM Request Data from Card */
		if (gu8_CardMode == CARD_MODE_USER)
		{
			cgu8_ATM_Req = ATM_REQUESTED;
		}else if(gu8_CardMode == CARD_MODE_ADMIN) // Card is in ADMIN Mode 
		{	//Send "**" to the atm to indicate that Card is in Admin mode
			SPI_SS_ENABLE();
			Spi_MasterSendPacket((uint8_t*)"**",3);
			SPI_SS_DISABLE();
			cgu8_ATM_Req = ATM_NOT_REQUESTED;
		}
	}
	else if((gu8_USER_Mode_State == USER_BUSY) && (cgu8_ATM_Req == ATM_NOT_REQUESTED))
	{	/* ATM CARD OUT*/
		gu8_USER_Mode_State = USER_IDLE;
	}
}
#endif
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
	Delay_ms(1000);
#if REQUEST_MODE == INTERRUPT_REQUEST
 	Ext_INT0_init(EXT_INT0_EDGE_FALL_RISE);
 	INT0_setCallBack(ATM_REQ_ISR);
#elif REQUEST_MODE == POLLING_REQUEST
	DIO_PORTD_DIR &= ~(1<<2);
	DIO_PORTD_DATA |= 1<<2;
#endif
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
	DIO_PORTD_DIR &= ~(1<<2);
	DIO_PORTD_DATA |= 1<<2;
	/**************************/
	/* Only for Testing */
// 	if(Eeprom_24_writeByte(CARD_INIT_ADDRESS, 0xFF) != EEPROM_24_STATUS_ERROR_OK)
// 		return APP_STATUS_ERROR_NOK;
// 	Delay_ms(10);
	/**************************/
	if(Terminal_Out((uint8_t*)"CARD Terminal Window\r\n") != TERMINAL_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	uint8_t u8_initData = 0;
	/* Check if the card was previously registered and has its data in the eeprom */
	if(Eeprom_24_readByte(CARD_INIT_ADDRESS, &u8_initData) != EEPROM_24_STATUS_ERROR_OK)
	{
		return APP_STATUS_ERROR_NOK;
	}
		
	if(u8_initData != CARD_INITIALIZED)
	{
		gu8_CardMode = CARD_MODE_ADMIN;
	}else
	{
		gu8_CardMode = CARD_MODE_USER;
		Terminal_Out((uint8_t*)"You are now in User Mode\r\n");
		AppMemory_getCardData(&gstr_userCardData);
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
	enuApp_Status_t App_terminalStatus = APP_STATUS_ERROR_OK;
	uint8_t au8_termInput[10] = {0};
	if(gu8_CardMode == CARD_MODE_ADMIN)
	{ /**************** PROGRAMMING MODE ****************/
			if(Terminal_Out((uint8_t*)"Programming Mode\r\nEnter the Following Data\r\n") != TERMINAL_STATUS_ERROR_OK)
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
		
			gu8_ADMIN_Request = ADMIN_NOT_REQUESTED;
			gu8_CardMode = CARD_MODE_USER;
			if(Terminal_Out((uint8_t*)"\nUSER Mode\r\n") != TERMINAL_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
	/****************************************************************/
	}else
	{/************* User Mode **************/
			/********************Handling ADMIN Requesting Programming Mode************************/
			if((gu8_ADMIN_Request == ADMIN_REQUESTED) && (gu8_USER_Mode_State == USER_IDLE))
			{
				gu8_CardMode = CARD_MODE_ADMIN;
			}
			App_terminalStatus = App_ReportTerminal(au8_termInput);
			if(App_terminalStatus == APP_STATUS_ERROR_OK)
			{
				if (stringCompare((uint8_t*) au8_termInput,(uint8_t*) cgau8_ADMIN_Word) == 1) /* User Wants to enter ADMIN Mode */
				{
					if(gu8_USER_Mode_State == USER_BUSY) /* CARD is Busy in transaction */
					{
						gu8_ADMIN_Request = ADMIN_REQUESTED;
						if(Terminal_Out((uint8_t*)"CARD is Busy Now, \rProgramming Mode will start after current Process\r\n") != TERMINAL_STATUS_ERROR_OK)
							return APP_STATUS_ERROR_NOK;
					}else /* User Mode is Idle ==> The Card is not used by ATM */
					{
						gu8_CardMode = CARD_MODE_ADMIN;
						return APP_STATUS_ERROR_OK;
					}
				}
			}else if((App_terminalStatus != APP_STATUS_ERROR_OK) && (App_terminalStatus != APP_STATUS_NO_OP))
				return APP_STATUS_ERROR_NOK;
			/****************************************************************/
			/************************* ATM Request **************************/
			if (cgu8_ATM_Req == ATM_REQUESTED)
			{
				gu8_USER_Mode_State = USER_BUSY;
				AppUSER_sendCardData(&gstr_userCardData);
				
				if(Terminal_Out((uint8_t*)"Data Successfully Sent\r\n") != TERMINAL_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				cgu8_ATM_Req = ATM_NOT_REQUESTED;
			}
			
	/****************************************************************/
	}
	
	Delay_ms(100);
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
		
		if(stringLength(pu8_data) != MAX_NAME_LENGTH+1) 
		{
			if(Terminal_Out((uint8_t*)"\nInvalid Name, Name should be 9 characters\r\n") != TERMINAL_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
		}else{break;}
			
		EmptyString(pu8_data);
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
		if(Terminal_Out((uint8_t*)"\nCard PAN: ") != TERMINAL_STATUS_ERROR_OK)
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
		if(stringLength(pu8_data) != MAX_PAN_LENGTH+1)
		{
			if(Terminal_Out((uint8_t*)"\nInvalid PAN, PAN should be 9 numeric characters\r\n") != TERMINAL_STATUS_ERROR_OK)	
				return APP_STATUS_ERROR_NOK;
			EmptyString(pu8_data);
			continue;
		}
		uint8_t u8_index=0;
		
		for(u8_index=0; u8_index<MAX_PAN_LENGTH; u8_index++)
		{
			if((pu8_data[u8_index]>'9') || (pu8_data[u8_index]<'0'))
			{
				if(Terminal_Out((uint8_t*)"\nInvalid PAN, PAN should be 9 numeric characters\r\n") != TERMINAL_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				EmptyString(pu8_data);
				break;
			}
		}
		if (u8_index == MAX_PAN_LENGTH)
		{
			break;
		}
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
		if(Terminal_Out((uint8_t*)"\nCard PIN: ") != TERMINAL_STATUS_ERROR_OK)
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
		
		
		if(stringLength(pu8_data) != MAX_PIN_LENGTH+1)
		{
			if(Terminal_Out((uint8_t*)"\nInvalid PIN, PIN should be 4 numeric characters\r\n") != TERMINAL_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
			EmptyString(pu8_data);
			continue;
		}
		uint8_t u8_index=0;
		
		for(u8_index=0; u8_index<MAX_PIN_LENGTH; u8_index++)
		{
			if((pu8_data[u8_index]>'9') || (pu8_data[u8_index]<'0'))
			{
				if(Terminal_Out((uint8_t*)"\nInvalid PIN, PIN should be 4 numeric characters\r\n") != TERMINAL_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				EmptyString(pu8_data);
				break;
			}
		}
		if (u8_index == MAX_PIN_LENGTH)
		{
			break;
		}
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
	if(Eeprom_24_writePacket(CARD_NAME_PAGE_ADDR, (pstr_CardData->au8_cardHolderName), MAX_NAME_LENGTH+1) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	Delay_ms(10);
	if(Eeprom_24_writePacket(CARD_PAN_PAGE_ADDR, (pstr_CardData->au8_primaryAccountNumber), MAX_PAN_LENGTH+1) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	Delay_ms(10);
	if(Eeprom_24_writePacket(CARD_PIN_PAGE_ADDR, (pstr_CardData->au8_pinNum), MAX_PIN_LENGTH+1) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	Delay_ms(10);
	if(Eeprom_24_writeByte(CARD_INIT_ADDRESS, CARD_INITIALIZED) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	
	return APP_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppUSER_sendCardData
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): pstr_CardData - Structure of data to be send to ATM.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to send the Card's Data to the ATM.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppUSER_sendCardData(strCardData_t* pstr_CardData)
{
	if(Terminal_Out((uint8_t*)"Data is being sent to the ATM...\r\n") != TERMINAL_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
		
	uint8_t au8_DataFrame[30] = {0};
	stringConcatenate(au8_DataFrame, (uint8_t*)"#");
	stringConcatenate(au8_DataFrame, pstr_CardData->au8_cardHolderName);
	stringConcatenate(au8_DataFrame, (uint8_t*)"-");
	stringConcatenate(au8_DataFrame, pstr_CardData->au8_primaryAccountNumber);
	stringConcatenate(au8_DataFrame, (uint8_t*)"-");
	stringConcatenate(au8_DataFrame, pstr_CardData->au8_pinNum);
	stringConcatenate(au8_DataFrame, (uint8_t*)"#-");
	
	SPI_SS_ENABLE();
	if(Spi_MasterSendPacket(au8_DataFrame, stringLength(au8_DataFrame)) != SPI_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	SPI_SS_DISABLE();
	
	return APP_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppMemory_getCardData
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pstr_CardData - Structure of data to hold the data saved in EEPROM.
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to get the Card's data from eeprom.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppMemory_getCardData(strCardData_t* pstr_CardData)
{
	if(Eeprom_24_readPacket(CARD_NAME_PAGE_ADDR, (pstr_CardData->au8_cardHolderName),MAX_NAME_LENGTH+1) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	Delay_ms(10);
	if(Eeprom_24_readPacket(CARD_PAN_PAGE_ADDR, (pstr_CardData->au8_primaryAccountNumber),MAX_PAN_LENGTH+1) != EEPROM_24_STATUS_ERROR_OK)
	return APP_STATUS_ERROR_NOK;
	Delay_ms(10);
	if(Eeprom_24_readPacket(CARD_PIN_PAGE_ADDR, (pstr_CardData->au8_pinNum),MAX_PIN_LENGTH+1) != EEPROM_24_STATUS_ERROR_OK)
	return APP_STATUS_ERROR_NOK;
	
	return APP_STATUS_ERROR_OK;
}