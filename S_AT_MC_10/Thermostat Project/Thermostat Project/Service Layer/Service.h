/*
 * Service.h
 *
 * Created: 7/16/2021 2:09:25 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef SERVICE_H_
#define SERVICE_H_

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "../ECUAL/Motor Module/Motor.h"
#include "../ECUAL/Keypad Module/Keypad.h"
#include "../ECUAL/Lcd Module/Lcd.h"
#include "../ECUAL/LM35 Module/LM35.h"
#include "../MCAL/Delay Module/Delay.h"
#include "../MCAL/StringManipulation.h"
#include "Service_Cfg.h"

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- CONSTANTS -*-*-*-*-*-*/
#define LCD_TEMP_POS_X			0
#define LCD_TEMP_POS_Y			5

#define LCD_FAN_POS_X			0
#define LCD_FAN_POS_Y			12

#define LCD_TREF_POS_X			1
#define LCD_TREF_POS_Y			5

#define LCD_SET_POS_X			1
#define LCD_SET_POS_Y			14
/*******************************************************************************
 *                          Module Data Types                                  *
 *******************************************************************************/
/*
 * Data Type for App return status
 */
typedef enum
{
	SRVC_STATUS_ERROR_NOK,
	SRVC_STATUS_ERROR_OK,
	SRVC_STATUS_ERROR_ID_INVALID,
	SRVC_STATUS_ERROR_NULL,
	SRVC_STATUS_INITIALIZED,
	SRVC_STATUS_UNINITIALIZED,
	SRVC_KPD_NUM
}enuSrvc_Status_t;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
/* Function to initialize the Service Layer */
enuSrvc_Status_t Service_init(void);

/* Function to report the value pressed in the keypad */
enuSrvc_Status_t Service_ReportKeypad(uint8_t* pu8_key);

#endif /* SERVICE_H_ */