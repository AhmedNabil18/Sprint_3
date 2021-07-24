/*
 * Terminal.c
 *
 * Created: 7/20/2021 9:22:31 PM
 *  Author: Ahmed Nabil
 */ 

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Terminal.h"



/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- GLOBAL STATIC VARIABLES *-*-*-*-*-*/
static enuTerminal_Status_t genu_TerminalModuleState = TERMINAL_STATUS_NOT_INIT;
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*- FUNCTIONS IMPLEMENTATION -*-*-*-*-*-*/

uint8_t gau8_data[MAX_INPUT_SIZE]={0};
uint8_t gu8_counter=0;
uint8_t gu8_flag=0;

void Uart_RXC_ISR(void)
{
	gau8_data[gu8_counter] = Uart_DataRegister();
	if (gau8_data[gu8_counter] == '\r')
	{
		Uart_sendByte(gau8_data[gu8_counter]);
		gau8_data[gu8_counter] = '\0';
		gu8_counter = 0;
		gu8_flag = 1;
	}else if( (gau8_data[gu8_counter] == '\b') && (gu8_counter != 0))
	{
		Uart_sendByte(gau8_data[gu8_counter]);
		gu8_counter--;
	}
	else if(gau8_data[gu8_counter] != '\b')
	{
		Uart_sendByte(gau8_data[gu8_counter]);
		gu8_counter++;
	}
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Terminal_init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuLed_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Initialize the Terminal module.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuTerminal_Status_t Terminal_init(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Terminal module is already initialized */
	if (genu_TerminalModuleState == TERMINAL_STATUS_INIT)
	{
		return TERMINAL_STATUS_INIT;
	}else{/*Nothing to here*/}
		
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Initialize the Uart Module */
	enuUart_Status_t Uart_State = Uart_init();
	if((UART_STATUS_ERROR_OK != Uart_State) && (UART_STATUS_ALREADY_INIT != Uart_State))
		return TERMINAL_STATUS_ERROR_NOK;
	Uart_EnableNotification_RXC(Uart_RXC_ISR);	
	/* Change the state of the Terminal module to Initialized */
	genu_TerminalModuleState = TERMINAL_STATUS_INIT;
	return TERMINAL_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Terminal_Out
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): pu8_OutputData - Pointer to a string to be printed on the terminal
* Parameters (inout): None
* Parameters (out): None
* Return value: enuLed_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to print a string on the terminal
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuTerminal_Status_t Terminal_Out(uint8_t *pu8_OutputData)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the argument is null pointer */
	if (pu8_OutputData == NULL_PTR)
	{
		return TERMINAL_STATUS_ERROR_NULL;
	}else{/*Nothing to here*/}
	/* Check if the Terminal module is not initialized */
	if (genu_TerminalModuleState == TERMINAL_STATUS_NOT_INIT)
	{
		return TERMINAL_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Put Output String on Terminal */
	enuUart_Status_t Uart_State = Uart_sendPacket(pu8_OutputData, stringLength(pu8_OutputData));
	if(UART_STATUS_ERROR_OK != Uart_State)
		return TERMINAL_STATUS_ERROR_NOK;
	return TERMINAL_STATUS_ERROR_OK;
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Terminal_In
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_InputData - Pointer to an input string from the terminal.
* Return value: enuLed_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to get the input from the terminal.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuTerminal_Status_t Terminal_In(uint8_t *pu8_InputData)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the argument is null pointer */
	if (pu8_InputData == NULL_PTR)
	{
		return TERMINAL_STATUS_ERROR_NULL;
	}else{/*Nothing to here*/}
	/* Check if the Terminal module is not initialized */
	if (genu_TerminalModuleState == TERMINAL_STATUS_NOT_INIT)
	{
		return TERMINAL_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	if(gu8_flag == 1)
	{
		gu8_flag = 0;
		stringCopy(gau8_data, pu8_InputData);
		EmptyString(gau8_data);
		return TERMINAL_STATUS_INPUT_CHANGED;
	}
	return TERMINAL_STATUS_ERROR_OK;
}
