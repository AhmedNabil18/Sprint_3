/*
 * LM35.c
 *
 * Created: 7/25/2021 9:59:21 PM
 *  Author: Ahmed Nabil
 */ 

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "LM35.h"
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- GLOBAL STATIC VARIABLES *-*-*-*-*-*/
static enuLM35_Status_t genu_LM35ModuleState = LM35_STATUS_NOT_INIT;
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*- FUNCTIONS IMPLEMENTATION -*-*-*-*-*-*/

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: LM35_init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuLM35_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Initialize the LM35 module.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuLM35_Status_t LM35_init(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the LM35 module is already initialized */
	if (genu_LM35ModuleState == LM35_STATUS_INIT)
	{
		return LM35_STATUS_INIT;
	}else{/*Nothing to here*/}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Initialize the ADC Module and check if any error returned */
	if(ADC_STATUS_ERROR_OK !=Adc_init())
		return LM35_STATUS_ERROR_NOK;
		
	/* Change the state of the LM35 module to Initialized */
	genu_LM35ModuleState = LM35_STATUS_INIT;
	return LM35_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: LM35_readTemp
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_Temp - Pointer to a Char to hold the temperature.
* Return value: enuLM35_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Read the current temperature and return in celsius.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuLM35_Status_t LM35_readTemp(uint8_t * pu8_Temp)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the LM35 module is not initialized */
	if (genu_LM35ModuleState != LM35_STATUS_INIT)
	{
	return LM35_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	/* Check if the pointer input is NULL */
	if (pu8_Temp == NULL_PTR)
	{
		return LM35_STATUS_ERROR_NULL;
	}else{/*Nothing to here*/}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint32_t u32_tempMilliVolt = 0;
	if(Adc_start(LM35_ID_CHANNEL, &u32_tempMilliVolt) != ADC_STATUS_ERROR_OK)
		return LM35_STATUS_ERROR_NOK;
	*pu8_Temp = u32_tempMilliVolt/LM35_STEP_VALUE;
	return LM35_STATUS_ERROR_OK;
}