/*
 * Button.h
 *
 * Created: 7/15/2021 6:36:15 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Button_Cfg.h"
#include "../../MCAL/Dio Module/Dio.h"
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- CONSTANTS -*-*-*-*-*-*/
#define BUTTON_STATE_RELEASED		0U
#define BUTTON_STATE_PRESSED		1U

/*******************************************************************************
 *                          Module Data Types                                  *
 *******************************************************************************/
/*
 * Data Type for Button return status
 */
typedef enum
{
	BTTN_STATUS_ERROR_NOK,
	BTTN_STATUS_ERROR_OK,
	BTTN_STATUS_ERROR_ID_INVALID,
	BTTN_STATUS_ERROR_NULL,
	BTTN_STATUS_NOT_INIT,
	BTTN_STATUS_INIT,
	BTTN_ERROR_DIO_INVALID
}enuBttn_Status_t;

/*
 * Data Type for Button state
 */
typedef uint8_t u8_ButtonState_t;
/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
/* Function to initialize the button module */
enuBttn_Status_t Button_init(void);

/* Function to get the state of the button */
enuBttn_Status_t Button_getState(uint8_t u8_bttnID, u8_ButtonState_t * pu8_state);

/* Function to update the state of the button */
enuBttn_Status_t Button_updateState(uint8_t u8_bttnID);

#endif /* BUTTON_H_ */