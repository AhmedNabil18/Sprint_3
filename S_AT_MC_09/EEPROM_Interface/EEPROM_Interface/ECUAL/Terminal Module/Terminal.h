/*
 * Terminal.h
 *
 * Created: 7/20/2021 9:22:22 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef TERMINAL_H_
#define TERMINAL_H_

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Terminal_Cfg.h"
#include "StringManipulation.h"
/*******************************************************************************
 *                          Module Data Types                                  *
 *******************************************************************************/
/*
 * Data Type for Terminal return status
 */
typedef enum
{
	TERMINAL_STATUS_ERROR_NOK,
	TERMINAL_STATUS_ERROR_OK,
	TERMINAL_STATUS_NOT_INIT,
	TERMINAL_STATUS_INIT,
	TERMINAL_STATUS_ERROR_NULL,
	TERMINAL_STATUS_INPUT_CHANGED
}enuTerminal_Status_t;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
/* Function to initialize the Terminal module */
enuTerminal_Status_t Terminal_init(void);

/* Function to put a string on the terminal */
enuTerminal_Status_t Terminal_Out(uint8_t *pu8_OutputData);

/* Function to get string input from the terminal */
enuTerminal_Status_t Terminal_In(uint8_t *pu8_InputData);

#endif /* TERMINAL_H_ */