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
#include "../MCAL/Spi Module/Spi.h"
#include "../MCAL/Delay Module/Delay.h"
#include "../MCAL/StringManipulation.h"
#include "../MCAL/Ext Interrupt Module/Ext_INT.h"

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- CONSTANTS -*-*-*-*-*-*/
#define INTERRUPT_REQUEST		0U
#define POLLING_REQUEST			1U
#define HARDWARE				0U
#define SIMULATION				1U


#define PROJECT						SIMULATION
#define REQUEST_MODE				INTERRUPT_REQUEST

#define CARD_MODE_ADMIN				0U
#define CARD_MODE_USER				1U

#define CARD_INIT_ADDRESS			0x00U
#define CARD_INITIALIZED			0xAAU

#if PROJECT == HARDWARE
#define CARD_NAME_PAGE_ADDR			0x01U
#define CARD_PAN_PAGE_ADDR			0x0BU
#define CARD_PIN_PAGE_ADDR			0x15U
#elif PROJECT == SIMULATION
#define CARD_NAME_PAGE_ADDR			0x20U
#define CARD_PAN_PAGE_ADDR			0x30U
#define CARD_PIN_PAGE_ADDR			0x40U
#endif
#define CARD_NAME				0U
#define CARD_PAN				1U
#define CARD_PIN				2U

#define MAX_NAME_LENGTH			9U
#define MAX_PAN_LENGTH			9U
#define MAX_PIN_LENGTH			4U

#define USER_IDLE				0U
#define USER_BUSY				1U

#define ADMIN_NOT_REQUESTED		0U
#define ADMIN_REQUESTED			1U

#define ATM_NOT_REQUESTED		0U
#define ATM_REQUESTED			1U
#define ATM_SENDING				2U
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- Data Types -*-*-*-*-*-*/
typedef struct
{
	uint8_t au8_cardHolderName[10];
	uint8_t au8_primaryAccountNumber[10];
	uint8_t au8_pinNum[5];
}strCardData_t;

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
	APP_STATUS_UNINITIALIZED,
	APP_STATUS_NO_OP
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

enuApp_Status_t App_ReportTerminal(uint8_t* pu8_data);

enuApp_Status_t AppADMIN_getCardName(uint8_t* pu8_data);

enuApp_Status_t AppADMIN_getCardPAN(uint8_t* pu8_data);

enuApp_Status_t AppADMIN_getCardPIN(uint8_t* pu8_data);

enuApp_Status_t AppADMIN_saveCardData(strCardData_t* pstr_CardData);

enuApp_Status_t AppUSER_sendCardData(strCardData_t* pstr_CardData);

enuApp_Status_t AppMemory_getCardData(strCardData_t* pstr_CardData);

#endif /* APP_H_ */