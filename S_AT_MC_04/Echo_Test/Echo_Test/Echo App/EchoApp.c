/*
 * EchoApp.c
 *
 * Created: 7/21/2021 1:09:10 AM
 *  Author: Ahmed Nabil
 */ 


#include "EchoApp.h"

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- GLOBAL STATIC VARIABLES *-*-*-*-*-*/
static enuApp_Status_t enuCurrentAppStatus = APP_STATUS_UNINITIALIZED;
uint8_t gau8_data[50]={0};
uint8_t gu8_counter=0;
uint8_t gu8_flag=0;
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*- FUNCTIONS IMPLEMENTATION -*-*-*-*-*-*/

void Uart_RXC_ISR(void)
{
	gau8_data[gu8_counter] = Uart_DataRegister();
	Uart_sendByte(gau8_data[gu8_counter]);
	if (gau8_data[gu8_counter] == '\r')
	{
		gau8_data[gu8_counter] = '\0';
		gu8_counter = 0;
		gu8_flag = 1;
	}else if (gau8_data[gu8_counter] == '\b')
	{
		gu8_counter--;
	}
	else
	{
		gu8_counter++;
	}
}
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: EchoApp_start
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Start the Application.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t EchoApp_start(void)
{
/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Initialize the Car application */
	if(EchoApp_init() != APP_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;	
	/* Application Super Loop */
	while (1)
	{
		/* Update the car status */
		if(EchoApp_update() != APP_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
		
	}
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: EchoApp_init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Initialize the Application.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t EchoApp_init(void)
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

	/* Initialize the Uart Module */
	enuUart_Status_t Uart_State = Uart_init();
	if((UART_STATUS_ERROR_OK != Uart_State) && (UART_STATUS_ALREADY_INIT != Uart_State))
		return APP_STATUS_ERROR_NOK;
	
	if(Uart_EnableNotification_RXC(Uart_RXC_ISR) != UART_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	EnableGlbl_Interrupt();

	/* Update enuCurrentAppStatus to initialized */
	enuCurrentAppStatus = APP_STATUS_INITIALIZED;
	return APP_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: EchoApp_update
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Update the state of the application.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t EchoApp_update(void)
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
	if(gu8_flag == 1)
	{
		gu8_flag = 0;
		if(Uart_sendPacket(gau8_data, stringLength(gau8_data)) != UART_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		Uart_sendByte( '\r');
		EmptyString(gau8_data);
	}
	return APP_STATUS_ERROR_OK;
}