/*
 * Dio.c
 *
 * Created: 7/13/2021 7:54:01 PM
 *  Author: Ahmed Nabil
 */ 

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Dio.h"
#include "..\ATMega32_Registers.h"
#include "Dio_Cfg.h"


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- GLOBAL STATIC VARIABLES *-*-*-*-*-*/
static strDio_Config_t * pstrDio_pinsConfig = NULL_PTR;
static uint8_t u8_Dio_Status = DIO_NOT_INITIALIZED;


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*- FUNCTIONS IMPLEMENTATION -*-*-*-*-*-*/

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Dio_init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): pstrDio_pins - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: enuDio_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Initialize the Dio module.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuDio_Status_t Dio_init(strDio_Config_t* pstrDio_pins)
{
	uint8_t u8_loopIndex=0;
	/*-* check if the input configuration pointer is not a NULL_PTR *-*/
	if (NULL_PTR == pstrDio_pins)
	{
		return DIO_STATUS_NULL_ARGUMENT;
	}
	for (u8_loopIndex=0; u8_loopIndex<DIO_USED_PINS_NUM; u8_loopIndex++)
	{
		if ((pstrDio_pins[u8_loopIndex].u8_PinNum) > PIN_7)
		{
			return DIO_STATUS_PIN_NUM_INVALID;
		}
		switch (pstrDio_pins[u8_loopIndex].u8_PortNum)
		{
			case PORT_A:
				DIO_PORTA_DIR &= ~(1<<(pstrDio_pins[u8_loopIndex].u8_PinNum));
				DIO_PORTA_DIR |= (pstrDio_pins[u8_loopIndex].u8_Direction)<<(pstrDio_pins[u8_loopIndex].u8_PinNum);
				DIO_PORTA_DATA &= ~(1<<(pstrDio_pins[u8_loopIndex].u8_PinNum));
				DIO_PORTA_DATA |= (pstrDio_pins[u8_loopIndex].u8_InitState)<<(pstrDio_pins[u8_loopIndex].u8_PinNum);
				break;
			case PORT_B:
				DIO_PORTB_DIR &= ~(1<<(pstrDio_pins[u8_loopIndex].u8_PinNum));
				DIO_PORTB_DIR |= (pstrDio_pins[u8_loopIndex].u8_Direction)<<(pstrDio_pins[u8_loopIndex].u8_PinNum);
				DIO_PORTB_DATA &= ~(1<<(pstrDio_pins[u8_loopIndex].u8_PinNum));
				DIO_PORTB_DATA |= (pstrDio_pins[u8_loopIndex].u8_InitState)<<(pstrDio_pins[u8_loopIndex].u8_PinNum);
				break;
			case PORT_C:
				DIO_PORTC_DIR &= ~(1<<(pstrDio_pins[u8_loopIndex].u8_PinNum));
				DIO_PORTC_DIR |= (pstrDio_pins[u8_loopIndex].u8_Direction)<<(pstrDio_pins[u8_loopIndex].u8_PinNum);
				DIO_PORTC_DATA &= ~(1<<(pstrDio_pins[u8_loopIndex].u8_PinNum));
				DIO_PORTC_DATA |= (pstrDio_pins[u8_loopIndex].u8_InitState)<<(pstrDio_pins[u8_loopIndex].u8_PinNum);
				break;
			case PORT_D:
				DIO_PORTD_DIR &= ~(1<<(pstrDio_pins[u8_loopIndex].u8_PinNum));
				DIO_PORTD_DIR |= (pstrDio_pins[u8_loopIndex].u8_Direction)<<(pstrDio_pins[u8_loopIndex].u8_PinNum);
				DIO_PORTD_DATA &= ~(1<<(pstrDio_pins[u8_loopIndex].u8_PinNum));
				DIO_PORTD_DATA |= (pstrDio_pins[u8_loopIndex].u8_InitState)<<(pstrDio_pins[u8_loopIndex].u8_PinNum);
				break;
			default:
				return DIO_STATUS_PORT_NUM_INVALID;
		}
	}
	pstrDio_pinsConfig = pstrDio_pins;
	u8_Dio_Status = DIO_INITIALIZED;
	return DIO_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Dio_writePin
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): u8_pinID - Index to the pin in the configurations array of structures
*				  u8_pinValue - Value of the pin to write HIGH or LOW
* Parameters (inout): None
* Parameters (out): None
* Return value: enuDio_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Write data on a Pin.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuDio_Status_t Dio_writePin(uint8_t u8_pinID, uint8_t u8_pinValue)
{
	/*-* Check if the pinID is out of configured range *-*/
	if(u8_pinID > DIO_USED_PINS_NUM)
	{
		return DIO_STATUS_PIN_ID_INVALID;
	}
	/*-* Check if the module is not initialized *-*/
	if(u8_Dio_Status == DIO_NOT_INITIALIZED)
	{
		return DIO_STATUS_NOT_INIT;
	}
	switch (pstrDio_pinsConfig[u8_pinID].u8_PortNum)
	{
		case PORT_A:
		if(u8_pinValue == PIN_HIGH)
			DIO_PORTA_DATA |= 1<<(pstrDio_pinsConfig[u8_pinID].u8_PinNum);
		else	
			DIO_PORTA_DATA &= ~(1<<(pstrDio_pinsConfig[u8_pinID].u8_PinNum));
		
		break;
		case PORT_B:
			if(u8_pinValue == PIN_HIGH)
				DIO_PORTB_DATA |= 1<<(pstrDio_pinsConfig[u8_pinID].u8_PinNum);
			else
				DIO_PORTB_DATA &= ~(1<<(pstrDio_pinsConfig[u8_pinID].u8_PinNum));
			break;
		case PORT_C:
			if(u8_pinValue == PIN_HIGH)
				DIO_PORTC_DATA |= 1<<(pstrDio_pinsConfig[u8_pinID].u8_PinNum);
			else
				DIO_PORTC_DATA &= ~(1<<(pstrDio_pinsConfig[u8_pinID].u8_PinNum));
			break;
		case PORT_D:
			if(u8_pinValue == PIN_HIGH)
				DIO_PORTD_DATA |= 1<<(pstrDio_pinsConfig[u8_pinID].u8_PinNum);
			else
				DIO_PORTD_DATA &= ~(1<<(pstrDio_pinsConfig[u8_pinID].u8_PinNum));
			break;
		default:
		return DIO_STATUS_PORT_NUM_INVALID;
	}
	return DIO_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Dio_togglePin
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): u8_pinID - Index to the pin in the configurations array of structures
* Parameters (inout): None
* Parameters (out): None
* Return value: enuDio_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Toggle data on a Pin.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuDio_Status_t Dio_togglePin(uint8_t u8_pinID)
{
	/*-* Check if the pinID is out of configured range *-*/
	if(u8_pinID > DIO_USED_PINS_NUM)
	{
		return DIO_STATUS_PIN_ID_INVALID;
	}
	/*-* Check if the module is not initialized *-*/
	if(u8_Dio_Status == DIO_NOT_INITIALIZED)
	{
		return DIO_STATUS_NOT_INIT;
	}
	switch (pstrDio_pinsConfig[u8_pinID].u8_PortNum)
	{
		case PORT_A:
		DIO_PORTA_DATA ^= (1<<(pstrDio_pinsConfig[u8_pinID].u8_PinNum));
		break;
		case PORT_B:
		DIO_PORTB_DATA ^= (1<<(pstrDio_pinsConfig[u8_pinID].u8_PinNum));
		break;
		case PORT_C:
		DIO_PORTC_DATA ^= (1<<(pstrDio_pinsConfig[u8_pinID].u8_PinNum));
		break;
		case PORT_D:
		DIO_PORTD_DATA ^= (1<<(pstrDio_pinsConfig[u8_pinID].u8_PinNum));
		break;
		default:
		return DIO_STATUS_PORT_NUM_INVALID;
	}	
	return DIO_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Dio_readPin
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): u8_pinID - Index to the pin in the configurations array of structures
* Parameters (inout): None
* Parameters (out): pu8_pinValue - pointer to variable to hold the current data of the pin
* Return value: enuDio_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Write data on a Pin.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuDio_Status_t Dio_readPin(uint8_t u8_pinID, uint8_t *pu8_pinValue)
{
	/*-* Check if the pinID is out of configured range *-*/
	if(u8_pinID > DIO_USED_PINS_NUM)
	{
		return DIO_STATUS_PIN_ID_INVALID;
	}
	/*-* Check if the module is not initialized *-*/
	if(u8_Dio_Status == DIO_NOT_INITIALIZED)
	{
		return DIO_STATUS_NOT_INIT;
	}
	switch (pstrDio_pinsConfig[u8_pinID].u8_PortNum)
	{
		case PORT_A:
		*pu8_pinValue = (DIO_PORTA_PIN & (1<<(pstrDio_pinsConfig[u8_pinID].u8_PinNum))) >> (pstrDio_pinsConfig[u8_pinID].u8_PinNum);
		break;
		case PORT_B:
		*pu8_pinValue = (DIO_PORTB_PIN & (1<<(pstrDio_pinsConfig[u8_pinID].u8_PinNum))) >> (pstrDio_pinsConfig[u8_pinID].u8_PinNum);
		break;
		case PORT_C:
		*pu8_pinValue = (DIO_PORTC_PIN & (1<<(pstrDio_pinsConfig[u8_pinID].u8_PinNum))) >> (pstrDio_pinsConfig[u8_pinID].u8_PinNum);
		break;
		case PORT_D:
		*pu8_pinValue = (DIO_PORTD_PIN & (1<<(pstrDio_pinsConfig[u8_pinID].u8_PinNum))) >> (pstrDio_pinsConfig[u8_pinID].u8_PinNum);
		break;
		default:
		return DIO_STATUS_PORT_NUM_INVALID;
	}
	return DIO_STATUS_ERROR_OK;
}