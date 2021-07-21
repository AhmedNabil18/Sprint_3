/*
 * App.h
 *
 * Created: 7/16/2021 1:22:44 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef APP_H_
#define APP_H_


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "../Srvc Layer/Service.h"

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- CONSTANTS -*-*-*-*-*-*/
#define LED_ALL_OFF						0U
#define LED_GREEN_ON					1U
#define LED_YELLOW_ON					2U
#define LED_RED_ON						3U


#define TRAFFIC_ACTION_IDLE				0U
#define TRAFFIC_ACTION_OK				1U
#define TRAFFIC_ACTION_GREEN			2U
#define TRAFFIC_ACTION_YELLOW			3U
#define TRAFFIC_ACTION_RED				4U
/*******************************************************************************
 *                          Module Data Types                                  *
 *******************************************************************************/
/*
 * Data Type for App return status
 */
typedef enum
{
	APP_STATUS_ERROR_NOK,
	APP_STATUS_ERROR_OK,
	APP_STATUS_ERROR_ID_INVALID,
	APP_STATUS_ERROR_NULL,
	APP_STATUS_INITIALIZED,
	APP_STATUS_UNINITIALIZED
}enuApp_Status_t;

/*
 * Data Type for Traffic state
 */
typedef enum
{
	TRAFFIC_OFF,
	TRAFFIC_AT,
	TRAFFIC_GO,
	TRAFFIC_WAIT,
	TRAFFIC_STOP
}enuTrafficState_t;


/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
/* Function to Start the application */
enuApp_Status_t TrafficApp_start(void);

/* Function to initialize the Application */
enuApp_Status_t TrafficApp_init(void);

/* Function to update the application */
enuApp_Status_t TrafficApp_update(void);



#endif /* APP_H_ */