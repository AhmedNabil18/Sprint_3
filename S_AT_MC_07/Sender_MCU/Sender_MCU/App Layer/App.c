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
enuTrafficState_t genu_AppState = TRAFFIC_ACTION_IDLE;
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*- FUNCTIONS IMPLEMENTATION -*-*-*-*-*-*/

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: TrafficApp_start
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Start the Application.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t TrafficApp_start(void)
{
	/**************************************************************************************/
	/*								Function Implementation								  */
	/**************************************************************************************/
	/* Initialize the Traffic application */
	if(TrafficApp_init() != APP_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	EnableGlbl_Interrupt();
	/* Application Super Loop */
	while (1)
	{
		/* Update the Traffic status */
		if(TrafficApp_update() != APP_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
			
	}
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: TrafficApp_init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Initialize the Application.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t TrafficApp_init(void)
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

	/* Call the initializer in service layer */
	if(SRVC_STATUS_ERROR_OK != Service_init())
	{
		return APP_STATUS_ERROR_NOK;
	}
	/* Update enuCurrentAppStatus to initialized */
	enuCurrentAppStatus = APP_STATUS_INITIALIZED;
	return APP_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: TrafficApp_update
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuApp_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Update the state of the application.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuApp_Status_t TrafficApp_update(void)
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
	/* Get the input from the service layer */
	enuTrafficState_t enu_trafficState = TRAFFIC_OFF;
	if(Service_ReportTerminal( (uint8_t*) &enu_trafficState) != SRVC_STATUS_ERROR_OK)
		return APP_STATUS_ERROR_NOK;
	if( (genu_AppState != enu_trafficState) || (TRAFFIC_AT == enu_trafficState))
	{
		genu_AppState = enu_trafficState;
		/* Check the input of the Terminal */
		uint8_t u8_Action = TRAFFIC_ACTION_IDLE;
		switch (enu_trafficState)
		{	/* Decide the Action of the application */
			case TRAFFIC_OFF:
				u8_Action = TRAFFIC_ACTION_IDLE;
				break;
			case TRAFFIC_AT:
				u8_Action = TRAFFIC_ACTION_OK;
				break;
			case TRAFFIC_GO:
				u8_Action = TRAFFIC_ACTION_GREEN;
				break;
			case TRAFFIC_WAIT:
				u8_Action = TRAFFIC_ACTION_YELLOW;
				break;
			case TRAFFIC_STOP:
				u8_Action = TRAFFIC_ACTION_RED;
				break;
			default:
				return APP_STATUS_ERROR_NOK;
		}
		
		/* Send Action to the Service Layer */
		if(Service_UpdateLeds(u8_Action) != SRVC_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;
		if(Service_UpdateTerminal(u8_Action) != SRVC_STATUS_ERROR_OK)
			return APP_STATUS_ERROR_NOK;	
	}
	return APP_STATUS_ERROR_OK;
}