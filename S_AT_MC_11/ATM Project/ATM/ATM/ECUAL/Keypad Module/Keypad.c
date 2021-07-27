/*
 * Keypad.c
 *
 * Created: 7/23/2021 10:44:36 PM
 *  Author: Ahmed Nabil
 */ 
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Keypad.h"

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- GLOBAL STATIC VARIABLES *-*-*-*-*-*/
static enuKeypad_Status_t genu_KeypadModuleState = KEYPAD_STATUS_NOT_INIT;
extern uint8_t gau8_Columns[KEYPAD_COLUMNS_NUM];
extern uint8_t gau8_Rows[KEYPAD_ROWS_NUM];
extern uint8_t gau8_ButtonSymbols[KEYPAD_ROWS_NUM][KEYPAD_COLUMNS_NUM];
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*- FUNCTIONS IMPLEMENTATION -*-*-*-*-*-*/

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Keypad_init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuKeypad_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Initialize the Keypad module.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuKeypad_Status_t Keypad_init(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Keypad module is already initialized */
	if (genu_KeypadModuleState == KEYPAD_STATUS_INIT)
	{
		return KEYPAD_STATUS_INIT;
	}else{/*Nothing to here*/}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Initialize the DIO Module and check if any error returned */
	enuDio_Status_t Dio_State = Dio_init(strDio_pins);
	if((DIO_STATUS_ERROR_OK != Dio_State) && (DIO_STATUS_ALREADY_INIT != Dio_State))
		return KEYPAD_STATUS_ERROR_NOK;
		
	/* Change the state of the Keypad module to Initialized */
	genu_KeypadModuleState = KEYPAD_STATUS_INIT;
	return KEYPAD_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Keypad_readKey
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_pressedKey - Pointer to a Char to hold the pressed key.
* Return value: enuKeypad_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Read the current pressed Key.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuKeypad_Status_t Keypad_readKey(uint8_t * pu8_pressedKey)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Keypad module is not initialized */
	if (genu_KeypadModuleState != KEYPAD_STATUS_INIT)
	{
		return KEYPAD_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	/* Check if the pointer input is NULL */
	if (pu8_pressedKey == NULL_PTR)
	{
		return KEYPAD_STATUS_ERROR_NULL;
	}else{/*Nothing to here*/}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint8_t u8_ColumnIndex=0, u8_RowIndex=0;
	uint8_t u8_pinValue = PIN_HIGH;
	for(u8_ColumnIndex=0; u8_ColumnIndex<KEYPAD_COLUMNS_NUM; u8_ColumnIndex++)
	{
		/* Put Zero Logic on each column pin */
		if(Dio_writePin(gau8_Columns[u8_ColumnIndex], PIN_LOW) != DIO_STATUS_ERROR_OK)
			return KEYPAD_STATUS_ERROR_NOK;
			
		/* Loop on the Rows to find if any pin is held Low (Pressed) */
		for(u8_RowIndex=0; u8_RowIndex<KEYPAD_ROWS_NUM; u8_RowIndex++)
		{
			if(Dio_readPin(gau8_Rows[u8_RowIndex], &u8_pinValue) != DIO_STATUS_ERROR_OK)
				return KEYPAD_STATUS_ERROR_NOK;
			if(u8_pinValue == PIN_LOW)
			{
				if(Dio_writePin(gau8_Columns[u8_ColumnIndex], PIN_HIGH) != DIO_STATUS_ERROR_OK)
					return KEYPAD_STATUS_ERROR_NOK;
				*pu8_pressedKey = gau8_ButtonSymbols[u8_RowIndex][u8_ColumnIndex];
				return KEYPAD_STATUS_PRESSED;
			}
		}
		
		/* Return Back logic HIGH on the Column Pin */
		if(Dio_writePin(gau8_Columns[u8_ColumnIndex], PIN_HIGH) != DIO_STATUS_ERROR_OK)
			return KEYPAD_STATUS_ERROR_NOK;
	}
	
	return KEYPAD_STATUS_NOT_PRESSED;
}