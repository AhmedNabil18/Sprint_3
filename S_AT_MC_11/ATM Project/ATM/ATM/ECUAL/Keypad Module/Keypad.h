/*
 * Keypad.h
 *
 * Created: 7/23/2021 10:44:45 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Keypad_Cfg.h"
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- CONSTANTS -*-*-*-*-*-*/
#define KEYPAD_STATE_RELEASED		0U
#define KEYPAD_STATE_PRESSED		1U

/*******************************************************************************
 *                          Module Data Types                                  *
 *******************************************************************************/
/*
 * Data Type for KEYPAD return status
 */
typedef enum
{
	KEYPAD_STATUS_ERROR_NOK,
	KEYPAD_STATUS_ERROR_OK,
	KEYPAD_STATUS_ERROR_ID_INVALID,
	KEYPAD_STATUS_ERROR_NULL,
	KEYPAD_STATUS_NOT_INIT,
	KEYPAD_STATUS_INIT,
	KEYPAD_STATUS_PRESSED,
	KEYPAD_STATUS_NOT_PRESSED
}enuKeypad_Status_t;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
/* Function to initialize the Keypad module */
enuKeypad_Status_t Keypad_init(void);

/* Function to get the state of the Keypad */
enuKeypad_Status_t Keypad_readKey(uint8_t * pu8_pressedKey);


#endif /* KEYPAD_H_ */