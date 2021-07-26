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
uint8_t gau8_threshold[3] = {0};
uint32_t gu32_Tref = 0;
uint8_t gau8_temp[3] = {0};
uint8_t gu8_temp = 0;
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
	DIO_PORTB_DIR |= 1<<3;
	/* Call the initializer of the Terminal */
	if(SRVC_STATUS_ERROR_OK != Service_init())
	{
		return APP_STATUS_ERROR_NOK;
	}
	if(Lcd_setCursor(0,0) != LCD_STATUS_ERROR_OK)
		return SRVC_STATUS_ERROR_NOK;
	if(Lcd_printString((uint8_t*)"Temp:   Fan: 0%") != LCD_STATUS_ERROR_OK)
		return SRVC_STATUS_ERROR_NOK;
	if(Lcd_setCursor(1,0) != LCD_STATUS_ERROR_OK)
		return SRVC_STATUS_ERROR_NOK;
	if(Lcd_printString((uint8_t*)"Tref:     Set:  ") != LCD_STATUS_ERROR_OK)
		return SRVC_STATUS_ERROR_NOK;
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
	enuSrvc_Status_t KeypdaStatus = Service_ReportKeypad(gau8_threshold);
	static uint8_t sau8_fanSpeed[5] = "0  %";
	static uint8_t su8_fanSpeed = 0;
	if(KeypdaStatus == SRVC_KPD_NUM)
	{
		stringToInteger((uint32_t*)&gu32_Tref, gau8_threshold);
		if(Lcd_setCursor(LCD_TREF_POS_X, LCD_TREF_POS_Y) != LCD_STATUS_ERROR_OK)
			return SRVC_STATUS_ERROR_NOK;
		if(Lcd_printString(gau8_threshold) != LCD_STATUS_ERROR_OK)
			return SRVC_STATUS_ERROR_NOK;
	}
	if (LM35_readTemp(&gu8_temp) == LM35_STATUS_ERROR_OK)
	{
		integerToString((uint16_t)gu8_temp, gau8_temp, DEC);
		if(Lcd_setCursor(LCD_TEMP_POS_X, LCD_TEMP_POS_Y) != LCD_STATUS_ERROR_OK)
		return SRVC_STATUS_ERROR_NOK;
		if(Lcd_printString(gau8_temp) != LCD_STATUS_ERROR_OK)
		return SRVC_STATUS_ERROR_NOK;
	}
	
	su8_fanSpeed = (100*(gu8_temp-gu32_Tref))/(MAX_TEMP-gu32_Tref);
	if(su8_fanSpeed>100) su8_fanSpeed=0;
	
	if(Lcd_setCursor(LCD_FAN_POS_X, LCD_FAN_POS_Y) != LCD_STATUS_ERROR_OK)
	return SRVC_STATUS_ERROR_NOK;
	if(Lcd_printString((uint8_t*)"   %") != LCD_STATUS_ERROR_OK)
	return SRVC_STATUS_ERROR_NOK;
	
	if(Lcd_setCursor(LCD_FAN_POS_X, LCD_FAN_POS_Y) != LCD_STATUS_ERROR_OK)
	return SRVC_STATUS_ERROR_NOK;
	if(Lcd_printDecimal(su8_fanSpeed) != LCD_STATUS_ERROR_OK)
	return SRVC_STATUS_ERROR_NOK;
	EmptyString(sau8_fanSpeed);
	
	Motor_stop(MOTOR_FAN_ID);
	Motor_run(MOTOR_FAN_ID,su8_fanSpeed,MOTOR_DIR_CLK_WISE);

	
	Delay_ms(155);
	return APP_STATUS_ERROR_OK;
}