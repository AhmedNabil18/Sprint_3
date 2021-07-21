/*
 * EchoApp.h
 *
 * Created: 7/21/2021 1:09:24 AM
 *  Author: Ahmed Nabil
 */ 


#ifndef ECHOAPP_H_
#define ECHOAPP_H_


#include "../MCAL/Uart Module/Uart.h"
#include "../StringManipulation.h"

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

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
/* Function to Start the application */
enuApp_Status_t EchoApp_start(void);

/* Function to initialize the Application */
enuApp_Status_t EchoApp_init(void);

/* Function to update the application */
enuApp_Status_t EchoApp_update(void);


#endif /* ECHOAPP_H_ */