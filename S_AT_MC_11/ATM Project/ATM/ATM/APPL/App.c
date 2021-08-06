/*
 * App.c
 *
 * Created: 7/16/2021 1:22:50 PM
 *  Author: Ahmed Nabil
 */ 

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "App.h"

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- GLOBAL VARIABLES *-*-*-*-*-*/
static enuApp_Status_t enuCurrentAppStatus = APP_STATUS_UNINITIALIZED;
static uint8_t gu8_ATMMode = ATM_MODE_IDLE;
static uint8_t gu8_USER_Mode_State = USER_IDLE;
static uint8_t gu8_ADMIN_Request = ADMIN_NOT_REQUESTED;
uint8_t cgu8_ATM_Req = ATM_NOT_REQUESTED;
uint8_t gu8_displayFlag = 0;
static strCardData_t  gstr_userCardData = {0};
strClientData_t	gstr_clientdata = {0};
uint8_t gu8_initData = 0;
uint8_t gu8_registeredAccNum = 0;
uint8_t gu8_ATMPin[5]={0};
uint8_t gau8_maxAmount[8]={0};	
uint8_t gu8_clientIndex = 0;


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- CONST VARIABLES *-*-*-*-*-*/
const uint8_t cgau8_ADMIN_Word[6] = "ADMIN";
const uint8_t ATM_TERM_NEW_CUSTOMER[2] = "1";
const uint8_t ATM_TERM_MAX_AMOUNT[2] = "2";
const uint8_t ATM_TERM_EXIT[2] = "3";
const uint8_t ATM_DB_ATM_PIN_VAL[5] = "8520";
const uint8_t cgau8_LoadingString[] = "Loading..";
const uint8_t cgau8_ATMString[] = "ATM Terminal\r";
uint8_t cgau8_AMOUNTString[] = "Enter Amount";
#include "AppADMIN.h"
#include "AppUSER.h"
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
	if(LCD_STATUS_ERROR_OK != Lcd_init())
		return APP_STATUS_ERROR_NOK;
	if(TERMINAL_STATUS_ERROR_OK != Terminal_init())
		return APP_STATUS_ERROR_NOK;
	if(EEPROM_24_STATUS_ERROR_OK != Eeprom_24_init())
		return APP_STATUS_ERROR_NOK;
	if(SPI_STATUS_ERROR_OK != Spi_init())
		return APP_STATUS_ERROR_NOK;
	if(KEYPAD_STATUS_ERROR_OK != Keypad_init())
		return APP_STATUS_ERROR_NOK;
	if(LM35_STATUS_ERROR_OK != LM35_init())
		return APP_STATUS_ERROR_NOK;
	if(MOTOR_STATUS_ERROR_OK != Motor_init())
		return APP_STATUS_ERROR_NOK;
	if(BTTN_STATUS_ERROR_OK != Button_init())
		return APP_STATUS_ERROR_NOK;
	/**************************/
	/* Only for Testing */
