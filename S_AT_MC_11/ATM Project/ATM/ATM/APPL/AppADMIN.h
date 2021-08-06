/*
 * AppADMIN.h
 *
 * Created: 7/30/2021 12:55:15 AM
 *  Author: Ahmed Nabil
 */ 


#ifndef APPADMIN_H_
#define APPADMIN_H_

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppADMIN_searchPan
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: sint16_t - return the index of the Customer in DB
* Description: Function to search for a customer's PAN if existed in DB.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
sint16_t AppADMIN_searchPan(uint8_t* pu8_data)
{
	uint8_t u8_clientPanAddr = 0;
	uint8_t au8_clientPAN[10] = {0};
	for (gu8_clientIndex=0; gu8_clientIndex<gu8_registeredAccNum; gu8_clientIndex++)
	{
		u8_clientPanAddr = ATM_DB_CUSTOMER_PAN_BASE_ADDR + gu8_clientIndex*ATM_DB_CUSTOMER_DATA_SIZE;
		if(Eeprom_24_readPacket(u8_clientPanAddr, au8_clientPAN, MAX_PAN_LENGTH+1) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
		if(stringCompare(au8_clientPAN, pu8_data) == 1)
		{
			return gu8_clientIndex;
		}
	}
	return -1;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppADMIN_processNewCustomer
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSrvc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to process adding a new customer in the data base.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppADMIN_processNewCustomer(void)
{
	uint8_t au8_tempPAN[10]={0};
	uint8_t au8_tempBalance[8]={0};
	uint8_t au8_input[2]={0};
	uint8_t u8_exist = 0;
	uint8_t u8_newCustomerPanAddr = 0;
	uint8_t u8_newCustomerBalAddr = 0;
	EmptyString(gstr_clientdata.au8_PAN);
	EmptyString(gstr_clientdata.au8_Balance);
	/* Get the Customer's Primary Account Number from the ADMIN Terminal */
	if(AppADMIN_getCustomerPAN(au8_tempPAN) != APP_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	
	sint16_t s16_clientIndex = AppADMIN_searchPan(au8_tempPAN);
	
	if(s16_clientIndex != -1)
	{
		Terminal_Out((uint8_t*)"This PAN is already existed in the Data Base\r\nDo you wish to update it?(y/n): ");
		AppADMIN_getInput(au8_input);
		if((au8_input[0] == 'y') || (au8_input[0] == 'Y'))
		{
			u8_exist = 1;
		}else if((au8_input[0] == 'n') || (au8_input[0] == 'N'))
		{
			return APP_STATUS_ERROR_OK; 
		}
	}
	stringCopy(au8_tempPAN, gstr_clientdata.au8_PAN);
	
	/* Get the Customer's Balance from the ADMIN Terminal */
	if(AppADMIN_getCustomerBalance(au8_tempBalance) != APP_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	stringCopy(au8_tempBalance, gstr_clientdata.au8_Balance);
	
	if(u8_exist == 0)
	{
		u8_newCustomerPanAddr = ATM_DB_CUSTOMER_PAN_BASE_ADDR + gu8_registeredAccNum*ATM_DB_CUSTOMER_DATA_SIZE ;
		u8_newCustomerBalAddr = ATM_DB_CUSTOMER_BAL_BASE_ADDR + gu8_registeredAccNum*ATM_DB_CUSTOMER_DATA_SIZE ;
		
		if(Eeprom_24_writePacket(u8_newCustomerPanAddr, gstr_clientdata.au8_PAN, stringLength(gstr_clientdata.au8_PAN)) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
		
		if(Eeprom_24_writePacket(u8_newCustomerBalAddr, gstr_clientdata.au8_Balance, stringLength(gstr_clientdata.au8_Balance)) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
		
		if(Eeprom_24_writeByte(ATM_DB_ACC_NUM_ADDR, ++gu8_registeredAccNum) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	}else
	{
		u8_newCustomerPanAddr = ATM_DB_CUSTOMER_PAN_BASE_ADDR + s16_clientIndex*ATM_DB_CUSTOMER_DATA_SIZE ;
		u8_newCustomerBalAddr = ATM_DB_CUSTOMER_BAL_BASE_ADDR + s16_clientIndex*ATM_DB_CUSTOMER_DATA_SIZE ;
		
		if(Eeprom_24_writePacket(u8_newCustomerPanAddr, gstr_clientdata.au8_PAN, stringLength(gstr_clientdata.au8_PAN)) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		
		if(Eeprom_24_writePacket(u8_newCustomerBalAddr, gstr_clientdata.au8_Balance, stringLength(gstr_clientdata.au8_Balance)) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
	}
	if(gu8_registeredAccNum == 1)
	{
		gu8_initData = ATM_DB_FLAG_SET_VAL;
		if(Eeprom_24_writeByte(ATM_DB_FLAG_ADDR, ATM_DB_FLAG_SET_VAL) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
	}
	return APP_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: AppADMIN_processExistingCustomer
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSrvc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to process adding a new customer in the data base.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t AppADMIN_processExistingCustomer(void)
{
	uint8_t au8_tempPAN[10]={0};
	uint8_t au8_tempBalance[8]={0};
	uint8_t au8_input[2]={0};
	uint8_t u8_exist = 1;
	uint8_t u8_newCustomerPanAddr = 0;
	uint8_t u8_newCustomerBalAddr = 0;
	EmptyString(gstr_clientdata.au8_PAN);
	EmptyString(gstr_clientdata.au8_Balance);
	/* Get the Customer's Primary Account Number from the ADMIN Terminal */
	if(AppADMIN_getCustomerPAN(au8_tempPAN) != APP_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	
	sint16_t s16_clientIndex = AppADMIN_searchPan(au8_tempPAN);
	
	if(s16_clientIndex == -1)
	{
		Terminal_Out((uint8_t*)"This PAN is not existed in the Data Base\r\nDo you wish to add new?(y/n): ");
		AppADMIN_getInput(au8_input);
		if((au8_input[0] == 'y') || (au8_input[0] == 'Y'))
		{
			u8_exist = 0;
		}else if((au8_input[0] == 'n') || (au8_input[0] == 'N'))
		{
			return APP_STATUS_ERROR_OK;
		}
	}
	stringCopy(au8_tempPAN, gstr_clientdata.au8_PAN);
	
	/* Get the Customer's Balance from the ADMIN Terminal */
	if(AppADMIN_getCustomerBalance(au8_tempBalance) != APP_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	stringCopy(au8_tempBalance, gstr_clientdata.au8_Balance);
	
	if(u8_exist == 0)
	{
		u8_newCustomerPanAddr = ATM_DB_CUSTOMER_PAN_BASE_ADDR + gu8_registeredAccNum*ATM_DB_CUSTOMER_DATA_SIZE ;
		u8_newCustomerBalAddr = ATM_DB_CUSTOMER_BAL_BASE_ADDR + gu8_registeredAccNum*ATM_DB_CUSTOMER_DATA_SIZE ;
		
		if(Eeprom_24_writePacket(u8_newCustomerPanAddr, gstr_clientdata.au8_PAN, stringLength(gstr_clientdata.au8_PAN)) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		
		if(Eeprom_24_writePacket(u8_newCustomerBalAddr, gstr_clientdata.au8_Balance, stringLength(gstr_clientdata.au8_Balance)) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		
		if(Eeprom_24_writeByte(ATM_DB_ACC_NUM_ADDR, ++gu8_registeredAccNum) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
			
		if(gu8_registeredAccNum == 1)
		{
			gu8_initData = ATM_DB_FLAG_SET_VAL;
			if(Eeprom_24_writeByte(ATM_DB_FLAG_ADDR, ATM_DB_FLAG_SET_VAL) != EEPROM_24_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		}
	}else
	{
		u8_newCustomerPanAddr = ATM_DB_CUSTOMER_PAN_BASE_ADDR + s16_clientIndex*ATM_DB_CUSTOMER_DATA_SIZE ;
		u8_newCustomerBalAddr = ATM_DB_CUSTOMER_BAL_BASE_ADDR + s16_clientIndex*ATM_DB_CUSTOMER_DATA_SIZE ;
		
		if(Eeprom_24_writePacket(u8_newCustomerPanAddr, gstr_clientdata.au8_PAN, stringLength(gstr_clientdata.au8_PAN)) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
		
		if(Eeprom_24_writePacket(u8_newCustomerBalAddr, gstr_clientdata.au8_Balance, stringLength(gstr_clientdata.au8_Balance)) != EEPROM_24_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	}
	return APP_STATUS_ERROR_OK;
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
		if(Terminal_Out((uint8_t*)"\nYour Input: ") != TERMINAL_STATUS_ERROR_OK)
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
		if(Terminal_Out((uint8_t*)"\nInvalid Choice, Only Choose from 1 to 3\r") != TERMINAL_STATUS_ERROR_OK)
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
		if(Terminal_Out((uint8_t*)"\nMax Amount: ") != TERMINAL_STATUS_ERROR_OK)
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
		uint8_t u8_index=0;
		for(u8_index=0; u8_index<MAX_BAL_LENGTH; u8_index++)
		{
			if(((pu8_data[u8_index]>'9') || (pu8_data[u8_index]<'0')) && (pu8_data[u8_index]!='.'))
			{
				break;
			}
		}
		if ((u8_index == MAX_BAL_LENGTH) && (pu8_data[u8_index] == '\0'))
		{
			break;
		}
		if(Terminal_Out((uint8_t*)"\nInvalid Balance, Balance should be in (xxxx.xx) format\r\n") != TERMINAL_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
		EmptyString(pu8_data);
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
		if(Terminal_Out((uint8_t*)"\nPAN: ") != TERMINAL_STATUS_ERROR_OK)
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
			if(Terminal_Out((uint8_t*)"\nInvalid PAN, PAN should be 9 numeric characters\r") != TERMINAL_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
			EmptyString(pu8_data);
			continue;
		}
		uint8_t u8_index=0;
		
		for(u8_index=0; u8_index<MAX_PAN_LENGTH; u8_index++)
		{
			if((pu8_data[u8_index]>'9') || (pu8_data[u8_index]<'0'))
			{
				if(Terminal_Out((uint8_t*)"\nInvalid PAN, PAN should be 9 numeric characters\r") != TERMINAL_STATUS_ERROR_OK)
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
		if(Terminal_Out((uint8_t*)"\nBalance: ") != TERMINAL_STATUS_ERROR_OK)
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
		
		uint8_t u8_index=0;
		for(u8_index=0; u8_index<MAX_BAL_LENGTH; u8_index++)
		{
			if(((pu8_data[u8_index]>'9') || (pu8_data[u8_index]<'0')) && (pu8_data[u8_index]!='.'))
			{
				break;
			}
		}
		if ((u8_index == MAX_BAL_LENGTH) && (pu8_data[u8_index] == '\0'))
		{
			break;
		}
		if(Terminal_Out((uint8_t*)"\nInvalid Balance, Balance should be in (xxxx.xx) format\r\n") != TERMINAL_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
		EmptyString(pu8_data);
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
	uint8_t u8_counter=0;
	Terminal_enablePasswordMode();
	do
	{
		if(Terminal_Out((uint8_t*)"\nATM PIN: ") != TERMINAL_STATUS_ERROR_OK)
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
		
		if(stringCompare((uint8_t*)ATM_DB_ATM_PIN_VAL ,pu8_data) != 1)
		{
			if(u8_counter == MAX_PIN_TRIAL)
			{
				gu8_ATMMode = ATM_MODE_USER;
				Lcd_printLCD((uint8_t*)"1.Insert Card", (uint8_t*)"2.Display Temp");
				Terminal_Out((uint8_t*)"Incorrect PIN\r\nPlease reach the Company\r\n");
				return APP_STATUS_PIN_NOT_CORRECT;
			}
			if(Terminal_Out((uint8_t*)"\nIncorrect PIN\r") != TERMINAL_STATUS_ERROR_OK)
				return APP_STATUS_ERROR_NOK;
			u8_counter++;
			EmptyString(pu8_data);
			continue;
		}else
		{
			if(Terminal_Out((uint8_t*)"\nLoading...\r") != TERMINAL_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
			break;
		}
		EmptyString(pu8_data);
		if(Terminal_Out((uint8_t*)"\nInvalid PIN, Only 4 characters\r") != TERMINAL_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
	} while (1);
	Terminal_disablePasswordMode();
	return APP_STATUS_PIN_CORRECT;
}



#endif /* APPADMIN_H_ */