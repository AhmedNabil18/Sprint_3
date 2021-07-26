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
		
	if(LCD_STATUS_ERROR_OK != Lcd_init())
		return SRVC_STATUS_ERROR_NOK;	
	if(KEYPAD_STATUS_ERROR_OK != Keypad_init())
		return SRVC_STATUS_ERROR_NOK;
	if(LM35_STATUS_ERROR_OK != LM35_init())
		return SRVC_STATUS_ERROR_NOK;
	if(MOTOR_STATUS_ERROR_OK != Motor_init())
		return SRVC_STATUS_ERROR_NOK;
		
	genu_SrvcStatus = SRVC_STATUS_INITIALIZED;
	return SRVC_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Service_ReportKeypad
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_key - Pointer to variable to hold the button pressed
* Return value: enuSrvc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to report the value pressed in the keypad.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuSrvc_Status_t Service_ReportKeypad(uint8_t* pu8_key)
{
	static uint8_t u8_index = 0;
	static uint8_t sau8_data[3]={0};
	enuKeypad_Status_t enuKeypadStatus = Keypad_readKey(&sau8_data[u8_index]);
	
	if(enuKeypadStatus == KEYPAD_STATUS_PRESSED)
	{
		if(sau8_data[u8_index] == '#')
		{
			sau8_data[u8_index] = '\0';
			u8_index = 0;
			if(Lcd_setCursor(LCD_SET_POS_X, LCD_SET_POS_Y) != LCD_STATUS_ERROR_OK)
				return SRVC_STATUS_ERROR_NOK;
			if(Lcd_printString((uint8_t*)"  ") != LCD_STATUS_ERROR_OK)
				return SRVC_STATUS_ERROR_NOK;
			if(Lcd_setCursor(LCD_SET_POS_X, LCD_SET_POS_Y) != LCD_STATUS_ERROR_OK)
				return SRVC_STATUS_ERROR_NOK;
			stringCopy(sau8_data, pu8_key);
			return SRVC_KPD_NUM;
		}else if(sau8_data[u8_index] == '*')
		{
			u8_index--;
			sau8_data[u8_index] = '\0';
			if(Lcd_setCursor(LCD_SET_POS_X, u8_index+LCD_SET_POS_Y) != LCD_STATUS_ERROR_OK)
			return SRVC_STATUS_ERROR_NOK;
			if(Lcd_printChar(' ') != LCD_STATUS_ERROR_OK)
			return SRVC_KPD_NUM;
		}else
		{
			if(u8_index!=2)
			{
				if(Lcd_setCursor(LCD_SET_POS_X, u8_index+LCD_SET_POS_Y) != LCD_STATUS_ERROR_OK)
					return SRVC_STATUS_ERROR_NOK;
				if(Lcd_printChar(sau8_data[u8_index]) != LCD_STATUS_ERROR_OK)
					return SRVC_STATUS_ERROR_NOK;
				u8_index++;
			}else
			{
				sau8_data[u8_index] = '\0';
			}
			
		}
	}else if(enuKeypadStatus == KEYPAD_STATUS_NOT_PRESSED)
	{
		return SRVC_STATUS_ERROR_OK;
	}
	
	return SRVC_STATUS_ERROR_OK;
}