// 	if(Eeprom_24_writeByte(ATM_DB_FLAG_ADDR, 0xFF) != EEPROM_24_STATUS_ERROR_OK)
// 		return APP_STATUS_ERROR_NOK;
// 	Delay_ms(10);
	/**************************/
	if(Lcd_printString((uint8_t*)cgau8_LoadingString) != LCD_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	if(Terminal_Out((uint8_t*)cgau8_ATMString) != TERMINAL_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	
	/* Check if the ATM was previously registered and has its database in the eeprom */
	if(Eeprom_24_readByte(ATM_DB_FLAG_ADDR, &gu8_initData) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	Delay_ms(10);
	
	if(gu8_initData != ATM_DB_FLAG_SET_VAL)
	{ /* Case not previously registered */
		gu8_ATMMode = ATM_MODE_ADMIN; // Set in ADMIN mode
		/* Read the Admin PIN from the memory */
		if(Eeprom_24_writePacket(ATM_DB_ATM_PIN_ADDR, (uint8_t*)ATM_DB_ATM_PIN_VAL,\
								stringLength((uint8_t*)ATM_DB_ATM_PIN_VAL)) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		/* Lock the ATM For the User */
		Lcd_printLCD((uint8_t*)"******ATM*******", (uint8_t*)"*****LOCKED*****");
		
		if(Terminal_Out((uint8_t*)"\nProgramming Mode\r") != TERMINAL_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
	}else
	{ /* Case it was previously registered == Contains data in the DB */
		gu8_ATMMode = ATM_MODE_USER;
		
		if(Eeprom_24_readPacket(ATM_DB_MAX_AMNT_ADDR, gau8_maxAmount, 8) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		if(Eeprom_24_readByte(ATM_DB_ACC_NUM_ADDR, &gu8_registeredAccNum) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		if(Eeprom_24_readPacket(ATM_DB_ATM_PIN_ADDR, gu8_ATMPin, 5) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		
		Lcd_printLCD((uint8_t*)"1.Insert Card", (uint8_t*)"2.Display Temp");
			
		if(Terminal_Out((uint8_t*)"\nUSER Mode\r") != TERMINAL_STATUS_ERROR_OK)
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
	enuApp_Status_t App_terminalStatus = APP_STATUS_ERROR_OK;
	uint8_t au8_termInput[10] = {0};
	static uint8_t u8_passFlag = 0;
	if(gu8_ATMMode == ATM_MODE_ADMIN)
	{ /**************** PROGRAMMING MODE ****************/
			uint8_t au8_Input[2]={0};
			uint8_t au8_tempPinNum[5]={0};
			uint8_t au8_tempMaxAmount[8]={0};
			enuApp_Status_t PIN_Status = 0;
			if(u8_passFlag == 0)
			{
				/* Get the ATM PIN from the ADMIN Terminal and check if it's correct */
				PIN_Status = AppADMIN_getAtmPIN(au8_tempPinNum);
				if(PIN_Status == APP_STATUS_PIN_NOT_CORRECT)
					return APP_STATUS_ERROR_OK;
				else if(PIN_Status != APP_STATUS_PIN_CORRECT)
					return APP_STATUS_ERROR_NOK;
				u8_passFlag = 1;
			}
			
			Terminal_Out((uint8_t*)"\n1.Add New Customer\n\r2.Update Existing Customer\n\r3.Update Max Amount\n\r4.Exit\r");
			
			AppADMIN_getInput(au8_Input);
			if(au8_Input[0] == '1')//--------- New Customer OPTION -----------//
			{
				AppADMIN_processNewCustomer();
				EmptyString(au8_Input);

			}else if(au8_Input[0] == '2')//--------- Update Existing Customer OPTION -----------//
			{
				AppADMIN_processExistingCustomer();
				EmptyString(au8_Input);
			}else if(au8_Input[0] == '3')//--------- Max Amount OPTION -----------//
			{
				AppADMIN_getnewMaxAmount(au8_tempMaxAmount);
				if(Eeprom_24_writePacket(ATM_DB_MAX_AMNT_ADDR, au8_tempMaxAmount, stringLength(au8_tempMaxAmount)) != EEPROM_24_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				stringCopy(au8_tempMaxAmount, gau8_maxAmount);	
			}else if(au8_Input[0] == '4')//--------- EXIT OPTION -----------//
			{
				if(gu8_initData == ATM_DB_FLAG_SET_VAL)
				{
					u8_passFlag = 0;
					gu8_ATMMode = ATM_MODE_USER;
					Lcd_clear();
					if(Terminal_Out((uint8_t*)"\nUSER Mode\r") != TERMINAL_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					
					Lcd_printLCD((uint8_t*)"1.Insert Card", (uint8_t*)"2.Display Temp");
					
					return APP_STATUS_ERROR_OK;
				}else
				{
					if(Terminal_Out((uint8_t*)"\nData Base is empty, you must register at least one customer\r") != TERMINAL_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					return APP_STATUS_ERROR_OK;
				}
			}else
			{
				if(Terminal_Out((uint8_t*)"\nInvalid Input!") != TERMINAL_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
			}
	/****************************************************************/
	}else
	{							/************* User Mode **************/
		/********************************************************************************************/
		/************************************ ADMIN ENTERED ****************************************/
		/********************************************************************************************/
			/********************Handling ADMIN Requesting Programming Mode************************/
			if((gu8_ADMIN_Request == ADMIN_REQUESTED) && (gu8_USER_Mode_State == USER_IDLE))
			{
				gu8_ATMMode = ATM_MODE_ADMIN;
			}
			App_terminalStatus = App_ReportTerminal(au8_termInput);
			if(App_terminalStatus == APP_STATUS_ERROR_OK)
			{
				if (stringCompare((uint8_t*) au8_termInput,(uint8_t*) cgau8_ADMIN_Word) == 1) /* User Wants to enter ADMIN Mode */
				{
					if(gu8_USER_Mode_State == USER_BUSY) /* CARD is Busy in transaction */
					{
						gu8_ADMIN_Request = ADMIN_REQUESTED;
						if(Terminal_Out((uint8_t*)"\nATM is Busy Now, Programming Mode will start after current Process\r") != TERMINAL_STATUS_ERROR_OK)
							return APP_STATUS_ERROR_NOK;
					}else /* User Mode is Idle ==> The ATM is not used by User */
					{
						gu8_ATMMode = ATM_MODE_ADMIN;
						
						Lcd_printLCD((uint8_t*)"******ATM*******", (uint8_t*)"*****LOCKED*****");
						
						if(Terminal_Out((uint8_t*)"\nProgramming Mode\r") != TERMINAL_STATUS_ERROR_OK)
							return APP_STATUS_ERROR_NOK;
						return APP_STATUS_ERROR_OK;
					}
				}
			}else if((App_terminalStatus != APP_STATUS_ERROR_OK) && (App_terminalStatus != APP_STATUS_NO_OP))
				return APP_STATUS_ERROR_NOK;
			/****************************************************************/
			/********************** Getting Input from User ************************/
			uint8_t au8_data=0;
			enuKeypad_Status_t enuKeypadStatus = Keypad_readKey(&au8_data);
			if(enuKeypadStatus == KEYPAD_STATUS_PRESSED)
			{
			/************************************ KEYPAD PRESSED ****************************************/
			/********* Insert Card Chosen ********/
				if(au8_data == '1') 
				{
					AppUSER_insertCard();
			/******* Display Temp Chosen *******/
				}else if(au8_data == '2')
				{
					AppUSER_displayTemp();
				}else if(au8_data == '-') 
				{
					gu8_displayFlag = 0;
					Lcd_printLCD((uint8_t*)"1.Insert Card", (uint8_t*)"2.Display Temp");
				}
			}else if(gu8_displayFlag == DISPLAY_TEMP) /* If in temperature screen >> Update the temp value */
			{
				uint8_t u8_currentTemp=0;
				if (LM35_readTemp(&u8_currentTemp) == LM35_STATUS_ERROR_OK)
				{
					if(Lcd_setCursor(1, 0) != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
					if(Lcd_printDecimal(u8_currentTemp) != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				}
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



