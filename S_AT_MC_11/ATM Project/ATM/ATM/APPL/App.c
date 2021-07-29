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
static uint8_t gu8_ATMMode = ATM_MODE_IDLE;
const uint8_t cgau8_ADMIN_Word[6] = "ADMIN";
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
const uint8_t ATM_TERM_NEW_CUSTOMER[2] = "1";
const uint8_t ATM_TERM_MAX_AMOUNT[2] = "2";
const uint8_t ATM_TERM_EXIT[2] = "3";
uint8_t ATM_DB_ATM_PIN_VAL[5] = "8520";
const uint8_t cgau8_LoadingString[] = "Loading..";
const uint8_t cgau8_ATMString[] = "ATM Terminal\r";
//uint8_t gu8_cardState = CARD_OUT;
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
	if(KEYPAD_STATUS_ERROR_OK != Keypad_init())
		return APP_STATUS_ERROR_NOK;
	if(LCD_STATUS_ERROR_OK != Lcd_init())
		return APP_STATUS_ERROR_NOK;
	if(LM35_STATUS_ERROR_OK != LM35_init())
		return APP_STATUS_ERROR_NOK;
	if(MOTOR_STATUS_ERROR_OK != Motor_init())
		return APP_STATUS_ERROR_NOK;
	if(BTTN_STATUS_ERROR_OK != Button_init())
		return APP_STATUS_ERROR_NOK;
	/**************************/
	/* Only for Testing */
