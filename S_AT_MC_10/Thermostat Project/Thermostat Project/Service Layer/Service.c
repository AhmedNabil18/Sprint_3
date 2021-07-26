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
			if(Lcd_setCursor(1,14) != LCD_STATUS_ERROR_OK)
				return SRVC_STATUS_ERROR_NOK;
			if(Lcd_printChar(' ') != LCD_STATUS_ERROR_OK)
				return SRVC_STATUS_ERROR_NOK;
			if(Lcd_setCursor(1,15) != LCD_STATUS_ERROR_OK)
				return SRVC_STATUS_ERROR_NOK;
			if(Lcd_printChar(' ') != LCD_STATUS_ERROR_OK)
				return SRVC_STATUS_ERROR_NOK;
			if(Lcd_setCursor(1,14) != LCD_STATUS_ERROR_OK)
				return SRVC_STATUS_ERROR_NOK;
			stringCopy(sau8_data, pu8_key);
			return SRVC_KPD_NUM;
		}else if(sau8_data[u8_index] == '*')
		{
			u8_index--;
			sau8_data[u8_index] = '\0';
			if(Lcd_setCursor(1,u8_index+14) != LCD_STATUS_ERROR_OK)
			return SRVC_STATUS_ERROR_NOK;
			if(Lcd_printChar(' ') != LCD_STATUS_ERROR_OK)
			return SRVC_KPD_NUM;
		}else
		{
			if(u8_index!=2)
			{
				if(Lcd_setCursor(1,u8_index+14) != LCD_STATUS_ERROR_OK)
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

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Service_UpdateMotors
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_motorAction - The action to be done on the motor 
*					u8_motorSpeed - The speed of the motor (Also holds the direction)
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSrvc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Update the state of the motor (Run the motor with the specific action
*				,direction and speed)
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuSrvc_Status_t Service_UpdateMotors(uint8_t u8_motorAction, uint8_t u8_motorSpeed)
{
// 	if(u8_motorAction == ACTION_STOP)
// 	{
// 		Motor_stop(MOTOR_LEFT);
// 		Motor_stop(MOTOR_RIGHT);
// 		return SRVC_STATUS_ERROR_OK;
// 	}
// 	uint8_t u8_speed = 0;
// 	uint8_t u8_leftMotorDir=MOTOR_FRWRD;
// 	uint8_t u8_rightMotorDir=MOTOR_FRWRD;
// 	
// 	switch (u8_motorSpeed)
// 	{
// 	case SPEED_30F:
// 		u8_speed = 30;
// 		break;
// 	case SPEED_60F:
// 		u8_speed = 60;
// 		break;
// 	case SPEED_90F:
// 		u8_speed = 90;
// 		break;
// 	case SPEED_30B:
// 		u8_speed = 30;
// 		u8_leftMotorDir = MOTOR_BKWRD;
// 		u8_rightMotorDir = MOTOR_BKWRD;
// 		break;
// 	}
// 	
// 	if (u8_motorAction == ACTION_LEFT)
// 	{
// 		u8_speed = 30;
// 		u8_leftMotorDir = MOTOR_FRWRD;
// 		u8_rightMotorDir = MOTOR_BKWRD;
// 	}else if (u8_motorAction == ACTION_RIGHT)
// 	{
// 		u8_speed = 30;
// 		u8_leftMotorDir = MOTOR_BKWRD;
// 		u8_rightMotorDir = MOTOR_FRWRD;
// 	}
// 	if( MOTOR_STATUS_ERROR_OK != Motor_run(MOTOR_LEFT, u8_speed, u8_leftMotorDir))
// 		return SRVC_STATUS_ERROR_NOK;
// 	if( MOTOR_STATUS_ERROR_OK != Motor_run(MOTOR_RIGHT, u8_speed, u8_rightMotorDir))
// 		return SRVC_STATUS_ERROR_NOK;
 	return  SRVC_STATUS_ERROR_OK;
}