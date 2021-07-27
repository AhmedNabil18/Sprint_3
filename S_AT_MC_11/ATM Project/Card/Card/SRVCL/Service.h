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
#include "../ECUAL/Eeprom_I2C Module/Eeprom_24.h"
#include "../ECUAL/Terminal Module/Terminal.h"
#include "../MCAL/Spi Module/Spi.h"
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

#define CARD_NAME				0U
#define CARD_PAN				1U
#define CARD_PIN				2U
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
	SRVC_STATUS_UNINITIALIZED
}enuSrvc_Status_t;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
/* Function to initialize the Service Layer */
enuSrvc_Status_t Service_init(void);

/* Function to report the value pressed in the keypad */
enuSrvc_Status_t Service_ReportTerminal(uint8_t* pu8_data);

#endif /* SERVICE_H_ */