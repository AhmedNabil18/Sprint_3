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

#define ATM_DB_CUSTOMER_PAN_BASE_ADDR			0x20 // 1 Page for each customer PAN
#define ATM_DB_CUSTOMER_BAL_BASE_ADDR			0x30 // 1 Page for each customer Balance

#define CARD_IN					1U
#define CARD_OUT				0U

#define MAX_NAME_LENGTH			9U
#define MAX_PAN_LENGTH			9U
#define MAX_PIN_LENGTH			4U

#define USER_IDLE				0U
#define USER_BUSY				1U

#define ADMIN_NOT_REQUESTED		0U
#define ADMIN_REQUESTED			1U

#define ATM_NOT_REQUESTED		0U
#define ATM_REQUESTED			1U

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
	APP_STATUS_KPD_NUM
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

enuApp_Status_t AppUSER_ReportKeypad(uint8_t* pu8_key);

enuApp_Status_t App_ReportTerminal(uint8_t* pu8_data);

enuApp_Status_t AppADMIN_getInput(uint8_t* pu8_data);

enuApp_Status_t AppADMIN_getCustomerPAN(uint8_t* pu8_data);

enuApp_Status_t AppADMIN_getCustomerBalance(uint8_t* pu8_data);

enuApp_Status_t AppADMIN_getAtmPIN(uint8_t* pu8_data);

enuApp_Status_t AppADMIN_saveNewCustomerData(void);

enuApp_Status_t AppUSER_getCardData(strCardData_t* pstr_CardData);

enuApp_Status_t AppADMIN_getnewMaxAmount(uint8_t* pu8_data);
#endif /* APP_H_ */