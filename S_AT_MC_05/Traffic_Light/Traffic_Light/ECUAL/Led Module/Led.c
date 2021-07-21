/*
 * Led.c
 *
 * Created: 7/20/2021 7:00:59 PM
 *  Author: Ahmed Nabil
 */ 


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Led.h"

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- GLOBAL STATIC VARIABLES *-*-*-*-*-*/
static enuLed_Status_t genu_LedModuleState = LED_STATUS_NOT_INIT;
extern uint8_t gau8_ledsUsed[LEDS_USED_NUM];
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*- FUNCTIONS IMPLEMENTATION -*-*-*-*-*-*/

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Led_init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuLed_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Initialize the Led module.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuLed_Status_t Led_init(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the led module is already initialized */
	if (genu_LedModuleState == LED_STATUS_INIT)
	{
		return LED_STATUS_INIT;
	}else{/*Nothing to here*/}
		
	/* Check if there are no DIO Pins configured */
	if(DIO_USED_PINS_NUM == 0)
	{
		return LED_ERROR_DIO_INVALID;
	}
		
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Initialize the DIO Module and check if any error returned */
	enuDio_Status_t Dio_State = Dio_init(strDio_pins);
	if((DIO_STATUS_ERROR_OK != Dio_State) && (DIO_STATUS_ALREADY_INIT != Dio_State))
		return LED_STATUS_ERROR_NOK;
	/* Change the state of the Led module to Initialized */
	genu_LedModuleState = LED_STATUS_INIT;
	return LED_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Led_On
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_LedID - Index of the led
* Parameters (inout): None
* Parameters (out): None
* Return value: enuLed_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to turn on the Led with the given ID.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuLed_Status_t Led_On(uint8_t u8_LedID)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the leds index is invalid */
	if (LEDS_USED_NUM < u8_LedID)
	{
		return LED_STATUS_ERROR_ID_INVALID;
	}else{/*Nothing to here*/}
				
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Write Logic 1 on the Led's Pin and check if any error returned */
	enuDio_Status_t Dio_State = Dio_writePin(gau8_ledsUsed[u8_LedID], PIN_HIGH);
	if(DIO_STATUS_ERROR_OK != Dio_State)
		return LED_STATUS_ERROR_NOK;
	return LED_STATUS_ERROR_OK;
}
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Led_Off
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_LedID - Index of the led
* Parameters (inout): None
* Parameters (out): None
* Return value: enuLed_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to turn off the Led with the given ID.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuLed_Status_t Led_Off(uint8_t u8_LedID)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the leds index is invalid */
	if (LEDS_USED_NUM < u8_LedID)
	{
		return LED_STATUS_ERROR_ID_INVALID;
	}else{/*Nothing to here*/}
		
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Write Logic 0 on the Led's Pin and check if any error returned */
	enuDio_Status_t Dio_State = Dio_writePin(gau8_ledsUsed[u8_LedID], PIN_LOW);
	if(DIO_STATUS_ERROR_OK != Dio_State)
		return LED_STATUS_ERROR_NOK;
	return LED_STATUS_ERROR_OK;
}
	
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Led_Refresh
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_LedID - Index of the led
* Parameters (inout): None
* Parameters (out): None
* Return value: enuLed_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to refresh the output of the Led with the given ID.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuLed_Status_t Led_Refresh(uint8_t u8_LedID)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the leds index is invalid */
	if (LEDS_USED_NUM < u8_LedID)
	{
		return LED_STATUS_ERROR_ID_INVALID;
	}else{/*Nothing to here*/}
		
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Read the state of the LED */
	u8_LedState_t u8_LedState = PIN_LOW;
	enuDio_Status_t Dio_State = Dio_readPin(gau8_ledsUsed[u8_LedID], &u8_LedState);
	if(DIO_STATUS_ERROR_OK != Dio_State)
		return LED_STATUS_ERROR_NOK;
	/* Refresh the state of the LED */
	Dio_State = Dio_writePin(gau8_ledsUsed[u8_LedID], u8_LedState);
	if(DIO_STATUS_ERROR_OK != Dio_State)
		return LED_STATUS_ERROR_NOK;
	return LED_STATUS_ERROR_OK;
}
