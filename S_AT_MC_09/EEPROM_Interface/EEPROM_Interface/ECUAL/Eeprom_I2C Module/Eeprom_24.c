/*
 * Eeprom_24.c
 *
 * Created: 7/22/2021 4:22:11 AM
 *  Author: Ahmed Nabil
 */ 

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Eeprom_24.h"

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- GLOBAL STATIC VARIABLES *-*-*-*-*-*/
static enuEeprom_24_Status_t genu_eepromModuleState = EEPROM_24_STATUS_NOT_INIT;


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- GLOBAL STATIC Functions *-*-*-*-*-*/
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: EEPROM_getAddresses
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u16_location - Location address needed to be accessed.
* Parameters (inout): None
* Parameters (out): pu8_slaveAddress - Pointer to variable to hold the slave address.
*					pu8_wordAddress - Pointer to variable to hold the word address.
* Return value: enuEeprom_24_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Calculate the slave address byte and the word address byte from the location
*				given as argument.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
static enuEeprom_24_Status_t EEPROM_getAddresses(uint8_t *pu8_slaveAddress, uint8_t *pu8_wordAddress, uint16_t u16_location)
{
	*pu8_slaveAddress = EEPROM_24_SLV_ADDRESS | (uint8_t)((u16_location & 0x0700)>>7);
	*pu8_wordAddress = u16_location & 0xFF;
	return EEPROM_24_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*- FUNCTIONS IMPLEMENTATION -*-*-*-*-*-*/


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Eeprom_24_init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuEeprom_24_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Initialize the EEPROM Module.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuEeprom_24_Status_t Eeprom_24_init(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the EEPROM module is already initialized */
	if (EEPROM_24_STATUS_INIT == genu_eepromModuleState)
	{
		return EEPROM_24_STATUS_INIT;
	}else{/*Nothing to here*/}
		
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Initialize the Master I2C Module */
	enuI2C_Status_t enu_I2CStatus = I2C_MasterInit();
	if((enu_I2CStatus == I2C_STATUS_NOT_INIT) && (enu_I2CStatus != I2C_STATUS_ALREADY_INIT))
	{
		return EEPROM_24_STATUS_ERROR_NOK;
	}
	/* Set the EEPROM Module State to Initialized*/
	genu_eepromModuleState = EEPROM_24_STATUS_INIT;
	return EEPROM_24_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Eeprom_24_writeByte
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u16_location - Location address of a word to put the data in.
*				   u8_data - Data to be written inside the memory location.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuEeprom_24_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Write a byte inside a specific location in memory.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuEeprom_24_Status_t  Eeprom_24_writeByte(uint16_t u16_location, uint8_t u8_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the EEPROM module is not initialized */
	if (EEPROM_24_STATUS_INIT != genu_eepromModuleState)
	{
		return EEPROM_24_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint8_t u8_slaveAddr=0;
	uint8_t u8_wordAddr=0;
	
	if(EEPROM_getAddresses(&u8_slaveAddr, &u8_wordAddr, u16_location) != EEPROM_24_STATUS_ERROR_OK)
		return EEPROM_24_STATUS_ERROR_NOK;
	
	if(I2C_MasterSendToLocation(u8_slaveAddr, u8_wordAddr, &u8_data, SINGLE_BYTE) != I2C_STATUS_ERROR_OK)
		return EEPROM_24_STATUS_ERROR_NOK;
	return EEPROM_24_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Eeprom_24_readByte
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u16_location - Location address of a word to read the data from.
* Parameters (inout): None
* Parameters (out): pu8_data - Pointer to variable to hold Data read from memory location.
* Return value: enuEeprom_24_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Read a byte inside a specific location in memory.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuEeprom_24_Status_t  Eeprom_24_readByte(uint16_t u16_location, uint8_t* pu8_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the EEPROM module is not initialized */
	if (EEPROM_24_STATUS_INIT != genu_eepromModuleState)
	{
		return EEPROM_24_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	/* Check if Argument is NULL PTR */
	if (pu8_data == NULL_PTR)
	{
		return EEPROM_24_STATUS_ERROR_NULL;
	}
	else{/*Nothing to here*/}
		
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint8_t slv_addr=0;
	uint8_t word_addr=0;
	if(EEPROM_getAddresses(&slv_addr, &word_addr, u16_location) != EEPROM_24_STATUS_ERROR_OK)
		return EEPROM_24_STATUS_ERROR_NOK;
	
	if(I2C_MasterReceiveFromLocation(slv_addr, word_addr, pu8_data, SINGLE_BYTE) != I2C_STATUS_ERROR_OK)
		return EEPROM_24_STATUS_ERROR_NOK;
	return EEPROM_24_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Eeprom_24_writePage
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_page - The page number to write data in it.
*				   pu8_data - Pointer to Data to be written to memory location.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuEeprom_24_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Write Page of data bytes to a specific page.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuEeprom_24_Status_t  Eeprom_24_writePage(uint8_t u8_page, uint8_t *pu8_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the EEPROM module is not initialized */
	if (EEPROM_24_STATUS_INIT != genu_eepromModuleState)
	{
		return EEPROM_24_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	/* Check if Argument is NULL PTR */
	if (pu8_data == NULL_PTR)
	{
		return EEPROM_24_STATUS_ERROR_NULL;
	}
	else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint16_t u8_startAddr = ((uint16_t)u8_page)<<4;
	uint8_t u8_slaveAddr=0;
	uint8_t u8_wordAddr=0;
	
	if(EEPROM_getAddresses(&u8_slaveAddr, &u8_wordAddr, u8_startAddr) != EEPROM_24_STATUS_ERROR_OK)
		return EEPROM_24_STATUS_ERROR_NOK;
	
	if(I2C_MasterSendToLocation(u8_slaveAddr, u8_wordAddr, pu8_data, EEPROM_24_PAGE_BYTES) != I2C_STATUS_ERROR_OK)
		return EEPROM_24_STATUS_ERROR_NOK;

	return EEPROM_24_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Eeprom_24_readByte
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_page - The page number to read its data.
* Parameters (inout): None
* Parameters (out): pu8_data - Pointer to variable to hold Data read from memory page.
* Return value: enuEeprom_24_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Read data inside a specific page in memory.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuEeprom_24_Status_t  Eeprom_24_readPage(uint8_t u8_page, uint8_t *pu8_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the EEPROM module is not initialized */
	if (EEPROM_24_STATUS_INIT != genu_eepromModuleState)
	{
		return EEPROM_24_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	/* Check if Argument is NULL PTR */
	if (pu8_data == NULL_PTR)
	{
		return EEPROM_24_STATUS_ERROR_NULL;
	}
	else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint16_t u8_startAddr = ((uint16_t)u8_page)<<4;
	uint8_t u8_slaveAddr=0;
	uint8_t u8_wordAddr=0;
	
	if(EEPROM_getAddresses(&u8_slaveAddr, &u8_wordAddr, u8_startAddr) != EEPROM_24_STATUS_ERROR_OK)
		return EEPROM_24_STATUS_ERROR_NOK;
	
	if(I2C_MasterReceiveFromLocation(u8_slaveAddr, u8_wordAddr, pu8_data, EEPROM_24_PAGE_BYTES) != I2C_STATUS_ERROR_OK)
		return EEPROM_24_STATUS_ERROR_NOK;

	return EEPROM_24_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Eeprom_24_writePacket
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u16_location - Location address of a word to start reading data from.
*				   u16_dataLen - Length of data to be read from memory.
*				   pu8_data - Pointer to Data write to memory location.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuEeprom_24_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Write Multiple data bytes to a specific location in memory.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuEeprom_24_Status_t  Eeprom_24_writePacket(uint16_t u16_location, uint8_t *pu8_data, uint16_t u16_dataLen)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the EEPROM module is not initialized */
	if (EEPROM_24_STATUS_INIT != genu_eepromModuleState)
	{
		return EEPROM_24_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	/* Check if Argument is NULL PTR */
	if (pu8_data == NULL_PTR)
	{
		return EEPROM_24_STATUS_ERROR_NULL;
	}
	else{/*Nothing to here*/}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint8_t u8_slaveAddr=0;
	uint8_t u8_wordAddr=0;
	uint16_t u8_dataIndex=0;
	uint8_t i=0;
	
	if(EEPROM_getAddresses(&u8_slaveAddr, &u8_wordAddr, u16_location) != EEPROM_24_STATUS_ERROR_OK)
		return EEPROM_24_STATUS_ERROR_NOK;
	
	
	uint8_t u8_pageStart = (u16_location & 0x7F0)>>4;
	uint8_t u8_byteOffset = u16_location & 0x00F;
	
	if(u8_byteOffset != 0)
	{
		uint8_t length = EEPROM_24_PAGE_BYTES - u8_byteOffset;
		if(I2C_MasterSendToLocation(u8_slaveAddr, u8_wordAddr, pu8_data, length) != I2C_STATUS_ERROR_OK)
			return EEPROM_24_STATUS_ERROR_NOK;
		u8_dataIndex = length;
		u16_dataLen = u16_dataLen - length;
		i=1;
	}
	
	uint8_t pages_num = u16_dataLen/EEPROM_24_PAGE_BYTES;
	while (pages_num--)
	{
		if(Eeprom_24_writePage(u8_pageStart+i, pu8_data+u8_dataIndex) != EEPROM_24_STATUS_ERROR_OK)
			return EEPROM_24_STATUS_ERROR_NOK;
		i++;
		u16_dataLen -= 16;
		u8_dataIndex += 16;
	}
	
	if (u16_dataLen > 0)
	{
		uint16_t u16_newLocation = (u8_pageStart + i) << 4;
		if(EEPROM_getAddresses(&u8_slaveAddr, &u8_wordAddr, u16_newLocation) != EEPROM_24_STATUS_ERROR_OK)
			return EEPROM_24_STATUS_ERROR_NOK;
		if(I2C_MasterSendToLocation(u8_slaveAddr, u8_wordAddr, pu8_data+u8_dataIndex, u16_dataLen) != I2C_STATUS_ERROR_OK)
			return EEPROM_24_STATUS_ERROR_NOK;
	}
	return EEPROM_24_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Eeprom_24_readPacket
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u16_location - Location address of a word to start reading data from.
*				   u16_dataLen - Length of data to be read from memory.
* Parameters (inout): None
* Parameters (out): pu8_data - Pointer to variable to hold Data read from memory location.
* Return value: enuEeprom_24_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Read Multiple data bytes from a specific location in memory.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuEeprom_24_Status_t  Eeprom_24_readPacket(uint16_t u16_location, uint8_t *pu8_data, uint16_t u16_dataLen)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the EEPROM module is not initialized */
	if (EEPROM_24_STATUS_INIT != genu_eepromModuleState)
	{
		return EEPROM_24_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	/* Check if Argument is NULL PTR */
	if (pu8_data == NULL_PTR)
	{
		return EEPROM_24_STATUS_ERROR_NULL;
	}
	else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint8_t u8_slaveAddr=0;
	uint8_t u8_wordAddr=0;
	uint16_t u16dataIndex=0;
	uint8_t i=0;
	
	if(EEPROM_getAddresses(&u8_slaveAddr, &u8_wordAddr, u16_location) != EEPROM_24_STATUS_ERROR_OK)
		return EEPROM_24_STATUS_ERROR_NOK;
	
	
	uint8_t u8_pageStart = (u16_location & 0x7F0)>>4;		//The first page to write in
	uint8_t u8_byteOffset = u16_location & 0x00F;
	
	if(u8_byteOffset != 0)
	{
		uint8_t length = EEPROM_24_PAGE_BYTES - u8_byteOffset;
		if(I2C_MasterReceiveFromLocation(u8_slaveAddr, u8_wordAddr, pu8_data, length) != I2C_STATUS_ERROR_OK)
			return EEPROM_24_STATUS_ERROR_NOK;
		u16dataIndex = length;
		u16_dataLen = u16_dataLen - length;
		i=1;
	}
	
	uint8_t pages_num = u16_dataLen/EEPROM_24_PAGE_BYTES;				//Number of pages needed
	while (pages_num--)
	{
		if(Eeprom_24_readPage(u8_pageStart+i, pu8_data+u16dataIndex) != EEPROM_24_STATUS_ERROR_OK)
			return EEPROM_24_STATUS_ERROR_NOK;
		i++;
		u16_dataLen -= 16;
		u16dataIndex += 16;
	}

	if (u16_dataLen > 0)
	{
		uint16_t new_location = (u8_pageStart + i) << 4;
		if(EEPROM_getAddresses(&u8_slaveAddr, &u8_wordAddr, new_location) != EEPROM_24_STATUS_ERROR_OK)
			return EEPROM_24_STATUS_ERROR_NOK;
		if(I2C_MasterReceiveFromLocation(u8_slaveAddr, u8_wordAddr, pu8_data+u16dataIndex, u16_dataLen) != I2C_STATUS_ERROR_OK)
			return EEPROM_24_STATUS_ERROR_NOK;
	}
	return EEPROM_24_STATUS_ERROR_OK;
}