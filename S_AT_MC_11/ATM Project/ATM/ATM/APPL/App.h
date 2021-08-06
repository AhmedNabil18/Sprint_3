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
#include "../ECUAL/Keypad Module/Keypad.h"
#include "../ECUAL/Lcd Module/Lcd.h"
#include "../ECUAL/LM35 Module/LM35.h"
#include "../ECUAL/Motor Module/Motor.h"
#include "../ECUAL/Button Module/Button.h"
#include "../MCAL/Dio Module/Dio.h"
#include "../MCAL/Spi Module/Spi.h"
#include "../MCAL/Delay Module/Delay.h"
#include "../MCAL/StringManipulation.h"

#define HARDWARE		0U
#define SIMULATION		1U


#define PROJECT			SIMULATION

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- CONSTANTS -*-*-*-*-*-*/
#define ATM_MODE_ADMIN				0U
#define ATM_MODE_USER				1U
#define ATM_MODE_IDLE				2U

#define ATM_DB_FLAG_ADDR						0x00 // 1 Byte
#define ATM_DB_FLAG_SET_VAL						0xAA

#define ATM_DB_CLIENT_DATA_LEN_ADDR				0x01 // 1 Byte
#define ATM_DB_CLIENT_DATA_LEN_VAL				0x0F

#define ATM_DB_ACC_NUM_ADDR						0x02 // 1 Byte

#define ATM_DB_MAX_AMNT_ADDR					0x03 // 8 Bytes

#define ATM_DB_ATM_PIN_ADDR						0x0B // 5 Bytes

#define ATM_DB_MAX_ACC_NUM_ADDR					0x10 // 2 Bytes

#if PROJECT == HARDWARE
#define ATM_DB_CUSTOMER_PAN_BASE_ADDR			0x40 // 1 Page for each customer PAN
#define ATM_DB_CUSTOMER_BAL_BASE_ADDR			0x4A // 1 Page for each customer Balance
#define ATM_DB_CUSTOMER_DATA_SIZE				64U
#elif PROJECT == SIMULATION
#define ATM_DB_CUSTOMER_PAN_BASE_ADDR			0x20 // 1 Page for each customer PAN
#define ATM_DB_CUSTOMER_BAL_BASE_ADDR			0x30 // 1 Page for each customer Balance
#define ATM_DB_CUSTOMER_DATA_SIZE				32U
#endif

#define MAX_PIN_TRIAL			3U

#define DISPLAY_TEMP			1U
#define CARD_IN					1U
#define CARD_OUT				0U

#define CARD_FRAME_LENGTH		28U
#define MAX_NAME_LENGTH			9U
#define MAX_PAN_LENGTH			9U
#define MAX_PIN_LENGTH			4U
#define MAX_BAL_LENGTH			7U
#define USER_IDLE				0U
#define USER_BUSY				1U

#define ADMIN_NOT_REQUESTED		0U
#define ADMIN_REQUESTED			1U

#define ATM_NOT_REQUESTED		0U
#define ATM_REQUESTED			1U

#define LCD_IN_POS_X			1U
#define LCD_IN_POS_Y			6U
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- Data Types -*-*-*-*-*-*/
typedef struct
{
	uint8_t au8_cardHolderName[10];
	uint8_t au8_primaryAccountNumber[10];
	uint8_t au8_pinNum[5];
}strCardData_t;

typedef struct
{
	uint8_t au8_PAN[10];
	uint8_t au8_Balance[8];
}strClientData_t;
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
	APP_STATUS_NO_OP,
	APP_STATUS_KPD_NUM,
	APP_STATUS_PIN_CORRECT,
	APP_STATUS_PIN_NOT_CORRECT,
	APP_STATUS_PAN_FOUND,
	APP_STATUS_PAN_NOT_FOUND
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

enuApp_Status_t AppADMIN_getAtmPIN(uint8_t* pu8_data);

enuApp_Status_t AppADMIN_getInput(uint8_t* pu8_data);

enuApp_Status_t AppADMIN_getnewMaxAmount(uint8_t* pu8_data);

sint16_t AppADMIN_searchPan(uint8_t* pu8_data);

enuApp_Status_t AppADMIN_processNewCustomer(void);

enuApp_Status_t AppADMIN_processExistingCustomer(void);

enuApp_Status_t AppADMIN_getCustomerPAN(uint8_t* pu8_data);

enuApp_Status_t AppADMIN_getCustomerBalance(uint8_t* pu8_data);

enuApp_Status_t AppADMIN_printOptions(void);

enuApp_Status_t AppUSER_ReportKeypad(uint8_t* pu8_key);

enuApp_Status_t AppUSER_startTransaction(void);

enuApp_Status_t AppUSER_checkPin(void);

enuApp_Status_t AppUSER_checkPan(void);

enuApp_Status_t App_ReportTerminal(uint8_t* pu8_data);

enuApp_Status_t AppUSER_getCardData(strCardData_t* pstr_CardData, uint8_t *pu8_dataString);

enuApp_Status_t AppUSER_getCardData(strCardData_t* pstr_CardData, uint8_t *pu8_dataString);

enuApp_Status_t AppUSER_startProcess(strCardData_t* pstr_CardData);

enuApp_Status_t AppUSER_insertCard(void);

enuApp_Status_t AppUSER_displayTemp(void);
#endif /* APP_H_ */