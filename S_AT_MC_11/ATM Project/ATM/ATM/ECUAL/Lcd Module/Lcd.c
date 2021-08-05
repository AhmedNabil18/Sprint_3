/*
 * Lcd.c
 *
 * Created: 7/24/2021 10:37:51 AM
 *  Author: Ahmed Nabil
 */ 

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Lcd.h"
#include "../../MCAL/StringManipulation.h"
#include "../../MCAL/Delay Module/Delay.h"
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- GLOBAL STATIC VARIABLES *-*-*-*-*-*/
static enuLcd_Status_t genu_LcdModuleState = LCD_STATUS_NOT_INIT;
// static uint8_t gu8_currentColumn = 0;
// static uint8_t gu8_currentRow = 0;

static void Lcd_WriteData(uint8_t u8_Data)
{
	(u8_Data & 0x1)!=0 ? Dio_writePin(LCD_D4,PIN_HIGH) : Dio_writePin(LCD_D4,PIN_LOW);
	(u8_Data & 0x2)!=0 ? Dio_writePin(LCD_D5,PIN_HIGH) : Dio_writePin(LCD_D5,PIN_LOW);
	(u8_Data & 0x4)!=0 ? Dio_writePin(LCD_D6,PIN_HIGH) : Dio_writePin(LCD_D6,PIN_LOW);
	(u8_Data & 0x8)!=0 ? Dio_writePin(LCD_D7,PIN_HIGH) : Dio_writePin(LCD_D7,PIN_LOW);
}
static void Lcd_EnableToggle(void)
{
	ENABLE;
	Delay_ms(1);
	DISABLE;
}
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*- FUNCTIONS IMPLEMENTATION -*-*-*-*-*-*/

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Lcd_init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuLcd_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Initialize the Lcd module.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuLcd_Status_t Lcd_init(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Lcd module is already initialized */
	if (genu_LcdModuleState == LCD_STATUS_INIT)
	{
		return LCD_STATUS_INIT;
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
		return LCD_STATUS_ERROR_NOK;
	
	Delay_ms(20);
	CMD;
	DISABLE;
	
	Lcd_sendCommand(0x33);
	Lcd_sendCommand(0x32);
	Lcd_sendCommand(0x28);
	//Lcd_sendCommand(0x08);
	Lcd_sendCommand(0x0C);
	Lcd_sendCommand(0x06);
	Lcd_sendCommand(0x01);
	//Delay_ms(2);
	/* Change the state of the Lcd module to Initialized */
	genu_LcdModuleState = LCD_STATUS_INIT;
	return LCD_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Lcd_sendCommand
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_command - Command to be sent to the LCD.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuLcd_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Send a Command to the LCD.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuLcd_Status_t Lcd_sendCommand(uint8_t u8_command)
{
/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	
	CMD;
	Lcd_WriteData((u8_command & 0xF0)>>4); 
	Lcd_EnableToggle();
	Delay_ms(1);
	Lcd_WriteData(u8_command & 0x0F);
	Lcd_EnableToggle();
	Delay_ms(2);
	return LCD_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Lcd_setCursor
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_Row - Row Index.
*				   u8_Column - Column Index.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuLcd_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to print single character at the current position of the cursor in the LCD.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuLcd_Status_t Lcd_setCursor(uint8_t u8_Row, uint8_t u8_Column)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Lcd module is not initialized */
	if (genu_LcdModuleState != LCD_STATUS_INIT)
	{
		return LCD_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	/* Check if the Row index is out of range */
	if(u8_Row > LCD_ROWS_NUM)
	{
		return LCD_STATUS_ERROR_ROW_INVALID;
	}else{/*Nothing to here*/}
	/* Check if the Column index is out of range */
	if(u8_Column > LCD_COLUMNS_NUM)
	{
		return LCD_STATUS_ERROR_COL_INVALID;
	}else{/*Nothing to here*/}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	    uint8_t u8_Command=0x00;
		u8_Command = LCD_ROW_1_INDEX + u8_Row*0x40 + u8_Column;
	    Lcd_sendCommand(u8_Command);
		
	return LCD_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Lcd_printChar
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_data - Character to be printed on the LCD.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuLcd_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to print single character at the current position of the cursor in the LCD.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuLcd_Status_t Lcd_printChar(uint8_t u8_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Lcd module is not initialized */
	if (genu_LcdModuleState != LCD_STATUS_INIT)
	{
		return LCD_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	DATA;             // => RS = 1
	Lcd_WriteData((u8_data & 0xF0)>>4);             //Data transfer
	Lcd_EnableToggle();
	Delay_ms(1);
	Lcd_WriteData(u8_data & 0x0F);
	Lcd_EnableToggle();
	Delay_ms(2);
	return LCD_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Lcd_printString
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): pu8_data - Pointer to string to be printed on the LCD.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuLcd_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to print string at the current position of the cursor in the LCD.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuLcd_Status_t Lcd_printString(uint8_t *pu8_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Lcd module is not initialized */
	if (genu_LcdModuleState != LCD_STATUS_INIT)
	{
		return LCD_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	while (*pu8_data != '\0')
	{
		Lcd_printChar(*pu8_data++);
	}
		
	return LCD_STATUS_ERROR_OK;
}


enuLcd_Status_t Lcd_printLCD(uint8_t *pu8_data1, uint8_t *pu8_data2)
{
	Lcd_clear();
	if(Lcd_setCursor(0, 0) != LCD_STATUS_ERROR_OK)
		return LCD_STATUS_ERROR_NOK;
	if(Lcd_printString(pu8_data1) != LCD_STATUS_ERROR_OK)
		return LCD_STATUS_ERROR_NOK;
	if(Lcd_setCursor(1,0) != LCD_STATUS_ERROR_OK)
		return LCD_STATUS_ERROR_NOK;
	if(Lcd_printString(pu8_data2) != LCD_STATUS_ERROR_OK)
		return LCD_STATUS_ERROR_NOK;
	return LCD_STATUS_ERROR_OK;
}

enuLcd_Status_t Lcd_clear(void)
{
	Lcd_sendCommand(0x01);
	return LCD_STATUS_ERROR_OK;
}

enuLcd_Status_t Lcd_shiftRight(void)
{
	Lcd_sendCommand(0x1C);
	return LCD_STATUS_ERROR_OK;
}

enuLcd_Status_t Lcd_shiftLeft(void)
{
	Lcd_sendCommand(0x18);
	return LCD_STATUS_ERROR_OK;
}
enuLcd_Status_t Lcd_printBinary(uint16_t u16_num)
{
	uint8_t au8_snum[17];
	integerToString(u16_num, au8_snum, BIN);
	Lcd_printString(au8_snum);
	return LCD_STATUS_ERROR_OK;
}
enuLcd_Status_t Lcd_printDecimal(uint16_t u16_num)
{
	uint8_t au8_snum[17];
	integerToString(u16_num, au8_snum, DEC);
	Lcd_printString(au8_snum);
	return LCD_STATUS_ERROR_OK;
}
enuLcd_Status_t Lcd_printHexa(uint16_t u16_num)
{
	uint8_t au8_snum[17];
	integerToString(u16_num, au8_snum, HEX);
	Lcd_printString(au8_snum);
	return LCD_STATUS_ERROR_OK;
}