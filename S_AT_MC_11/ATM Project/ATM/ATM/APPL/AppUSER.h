/*
 * AppUSER.h
 *
 * Created: 7/30/2021 12:55:03 AM
 *  Author: Ahmed Nabil
 */ 


#ifndef APPUSER_H_
#define APPUSER_H_

uint8_t Kpd_enablePass = 0;
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppUSER_displayTemp
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSrvc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to display the temperature.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppUSER_displayTemp(void)
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
	return APP_STATUS_ERROR_OK;
}
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppUSER_insertCard
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSrvc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to run the Card Inserting Process.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppUSER_insertCard(void)
{
	gu8_USER_Mode_State = USER_BUSY;
	uint8_t u8_buttonVal = PIN_LOW;
	uint8_t au8_inputString[MAX_INPUT_SIZE]={0};
	
	Lcd_printLCD((uint8_t*)"Waiting for Card", (uint8_t*)"  Press Button");
	while(u8_buttonVal == PIN_LOW)
	{
		if(Button_updateState(BUTTON_CARD)!=BTTN_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		if(Button_getState(BUTTON_CARD, &u8_buttonVal) != BTTN_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
	}
	Lcd_printLCD((uint8_t*)"Button Pressed",(uint8_t*)"Receiving Data");
	Dio_writePin(DIO_SIG_CHANNEL_ID, PIN_LOW);
	EmptyString(au8_inputString);
	while (au8_inputString[0] != '#')
	{
		if(Spi_SlaveReceivePacket(au8_inputString, CARD_FRAME_LENGTH) != SPI_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		if((au8_inputString[0] == '*') && (au8_inputString[1] == '*'))
		{
			Dio_writePin(DIO_SIG_CHANNEL_ID, PIN_HIGH);
			Lcd_printLCD((uint8_t*)"Card is not",(uint8_t*)"Programmed");
			Delay_ms(1000);
			Lcd_printLCD((uint8_t*)"Please Contact",(uint8_t*)"Customer Service");
			Delay_ms(1000);
			gu8_USER_Mode_State = USER_IDLE;
			Lcd_printLCD((uint8_t*)"1.Insert Card", (uint8_t*)"2.Display Temp");
			return APP_STATUS_ERROR_OK;
		}
	}
	if(AppUSER_getCardData(&gstr_userCardData, au8_inputString) != APP_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
		
	enuApp_Status_t processState = AppUSER_startProcess(&gstr_userCardData);
	if((processState == APP_STATUS_PAN_NOT_FOUND) || (processState == APP_STATUS_PIN_NOT_CORRECT))
	{
		Delay_ms(1000);
		gu8_USER_Mode_State = USER_IDLE;
		Lcd_printLCD((uint8_t*)"1.Insert Card", (uint8_t*)"2.Display Temp");
	}else if (processState != APP_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	
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
	enuApp_Status_t PAN_status = AppUSER_checkPan();
	if(PAN_status == APP_STATUS_PAN_NOT_FOUND)
	{
		Lcd_printLCD((uint8_t*)"Your Card is not", (uint8_t*)"in the Database");
		Dio_writePin(DIO_SIG_CHANNEL_ID, PIN_HIGH);
		return APP_STATUS_PAN_NOT_FOUND;
	}
	
	if(Lcd_printString((uint8_t*)"Enter Your PIN") != LCD_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	enuApp_Status_t PIN_status = AppUSER_checkPin();
	if(PIN_status == APP_STATUS_PIN_NOT_CORRECT)
	{
		return APP_STATUS_PIN_NOT_CORRECT;
	}else if(PIN_status != APP_STATUS_PIN_CORRECT)
		return APP_STATUS_ERROR_NOK;
	
	
	Lcd_printLCD((uint8_t*)"    Welcome", (uint8_t*)"    Customer");
	Delay_ms(1000);
	
	Lcd_printLCD((uint8_t*)"  Your Balance", gstr_clientdata.au8_Balance);
	Delay_ms(1000);
	
	Lcd_clear();
	if(Lcd_setCursor(0, 0) != LCD_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	if(Lcd_printString(cgau8_AMOUNTString) != LCD_STATUS_ERROR_OK)
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
	float32_t f32_balance = stringToFloat(gstr_clientdata.au8_Balance);
	float32_t f32_maxAmount = stringToFloat(gau8_maxAmount);
	float32_t f32_amount = 0;
	while(1)
	{
		EmptyString(au8_inputKPD);
		KeypdaStatus = AppUSER_ReportKeypad(au8_inputKPD);
		
		if(KeypdaStatus == APP_STATUS_KPD_NUM)
		{
			if((stringLength(au8_inputKPD) != MAX_BAL_LENGTH+1) || (au8_inputKPD[4] != '.'))
			{
// 				Lcd_printLCD((uint8_t*)"Invalid Balance", (uint8_t*)"Format = xxxx.xx");
				Lcd_clear();
				Lcd_setCursor(0,0);
				Lcd_printString(cgau8_AMOUNTString);
//  			Delay_ms(1000);
//  			Lcd_clear();
//  			Lcd_setCursor(0,0);
//  			Lcd_printString((uint8_t*)"Enter Amount");
				continue;
			}
			f32_amount = stringToFloat(au8_inputKPD);
			
			if(f32_maxAmount < f32_amount)
			{
				Lcd_printLCD((uint8_t*)"   Max Amount", (uint8_t*)"    Exceeded");
				
				Delay_ms(500);
				
				Lcd_clear();
				if(Lcd_setCursor(0, 0) != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				if(Lcd_printString(cgau8_AMOUNTString) != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
			}else
			{
			Lcd_printLCD((uint8_t*)"  Please Wait", (uint8_t*)" Processing....");
				
			if(f32_balance < f32_amount)
			{
				Lcd_printLCD((uint8_t*)"  Insufficient", (uint8_t*)"      Fund");
					
				Delay_ms(500);
					
				Lcd_clear();
				if(Lcd_setCursor(0, 0) != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				if(Lcd_printString(cgau8_AMOUNTString) != LCD_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
				}else/****************** Transaction Successfull ******************/
				{
					Lcd_printLCD((uint8_t*)"   Successful", (uint8_t*)"  Transaction");
						
					Motor_run(MOTOR_CASH_ID, 100, MOTOR_DIR_CLK_WISE);
					Delay_ms(1000);
					Motor_stop(MOTOR_CASH_ID);
					gu8_USER_Mode_State = USER_IDLE;
						
					f32_balance -= f32_amount;
					EmptyString(gstr_clientdata.au8_Balance);
					floatToString(f32_balance, gstr_clientdata.au8_Balance);
						
					uint8_t u8_newCustomerBalAddr = ATM_DB_CUSTOMER_BAL_BASE_ADDR + gu8_clientIndex*ATM_DB_CUSTOMER_DATA_SIZE;
					if(Eeprom_24_writePacket(u8_newCustomerBalAddr, gstr_clientdata.au8_Balance, \
					stringLength(gstr_clientdata.au8_Balance)) != EEPROM_24_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
						
					Lcd_printLCD((uint8_t*)"  Your Balance", gstr_clientdata.au8_Balance);
					Delay_ms(1000);
					Dio_writePin(DIO_SIG_CHANNEL_ID, PIN_HIGH); //Card Out
					Lcd_printLCD((uint8_t*)"1.Insert Card", (uint8_t*)"2.Display Temp");
						
					return APP_STATUS_ERROR_OK;
				}
			}
		}
		Delay_ms(200);
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
	uint8_t u8_clientPanAddr = 0;
	uint8_t au8_clientPAN[10] = {0};
	uint8_t u8_panFound = 0;
	for (gu8_clientIndex=0; gu8_clientIndex<gu8_registeredAccNum; gu8_clientIndex++)
	{
		u8_clientPanAddr = ATM_DB_CUSTOMER_PAN_BASE_ADDR + gu8_clientIndex*ATM_DB_CUSTOMER_DATA_SIZE;
		if(Eeprom_24_readPacket(u8_clientPanAddr, au8_clientPAN, MAX_PAN_LENGTH+1) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		if(stringCompare(au8_clientPAN, gstr_userCardData.au8_primaryAccountNumber) == 1)
		{
			u8_panFound = 1;
			break;
		}
	}
	
	if(u8_panFound == 1)
	{
		uint8_t u8_clientBalAddr = ATM_DB_CUSTOMER_BAL_BASE_ADDR + gu8_clientIndex*ATM_DB_CUSTOMER_DATA_SIZE;
			
		stringCopy(au8_clientPAN, gstr_clientdata.au8_PAN);
			
		if(Eeprom_24_readPacket(u8_clientBalAddr, gstr_clientdata.au8_Balance, MAX_BAL_LENGTH+1) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
			
		return APP_STATUS_PAN_FOUND;
	}

	return APP_STATUS_PAN_NOT_FOUND;
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
	Kpd_enablePass = 1;
	uint8_t u8_loopCounter = 0;
	while(1)
	{
		EmptyString(au8_inputKPD);
		KeypdaStatus = AppUSER_ReportKeypad(au8_inputKPD);
			
		if(KeypdaStatus == APP_STATUS_KPD_NUM)
		{
			u8_loopCounter++;
			if(stringCompare(au8_inputKPD, gstr_userCardData.au8_pinNum) != 1)
			{
				if(u8_loopCounter == MAX_PIN_TRIAL)
				{
					Lcd_printLCD((uint8_t*)" Incorrect PIN", (uint8_t*)" ");
					Delay_ms(1000);
					Lcd_printLCD((uint8_t*)"Please Contact", (uint8_t*)"Customer Service");
					Delay_ms(1000);
					Dio_writePin(DIO_SIG_CHANNEL_ID, PIN_HIGH);
					return APP_STATUS_PIN_NOT_CORRECT;
				}
				Lcd_printLCD((uint8_t*)" Incorrect PIN", (uint8_t*)"   Try Again");
				Delay_ms(500);
					
				Lcd_clear();
				if(Lcd_setCursor(0, 0) != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				if(Lcd_printString((uint8_t*)"Enter Your PIN") != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
			}else
			{
				Lcd_printLCD((uint8_t*)"  Please Wait", (uint8_t*)" Processing....");
					Kpd_enablePass = 0;
				return APP_STATUS_PIN_CORRECT;
			}
		}
		Delay_ms(200);
	}
	Kpd_enablePass = 0;
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
		}else if((sau8_data[u8_index] != '+') || (sau8_data[u8_index] != 'x'))
		{
			if(u8_index!=10)
			{
				if(Lcd_setCursor(LCD_IN_POS_X, u8_index+LCD_IN_POS_Y) != LCD_STATUS_ERROR_OK)
					return APP_STATUS_ERROR_NOK;
				if(Kpd_enablePass == 0)
				{
					if(Lcd_printChar(sau8_data[u8_index]) != LCD_STATUS_ERROR_OK)
						return APP_STATUS_ERROR_NOK;
				}
				else
				{
					if(u8_index < 4)
					{
						if(Lcd_printChar('*') != LCD_STATUS_ERROR_OK)
							return APP_STATUS_ERROR_NOK;
					}else
					{
						u8_index--;
					}
				}
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

#endif /* APPUSER_H_ */