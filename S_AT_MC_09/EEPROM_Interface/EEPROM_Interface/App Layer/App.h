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
#include "../ECUAL/Eeprom_I2C Module/Eeprom_24.h"
#include "../ECUAL/Terminal Module/Terminal.h"

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- CONSTANTS -*-*-*-*-*-*/

#define APP_ACTION_TERMINAL			0U
#define APP_ACTION_WRITE			1U
#define APP_ACTION_READ				2U

#define APP_STATE_CMD				0U
#define APP_STATE_ADDRESS			1U
#define APP_STATE_DATA				2U
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

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
/* Function to Start the application */
enuApp_Status_t App_start(void);

/* Function to initialize the Application */
enuApp_Status_t App_init(void);

/* Function to update the application */
enuApp_Status_t App_update(void);



#endif /* APP_H_ */