//	DIO_PORTC_DIR |= 1<<3;
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
	{
		gu8_ATMMode = ATM_MODE_ADMIN;
		
		if(Eeprom_24_writePacket(ATM_DB_ATM_PIN_ADDR, ATM_DB_ATM_PIN_VAL,stringLength(ATM_DB_ATM_PIN_VAL)) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		
		if(Lcd_setCursor(0,0) != LCD_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_OK;
		if(Lcd_printString((uint8_t*)"*******ATM******") != LCD_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		if(Lcd_setCursor(1,0) != LCD_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_OK;
		if(Lcd_printString((uint8_t*)"*****LOCKED*****") != LCD_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		
		if(Terminal_Out((uint8_t*)"Programming Mode\r") != TERMINAL_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
	}else
	{//gu8_ATMPin
		gu8_ATMMode = ATM_MODE_USER;
		
		if(Eeprom_24_readPacket(ATM_DB_MAX_AMNT_ADDR, gau8_maxAmount, 8) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		if(Eeprom_24_readByte(ATM_DB_ACC_NUM_ADDR, &gu8_registeredAccNum) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		if(Eeprom_24_readPacket(ATM_DB_ATM_PIN_ADDR, gu8_ATMPin, 5) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		
		if(Lcd_setCursor(0,0) != LCD_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_OK;
		if(Lcd_printString((uint8_t*)"1.Insert Card") != LCD_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		if(Lcd_setCursor(1,0) != LCD_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_OK;
		if(Lcd_printString((uint8_t*)"2.Display Temp") != LCD_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
			
		if(Terminal_Out((uint8_t*)"USER Mode\r") != TERMINAL_STATUS_ERROR_OK)
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
			uint8_t au8_tempPAN[10]={0};
			uint8_t au8_tempBalance[8]={0};
			uint8_t au8_Input[2]={0};
			uint8_t au8_tempPinNum[5]={0};
			uint8_t au8_tempMaxAmount[8]={0};
			
			if(u8_passFlag == 0)
			{
				/* Get the ATM PIN from the ADMIN Terminal and check if it's correct */
				if(AppADMIN_getAtmPIN(au8_tempPinNum) != APP_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				u8_passFlag = 1;
			}
			
			if(Terminal_Out((uint8_t*)"1.Add New Customer\r2.Update Max Amount\r3.Exit\r") != TERMINAL_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				
			AppADMIN_getInput(au8_Input);
			if(au8_Input[0] == '1')//--------- New Customer OPTION -----------//
			{
				/* Get the Customer's Primary Account Number from the ADMIN Terminal */
				if(AppADMIN_getCustomerPAN(au8_tempPAN) != APP_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				stringCopy(au8_tempPAN, gstr_clientdata.au8_PAN);
				
				/* Get the Customer's Balance from the ADMIN Terminal */
				if(AppADMIN_getCustomerBalance(au8_tempBalance) != APP_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				stringCopy(au8_tempBalance, gstr_clientdata.au8_Balance);
				
				if(AppADMIN_saveNewCustomerData() != APP_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				EmptyString(au8_Input);
				if(gu8_registeredAccNum == 1)
				{
					gu8_initData = ATM_DB_FLAG_SET_VAL;
					if(Eeprom_24_writeByte(ATM_DB_FLAG_ADDR, ATM_DB_FLAG_SET_VAL) != EEPROM_24_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				}

			}else if(au8_Input[0] == '2')//--------- Max Amount OPTION -----------//
			{
				AppADMIN_getnewMaxAmount(au8_tempMaxAmount);
				if(Eeprom_24_writePacket(ATM_DB_MAX_AMNT_ADDR, au8_tempMaxAmount, stringLength(au8_tempMaxAmount)) != EEPROM_24_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				stringCopy(au8_tempMaxAmount, gau8_maxAmount);
					
			}else if(au8_Input[0] == '3')//--------- EXIT OPTION -----------//
			{
				if(gu8_initData == ATM_DB_FLAG_SET_VAL)
				{
					u8_passFlag = 0;
					gu8_ATMMode = ATM_MODE_USER;
					Lcd_clear();
					if(Terminal_Out((uint8_t*)"USER Mode\r") != TERMINAL_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					if(Lcd_setCursor(0,0) != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_OK;
					if(Lcd_printString((uint8_t*)"1.Insert Card") != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					if(Lcd_setCursor(1,0) != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_OK;
					if(Lcd_printString((uint8_t*)"2.Display Temp") != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					
					return APP_STATUS_ERROR_OK;
				}else
				{
					if(Terminal_Out((uint8_t*)"Data Base is empty, you must register at least one customer\r") != TERMINAL_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					return APP_STATUS_ERROR_OK;
				}
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
						if(Terminal_Out((uint8_t*)"ATM is Busy Now, Programming Mode will start after current Process\r") != TERMINAL_STATUS_ERROR_OK)
							return APP_STATUS_ERROR_NOK;
					}else /* User Mode is Idle ==> The ATM is not used by User */
					{
						gu8_ATMMode = ATM_MODE_ADMIN;
						Lcd_clear();
						if(Lcd_setCursor(0,0) != LCD_STATUS_ERROR_OK)
							return APP_STATUS_ERROR_OK;
						if(Lcd_printString((uint8_t*)"*******ATM******") != LCD_STATUS_ERROR_OK)
							return APP_STATUS_ERROR_NOK;
						if(Lcd_setCursor(1,0) != LCD_STATUS_ERROR_OK)
							return APP_STATUS_ERROR_OK;
						if(Lcd_printString((uint8_t*)"*****LOCKED*****") != LCD_STATUS_ERROR_OK)
							return APP_STATUS_ERROR_NOK;
						if(Terminal_Out((uint8_t*)"Programming Mode\r") != TERMINAL_STATUS_ERROR_OK)
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
			/********************************************************************************************/
			/************************************ KEYPAD PRESSED ****************************************/
			/********************************************************************************************/
				/********* Insert Card Chosen ********/
				if(au8_data == '1') 
				{
					gu8_USER_Mode_State = USER_BUSY;
					uint8_t u8_buttonVal = PIN_LOW;
					uint8_t au8_inputString[MAX_INPUT_SIZE]={0};
					Lcd_clear();
					if(Lcd_setCursor(0,0) != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_OK;
					if(Lcd_printString((uint8_t*)"Waiting for Card") != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					if(Lcd_setCursor(1,0) != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_OK;
					if(Lcd_printString((uint8_t*)"Press Button ;)") != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					while(u8_buttonVal == PIN_LOW)
					{
						if(Button_updateState(BUTTON_CARD)!=BTTN_STATUS_ERROR_OK)
							return APP_STATUS_ERROR_NOK;
						if(Button_getState(BUTTON_CARD, &u8_buttonVal) != BTTN_STATUS_ERROR_OK)
							return APP_STATUS_ERROR_NOK;
					}
					Dio_writePin(DIO_SIG_CHANNEL_ID, PIN_LOW);
					Delay_ms(1);
					Dio_writePin(DIO_SIG_CHANNEL_ID, PIN_HIGH);
					while (au8_inputString[0] != '#')
					{
						if(Spi_SlaveReceivePacket(au8_inputString, CARD_FRAME_LENGTH) != SPI_STATUS_ERROR_OK)
							return APP_STATUS_ERROR_NOK;
					}
					
					if(AppUSER_getCardData(&gstr_userCardData, au8_inputString) != APP_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					Dio_writePin(DIO_SIG_CHANNEL_ID, PIN_LOW);
					Delay_ms(1);
					Dio_writePin(DIO_SIG_CHANNEL_ID, PIN_HIGH);
					if(AppUSER_startProcess(&gstr_userCardData) != APP_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
			/********************************************************************************************/
			/********************************************************************************************/
			/********************************************************************************************/
			/******* Display Temp Chosen *******/
				}else if(au8_data == '2')
				{
					uint8_t u8_currentTemp=0;
					gu8_displayFlag = DISPLAY_TEMP;
					Lcd_clear();
					if(Lcd_setCursor(0, 0) != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					if(Lcd_printString((uint8_t*)"Current Temp is:") != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					if (LM35_readTemp(&u8_currentTemp) == LM35_STATUS_ERROR_OK)
					{
						if(Lcd_setCursor(1, 0) != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
						if(Lcd_printDecimal(u8_currentTemp) != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					}
					if(Lcd_printString((uint8_t*)"   '-' To Exit") != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
				}else if(au8_data == '-')
				{
					gu8_displayFlag = 0;
					Lcd_clear();
					if(Lcd_setCursor(0,0) != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_OK;
					if(Lcd_printString((uint8_t*)"1.Insert Card") != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					if(Lcd_setCursor(1,0) != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_OK;
					if(Lcd_printString((uint8_t*)"2.Display Temp") != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
				}
			}else if(gu8_displayFlag == DISPLAY_TEMP)
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
	
	return APP_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppUSER_startProcess
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): pstr_CardData - Pointer to Card Data to be processed.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSrvc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to start processing the card.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppUSER_startProcess(strCardData_t* pstr_CardData)
{
	Lcd_clear();
	if(Lcd_setCursor(0, 0) != LCD_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	if(Lcd_printString((uint8_t*)"Enter Your PIN") != LCD_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	if(AppUSER_checkPin() != APP_STATUS_PIN_CORRECT)
		return APP_STATUS_ERROR_NOK;
	
	if(AppUSER_checkPan() != APP_STATUS_PAN_FOUND)
		return APP_STATUS_ERROR_NOK;
	
	Lcd_clear();
	if(Lcd_setCursor(0, 0) != LCD_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	if(Lcd_printString((uint8_t*)"Enter Amount") != LCD_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	if(AppUSER_startTransaction() != APP_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	
	return APP_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppUSER_startTransaction
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSrvc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to check the card's pan if valid in DB or not.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppUSER_startTransaction(void)
{
	uint8_t au8_inputKPD[8] = {0};
	enuApp_Status_t KeypdaStatus = 0;
	while(1)
	{
		EmptyString(au8_inputKPD);
		KeypdaStatus = AppUSER_ReportKeypad(au8_inputKPD);
		
		if(KeypdaStatus == APP_STATUS_KPD_NUM)
		{
			Terminal_Out(au8_inputKPD);
			Terminal_Out(gau8_maxAmount);
			if(stringCompare(gau8_maxAmount, au8_inputKPD) == 3)
			{
				Lcd_clear();
				if(Lcd_setCursor(0, 0) != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				if(Lcd_printString((uint8_t*)"Max Amount") != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				if(Lcd_setCursor(1, 0) != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				if(Lcd_printString((uint8_t*)"Exceeded") != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				Delay_ms(500);
				Lcd_clear();
				if(Lcd_setCursor(0, 0) != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				if(Lcd_printString((uint8_t*)"Enter Amount") != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
			}else
			{
				Lcd_clear();
				if(Lcd_setCursor(0, 0) != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				if(Lcd_printString((uint8_t*)"Please Wait") != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				if(Lcd_setCursor(1, 0) != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				if(Lcd_printString((uint8_t*)"Processing...") != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				if(stringCompare(gstr_clientdata.au8_Balance, au8_inputKPD) == 3)
				{
					Lcd_clear();
					if(Lcd_setCursor(0, 0) != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					if(Lcd_printString((uint8_t*)"Insufficient") != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					if(Lcd_setCursor(1, 0) != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					if(Lcd_printString((uint8_t*)"Fund") != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					Delay_ms(500);
					Lcd_clear();
					if(Lcd_setCursor(0, 0) != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
					if(Lcd_printString((uint8_t*)"Enter Amount") != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
				}else/****************** Transaction Successfull ******************/
				{
					Lcd_clear();
					if(Lcd_setCursor(0, 0) != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
					if(Lcd_printString((uint8_t*)"Successful") != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
					if(Lcd_setCursor(1, 0) != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
					if(Lcd_printString((uint8_t*)"Transaction") != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
					Motor_run(MOTOR_CASH_ID, 100, MOTOR_DIR_CLK_WISE);
					Delay_ms(1000);
					Motor_stop(MOTOR_CASH_ID);
					gu8_USER_Mode_State = USER_IDLE;
					Lcd_clear();
					if(Lcd_printString((uint8_t*)"1.Insert Card") != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
					if(Lcd_setCursor(1,0) != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_OK;
					if(Lcd_printString((uint8_t*)"2.Display Temp") != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
					
					return APP_STATUS_ERROR_OK;
				}
			}
		}
		Delay_ms(155);
	}
}
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppUSER_checkPan
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSrvc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to check the card's pan if valid in DB or not.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppUSER_checkPan(void)
{
	uint8_t u8_clientIndex = 0;
	uint8_t u8_clientPanAddr = 0;
	uint8_t au8_clientPAN[10] = {0};
	
	for (u8_clientIndex=0; u8_clientIndex<gu8_registeredAccNum; u8_clientIndex++)
	{
		u8_clientPanAddr = ATM_DB_CUSTOMER_PAN_BASE_ADDR + u8_clientIndex*16;
		if(Eeprom_24_readPacket(u8_clientPanAddr, au8_clientPAN, MAX_PAN_LENGTH+1) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		if(stringCompare(au8_clientPAN, gstr_userCardData.au8_primaryAccountNumber) == 1)
			break;
	}	
	
	uint8_t u8_clientBalAddr = ATM_DB_CUSTOMER_BAL_BASE_ADDR + u8_clientIndex*16;
	
	stringCopy(au8_clientPAN, gstr_clientdata.au8_PAN);
	
	if(Eeprom_24_readPacket(u8_clientBalAddr, gstr_clientdata.au8_Balance, MAX_BAL_LENGTH+1) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	
	return APP_STATUS_PAN_FOUND;
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppUSER_checkPin
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSrvc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to check the user's input pin if right or not.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppUSER_checkPin(void)
{
	uint8_t au8_inputKPD[5] = {0};
	enuApp_Status_t KeypdaStatus = 0;
	while(1)
	{
		EmptyString(au8_inputKPD);
		KeypdaStatus = AppUSER_ReportKeypad(au8_inputKPD);
		
		if(KeypdaStatus == APP_STATUS_KPD_NUM)
		{
			if(stringCompare(au8_inputKPD, gstr_userCardData.au8_pinNum) != 1)
			{
				Lcd_clear();
				if(Lcd_setCursor(0, 0) != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				if(Lcd_printString((uint8_t*)"Incorrect PIN") != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				if(Lcd_setCursor(1, 0) != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				if(Lcd_printString((uint8_t*)"Try Again") != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				Delay_ms(500);
				Lcd_clear();
				if(Lcd_setCursor(0, 0) != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				if(Lcd_printString((uint8_t*)"Enter Your PIN") != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
			}else
			{
				Lcd_clear();
				if(Lcd_setCursor(0, 0) != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				if(Lcd_printString((uint8_t*)"Please Wait") != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				if(Lcd_setCursor(1, 0) != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				if(Lcd_printString((uint8_t*)"Processing...") != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				
				return APP_STATUS_PIN_CORRECT;
			}
		}
		Delay_ms(155);
	}	
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppUSER_ReportKeypad
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_key - Pointer to variable to hold the button pressed
* Return value: enuSrvc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to report the value pressed in the keypad.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppUSER_ReportKeypad(uint8_t* pu8_key)
{
	static uint8_t u8_index = 0;
	static uint8_t sau8_data[10]={0};
	enuKeypad_Status_t enuKeypadStatus = Keypad_readKey(&sau8_data[u8_index]);
	
	if(enuKeypadStatus == KEYPAD_STATUS_PRESSED)
	{
		if(sau8_data[u8_index] == '=')
		{
			sau8_data[u8_index] = '\0';
			u8_index = 0;
			if(Lcd_setCursor(LCD_IN_POS_X, LCD_IN_POS_Y) != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
			if(Lcd_printString((uint8_t*)"  ") != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
			if(Lcd_setCursor(LCD_IN_POS_X, LCD_IN_POS_Y) != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
			stringCopy(sau8_data, pu8_key);
			return APP_STATUS_KPD_NUM;
		}else if(sau8_data[u8_index] == '-')
		{
			u8_index--;
			sau8_data[u8_index] = '\0';
			if(Lcd_setCursor(LCD_IN_POS_X, u8_index+LCD_IN_POS_Y) != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
			if(Lcd_printChar(' ') != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
		}else
		{
			if(u8_index!=10)
			{
				if(Lcd_setCursor(LCD_IN_POS_X, u8_index+LCD_IN_POS_Y) != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				if(Lcd_printChar(sau8_data[u8_index]) != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				u8_index++;
			}else
			{
				sau8_data[u8_index] = '\0';
			}
			
		}
	}else if(enuKeypadStatus == KEYPAD_STATUS_NOT_PRESSED)
	{
		return APP_STATUS_ERROR_OK;
	}
	
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
* Service Name: AppADMIN_getInput
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_data - Pointer to variable to hold the input Customer's PAN by terminal.
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to get the ADMIN Input Choice.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppADMIN_getInput(uint8_t* pu8_data)
{
	enuApp_Status_t App_terminalStatus = APP_STATUS_ERROR_OK;
	do
	{
		if(Terminal_Out((uint8_t*)"Your Input: ") != TERMINAL_STATUS_ERROR_OK)
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
		
		if (pu8_data[1] == '\0')
		break;
		EmptyString(pu8_data);
		if(Terminal_Out((uint8_t*)"Invalid Choice, Only Choose from 1 to 3\r") != TERMINAL_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	} while (1);
	return APP_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppADMIN_getnewMaxAmount
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_data - Pointer to variable to hold the input Customer's PAN by terminal.
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to get the customer's pan.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppADMIN_getnewMaxAmount(uint8_t* pu8_data)
{
	enuApp_Status_t App_terminalStatus = APP_STATUS_ERROR_OK;
	do
	{
		if(Terminal_Out((uint8_t*)"Max Amount: ") != TERMINAL_STATUS_ERROR_OK)
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
		
		if ((pu8_data[7] == '\0') && (pu8_data[4] == '.'))
			break;
		EmptyString(pu8_data);
		if(Terminal_Out((uint8_t*)"Invalid Balance, Only 7 characters eg. 1234.56\r") != TERMINAL_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
	} while (1);
	return APP_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppADMIN_getCustomerPAN
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_data - Pointer to variable to hold the input Customer's PAN by terminal.
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to get the customer's pan.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppADMIN_getCustomerPAN(uint8_t* pu8_data)
{
	enuApp_Status_t App_terminalStatus = APP_STATUS_ERROR_OK;
	do
	{
		if(Terminal_Out((uint8_t*)"PAN: ") != TERMINAL_STATUS_ERROR_OK)
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
		EmptyString(pu8_data);
		if(Terminal_Out((uint8_t*)"Invalid PAN, Only 9 characters\r") != TERMINAL_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	} while (1);
	return APP_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppADMIN_getCustomerBalance
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_data - Pointer to variable to hold the input Balance by terminal.
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to get the balance of the customer.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppADMIN_getCustomerBalance(uint8_t* pu8_data)
{
	enuApp_Status_t App_terminalStatus = APP_STATUS_ERROR_OK;
	do
	{
		if(Terminal_Out((uint8_t*)"Balance: ") != TERMINAL_STATUS_ERROR_OK)
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
		
		if (pu8_data[7] == '\0')
		break;
		EmptyString(pu8_data);
		if(Terminal_Out((uint8_t*)"Invalid Balance, Only 7 characters\r") != TERMINAL_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	} while (1);
	return APP_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppADMIN_getAtmPIN
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_data - Pointer to variable to hold the input ATM PIN by terminal.
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to get the atm pin.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppADMIN_getAtmPIN(uint8_t* pu8_data)
{
	enuApp_Status_t App_terminalStatus = APP_STATUS_ERROR_OK;
	
	Terminal_enablePasswordMode();
	do
	{
		if(Terminal_Out((uint8_t*)"ATM PIN: ") != TERMINAL_STATUS_ERROR_OK)
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
		
		if(stringCompare(ATM_DB_ATM_PIN_VAL ,pu8_data) != 1)
		{
			if(Terminal_Out((uint8_t*)"Incorrect PIN\r") != TERMINAL_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
		}else
		{
			if(Terminal_Out((uint8_t*)"Loading...\r") != TERMINAL_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
			break;
		}
		EmptyString(pu8_data);
		if(Terminal_Out((uint8_t*)"Invalid PIN, Only 4 characters\r") != TERMINAL_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
	} while (1);
	Terminal_disablePasswordMode();
	return APP_STATUS_ERROR_OK;
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppADMIN_saveNewCustomerData
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to save PAN and Balance of a new Customer in the EEPROM
*			   Also this function sets the INIT Flag in the memory.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppADMIN_saveNewCustomerData(void)
{
	uint8_t u8_newCustomerPanAddr = ATM_DB_CUSTOMER_PAN_BASE_ADDR + gu8_registeredAccNum*16 ;
	uint8_t u8_newCustomerBalAddr = ATM_DB_CUSTOMER_BAL_BASE_ADDR + gu8_registeredAccNum*16 ;
	
	if(Eeprom_24_writePacket(u8_newCustomerPanAddr, gstr_clientdata.au8_PAN, stringLength(gstr_clientdata.au8_PAN)) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
		
	if(Eeprom_24_writePacket(u8_newCustomerBalAddr, gstr_clientdata.au8_Balance, stringLength(gstr_clientdata.au8_Balance)) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	
	if(Eeprom_24_writeByte(ATM_DB_ACC_NUM_ADDR, ++gu8_registeredAccNum) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	
	return APP_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppUSER_getCardData
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): pu8_dataString - Frame of card's data.
* Parameters (inout): None
* Parameters (out): pstr_CardData - Structure to save the card's data in.
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to save the Name, PAN and the PIN of the Card in the Card data Struct
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppUSER_getCardData(strCardData_t* pstr_CardData, uint8_t *pu8_dataString)
{
	
	uint8_t u8_frameIndex=0;
	uint8_t u8_dataIndex=0;
	/****************** Get Card Name **********************/
	for(u8_frameIndex=1; u8_frameIndex<MAX_NAME_LENGTH+1 ; u8_frameIndex++)
	{
		pstr_CardData->au8_cardHolderName[u8_dataIndex] = pu8_dataString[u8_frameIndex];
		u8_dataIndex++;
	}
	pstr_CardData->au8_cardHolderName[u8_dataIndex] = '\0';
	/****************** Get Card PAN **********************/
	u8_dataIndex=0;
	for(u8_frameIndex=11; u8_frameIndex<MAX_PAN_LENGTH+11 ; u8_frameIndex++)
	{
		pstr_CardData->au8_primaryAccountNumber[u8_dataIndex] = pu8_dataString[u8_frameIndex];
		u8_dataIndex++;
	}
	pstr_CardData->au8_primaryAccountNumber[u8_dataIndex] = '\0';
	/****************** Get Card PIN **********************/
	u8_dataIndex=0;
	for(u8_frameIndex=21; u8_frameIndex<MAX_PIN_LENGTH+21 ; u8_frameIndex++)
	{
		pstr_CardData->au8_pinNum[u8_dataIndex] = pu8_dataString[u8_frameIndex];
		u8_dataIndex++;
	}
	pstr_CardData->au8_pinNum[u8_dataIndex] = '\0';
	return APP_STATUS_ERROR_OK;
}