/*
 * LM35.h
 *
 * Created: 7/25/2021 9:59:31 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef LM35_H_
#define LM35_H_

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "LM35_Cfg.h"
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- CONSTANTS -*-*-*-*-*-*/
#define LM35_STEP_VALUE				10U //10 milliVolts

/*******************************************************************************
 *                          Module Data Types                                  *
 *******************************************************************************/
/*
 * Data Type for LM35 return status
 */
typedef enum
{
	LM35_STATUS_ERROR_NOK,
	LM35_STATUS_ERROR_OK,
	LM35_STATUS_ERROR_ID_INVALID,
	LM35_STATUS_ERROR_NULL,
	LM35_STATUS_NOT_INIT,
	LM35_STATUS_INIT,
	LM35_STATUS_PRESSED,
	LM35_STATUS_NOT_PRESSED
}enuLM35_Status_t;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
/* Function to initialize the LM35 module */
enuLM35_Status_t LM35_init(void);

/* Function to read the temperature in celsius */
enuLM35_Status_t LM35_readTemp(uint8_t * pu8_Temp);



#endif /* LM35_H_ */