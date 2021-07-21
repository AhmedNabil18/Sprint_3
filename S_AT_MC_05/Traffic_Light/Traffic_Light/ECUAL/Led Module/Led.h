/*
 * Led.h
 *
 * Created: 7/20/2021 7:01:23 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef LED_H_
#define LED_H_


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Led_Cfg.h"
#include "../../MCAL/Dio Module/Dio.h"
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- CONSTANTS -*-*-*-*-*-*/

/*******************************************************************************
 *                          Module Data Types                                  *
 *******************************************************************************/
/*
 * Data Type for LED return status
 */
typedef enum
{
	LED_STATUS_ERROR_NOK,
	LED_STATUS_ERROR_OK,
	LED_STATUS_ERROR_ID_INVALID,
	LED_STATUS_ERROR_NULL,
	LED_STATUS_NOT_INIT,
	LED_STATUS_INIT,
	LED_ERROR_DIO_INVALID
}enuLed_Status_t;

/*
 * Data Type for Led state
 */
typedef uint8_t u8_LedState_t;
/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
/* Function to initialize the Led module */
enuLed_Status_t Led_init(void);

/* Function to turn on the Led */
enuLed_Status_t Led_On(uint8_t u8_LedID);

/* Function to turn off the Led */
enuLed_Status_t Led_Off(uint8_t u8_LedID);

/* Function to refresh the output of the LED */
enuLed_Status_t Led_Refresh(uint8_t u8_LedID);




#endif /* LED_H_ */