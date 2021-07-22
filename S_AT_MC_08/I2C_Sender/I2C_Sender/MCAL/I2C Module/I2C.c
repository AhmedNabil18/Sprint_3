/*
 * I2C.c
 *
 * Created: 7/21/2021 8:34:53 PM
 *  Author: Ahmed Nabil
 */ 

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "I2C.h"
#include "..\ATMega32_Registers.h"


//static void (*I2C_STC_CallBackPtr)(void);
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- GLOBAL STATIC VARIABLES *-*-*-*-*-*/
static enuI2C_Status_t genuI2C_Status = I2C_STATUS_NOT_INIT;


/*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- FUNCTIONS IMPLEMENTATION -*-*-*-*-*-*/
#if I2C_MODE == I2C_MODE_MASTER
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: I2C_MasterInit
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuI2C_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Initialize the I2C Master module.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuI2C_Status_t I2C_MasterInit(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the I2C module is already initialized */
	if (I2C_STATUS_INIT == genuI2C_Status)
	{
		return I2C_STATUS_ALREADY_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	
	switch(PRESCALER_VALUE)
	{
		case I2C_PRESACLER_1:
			I2C_TWSR_REG = 0x00;
			break;
		case I2C_PRESACLER_4:
			I2C_TWSR_REG = 0x01;
			break;
		case I2C_PRESACLER_16:
			I2C_TWSR_REG = 0x02;
			break;
		case I2C_PRESACLER_64:
			I2C_TWSR_REG = 0x03;
			break;
		default:
			return I2C_STATUS_ERROR_NOK;
	}
	I2C_TWBR_REG = I2C_TWBR_VALUE();
	/* Set the I2C Module State to Initialized*/
	genuI2C_Status = I2C_STATUS_INIT;
	return I2C_STATUS_ERROR_OK;
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: I2C_MasterSendSTART
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuI2C_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Send Start bit on the bus.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuI2C_Status_t I2C_MasterSendSTART(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the I2C module is not initialized */
	if (I2C_STATUS_INIT != genuI2C_Status)
	{
		return I2C_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	I2C_START_ENABLE_FLAG;
	DIO_PORTB_DATA = 1<<0;
	while (I2C_WAIT_TWINT());
	if (I2C_readSTATUS() != I2C_SYMB_START)
		return I2C_STATUS_ERROR_NOK;
	return I2C_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: I2C_MasterSendSlaveAddress
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_salveAddress - Address of the slave to communicate with.
*				   u8_R_W - I2C_READ or I2C_WRITE Command.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuI2C_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Send Slave address on the bus.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuI2C_Status_t I2C_MasterSendSlaveAddress(uint8_t u8_salveAddress, uint8_t u8_R_W)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the I2C module is not initialized */
	if (I2C_STATUS_INIT != genuI2C_Status)
	{
		return I2C_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	/* Check if the u8_R_W is not correct */
	if (I2C_STATUS_INIT == u8_R_W)
	{
		return I2C_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint8_t u8_data = (u8_salveAddress<<1)|u8_R_W;
	I2C_TWDR_REG = u8_data;
	I2C_ENABLE_FLAG;
	while(I2C_WAIT_TWINT());
	uint8_t u8_status = I2C_readSTATUS();
	switch (u8_status)
	{
		case I2C_SYMB_ARB_LOST:
			return I2C_SYMB_ARB_LOST;
		
		case I2C_SYMB_SLV_ADD_W_ACK:
			return I2C_STATUS_ERROR_OK;
		
		case I2C_SYMB_SLV_ADD_W_NACK:
		return I2C_SYMB_SLV_ADD_W_NACK;
		
		case I2C_SYMB_SLV_ADD_R_ACK:
			return I2C_STATUS_ERROR_OK;
		
		case I2C_SYMB_SLV_ADD_R_NACK:
			return I2C_SYMB_SLV_ADD_R_NACK;
		
		default:
		return I2C_STATUS_ERROR_NOK;
	}
	return I2C_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: I2C_MasterSendRepSTART
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuI2C_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Send Repeated Start bit on the bus.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuI2C_Status_t I2C_MasterSendRepSTART(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the I2C module is not initialized */
	if (I2C_STATUS_INIT != genuI2C_Status)
	{
		return I2C_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	I2C_START_ENABLE_FLAG;
	while (I2C_WAIT_TWINT());
	if (I2C_readSTATUS() != I2C_SYMB_REP_START)
		return I2C_STATUS_ERROR_NOK;
	return I2C_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: I2C_MasterSendSTOP
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuI2C_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Send Stop bit on the bus.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuI2C_Status_t I2C_MasterSendSTOP(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the I2C module is not initialized */
	if (I2C_STATUS_INIT != genuI2C_Status)
	{
		return I2C_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	I2C_STOP_ENABLE_FLAG;
	while(I2C_TWCR_REG & (1<<I2C_TWCR_TWSTO));
	return I2C_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: I2C_MasterSendByte
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_data - Data to be sent to the slave.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuI2C_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Send Data Byte to the slave.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuI2C_Status_t I2C_MasterSendByte(uint8_t u8_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the I2C module is not initialized */
	if (I2C_STATUS_INIT != genuI2C_Status)
	{
		return I2C_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	I2C_TWDR_REG = u8_data;
	I2C_ENABLE_FLAG;
	
	while (I2C_WAIT_TWINT());
	uint8_t u8_status = I2C_readSTATUS();
	if (u8_status == I2C_SYMB_DATA_Tx_ACK)
	return I2C_SYMB_DATA_Tx_ACK;
	if (u8_status == I2C_SYMB_DATA_Tx_NACK)
	return I2C_SYMB_DATA_Tx_NACK;
	else
	return I2C_STATUS_ERROR_NOK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: I2C_MasterReceiveByte_ACK
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): pu8_data - Pointer to variable to hold Data received from the slave.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuI2C_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Receive Single Data Byte from Slave with ACK.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuI2C_Status_t I2C_MasterReceiveByte_ACK(uint8_t *pu8_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the I2C module is not initialized */
	if (I2C_STATUS_INIT != genuI2C_Status)
	{
		return I2C_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	I2C_ENABLE_FLAG_ACK;
	while (I2C_WAIT_TWINT());
	if(I2C_SYMB_DATA_Rx_ACK != I2C_readSTATUS())
	return I2C_STATUS_ERROR_NOK;
	*pu8_data = I2C_TWDR_REG;
	return I2C_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: I2C_MasterReceiveByte_NACK
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): pu8_data - Pointer to variable to hold Data received from the slave.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuI2C_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Receive Single Data Byte from Slave with NACK.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuI2C_Status_t I2C_MasterReceiveByte_NACK(uint8_t *pu8_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the I2C module is not initialized */
	if (I2C_STATUS_INIT != genuI2C_Status)
	{
		return I2C_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	I2C_ENABLE_FLAG;
	while (I2C_WAIT_TWINT());
	if(I2C_SYMB_DATA_Rx_NACK != I2C_readSTATUS())
	return I2C_STATUS_ERROR_NOK;
	
	*pu8_data = I2C_TWDR_REG;
	return I2C_STATUS_ERROR_OK;
}

enuI2C_Status_t I2C_MasterSendPacket(uint8_t u8_slaveAddress, uint8_t * pu8_data, uint16_t u16_dataLen)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the I2C module is not initialized */
	if (I2C_STATUS_INIT != genuI2C_Status)
	{
		return I2C_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	if (I2C_MasterSendSTART() == I2C_STATUS_ERROR_NOK)	return I2C_STATUS_ERROR_NOK;
	if (I2C_MasterSendSlaveAddress(u8_slaveAddress, I2C_WRITE) != I2C_STATUS_ERROR_OK)	return I2C_STATUS_ERROR_NOK;
	
	while (u16_dataLen--)
	if (I2C_MasterSendByte(*pu8_data++) == I2C_STATUS_ERROR_NOK)	return I2C_STATUS_ERROR_NOK;
	
	if (I2C_MasterSendSTOP() == I2C_STATUS_ERROR_NOK)	return I2C_STATUS_ERROR_NOK;
	return I2C_STATUS_ERROR_OK;
}

enuI2C_Status_t I2C_MasterReceivePacket(uint8_t u8_slaveAddress, uint8_t * pu8_data, uint16_t u16_dataLen)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the I2C module is not initialized */
	if (I2C_STATUS_INIT != genuI2C_Status)
	{
		return I2C_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	if (I2C_MasterSendSTART() == I2C_STATUS_ERROR_NOK)	return I2C_STATUS_ERROR_NOK;
	if (I2C_MasterSendSlaveAddress(u8_slaveAddress, I2C_READ) != I2C_STATUS_ERROR_OK)	return I2C_STATUS_ERROR_NOK;
	
	while (--u16_dataLen)
	I2C_MasterReceiveByte_ACK(pu8_data++);
	
	I2C_MasterReceiveByte_NACK(pu8_data);
	if (I2C_MasterSendSTOP() == I2C_STATUS_ERROR_NOK)	return I2C_STATUS_ERROR_NOK;
	return I2C_STATUS_ERROR_OK;
	
}

enuI2C_Status_t I2C_MasterReceiveGeneral(uint8_t u8_slaveAddress, uint8_t * pu8_source, uint16_t u16_sourceLen, uint8_t * pu8_destination, uint16_t u16_destinationLen)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the I2C module is not initialized */
	if (I2C_STATUS_INIT != genuI2C_Status)
	{
		return I2C_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	if (I2C_MasterSendSTART() == I2C_STATUS_ERROR_NOK)	return I2C_STATUS_ERROR_NOK;
	if (I2C_MasterSendSlaveAddress(u8_slaveAddress, I2C_WRITE) != I2C_STATUS_ERROR_OK)	return I2C_STATUS_ERROR_NOK;
	
	while (u16_sourceLen--)
	if (I2C_MasterSendByte(*pu8_source++) == I2C_STATUS_ERROR_NOK)	return I2C_STATUS_ERROR_NOK;
	
	I2C_MasterSendRepSTART();
	
	if (I2C_MasterSendSlaveAddress(u8_slaveAddress, I2C_READ) != I2C_STATUS_ERROR_OK)	return I2C_STATUS_ERROR_NOK;
	
	while (--u16_destinationLen)
	I2C_MasterReceiveByte_ACK(pu8_destination++);
	
	I2C_MasterReceiveByte_NACK(pu8_destination);
	I2C_MasterSendSTOP();
	
	return I2C_STATUS_ERROR_OK;
}

enuI2C_Status_t I2C_MasterSendToLocation(uint8_t u8_slaveAddress, uint8_t u8_location ,uint8_t * pu8_data, uint16_t u16_dataLen)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the I2C module is not initialized */
	if (I2C_STATUS_INIT != genuI2C_Status)
	{
		return I2C_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	if (I2C_MasterSendSTART() == I2C_STATUS_ERROR_NOK)	return I2C_STATUS_ERROR_NOK;
	if (I2C_MasterSendSlaveAddress(u8_slaveAddress, I2C_WRITE) != I2C_STATUS_ERROR_OK)	return I2C_STATUS_ERROR_NOK;
	
	if (I2C_MasterSendByte(u8_location) == I2C_STATUS_ERROR_NOK)	return I2C_STATUS_ERROR_NOK;
	while (u16_dataLen--)
	if (I2C_MasterSendByte(*pu8_data++) == I2C_STATUS_ERROR_NOK)	return I2C_STATUS_ERROR_NOK;
	
	if (I2C_MasterSendSTOP() == I2C_STATUS_ERROR_NOK)	return I2C_STATUS_ERROR_NOK;
	return I2C_STATUS_ERROR_OK;
}

enuI2C_Status_t I2C_MasterReceiveFromLocation(uint8_t u8_slaveAddress, uint8_t u8_location ,uint8_t * pu8_data, uint16_t u16_dataLen)
{
	return I2C_MasterReceiveGeneral(u8_slaveAddress, &u8_location, 1, pu8_data, u16_dataLen);
}
#endif

#if I2C_MODE == I2C_MODE_SLAVE
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: I2C_SlaveInit
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuI2C_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Initialize the I2C Slave module.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuI2C_Status_t I2C_SlaveInit(uint8_t u8_OwnSlaveAddress)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the I2C module is already initialized */
	if (I2C_STATUS_INIT == genuI2C_Status)
	{
		return I2C_STATUS_ALREADY_INIT;
	}else{/*Nothing to here*/}
		
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Set own slave address of the device */
	I2C_TWAR_REG = u8_OwnSlaveAddress << 1;
	I2C_ENABLE_FLAG_ACK;
	/* Set the I2C Module State to Initialized*/
	genuI2C_Status = I2C_STATUS_INIT;
	return I2C_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: I2C_SlaveCheck
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuI2C_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Check any action came to the slave.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuI2C_Status_t I2C_SlaveCheck(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the I2C module is not initialized */
	if (I2C_STATUS_INIT != genuI2C_Status)
	{
		return I2C_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint8_t u8_status;
	
	while(!(I2C_TWCR_REG & (1<<I2C_TWCR_TWINT)));
	u8_status = I2C_readSTATUS();
	
	if((u8_status==I2C_SLV_OWN_ADDRESS_W) || (u8_status==I2C_SLV_ACK_RETURNED_W))
	return I2C_STATUS_SLAVE_READ;
	if((u8_status == I2C_SLV_OWN_ADDRESS_R) || (u8_status== I2C_SLV_ACK_RETURNED_R))
	return I2C_STATUS_SLAVE_WRITE;
	if((u8_status == I2C_SLV_GNRL_CALL) || (u8_status== I2C_SLV_GNRL_CALL_ARB_LST))
	return I2C_STATUS_SLAVE_GNRL_CALL;
	
	return I2C_STATUS_SLAVE_DEAF;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: I2C_SlaveSendByte
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_data - Data to be transmitted to the master
* Parameters (inout): None
* Parameters (out): None
* Return value: enuI2C_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to send data to the master.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuI2C_Status_t I2C_SlaveSendByte(uint8_t u8_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the I2C module is not initialized */
	if (I2C_STATUS_INIT != genuI2C_Status)
	{
		return I2C_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint8_t u8_status;
	
	I2C_TWDR_REG = u8_data;
	I2C_ENABLE_FLAG_ACK; 
	while(!(I2C_TWCR_REG & (1<<I2C_TWCR_TWINT)));
	
	u8_status = I2C_readSTATUS();
	if(u8_status == I2C_SLV_STOP_REP_START)	
	{
		I2C_TWCR_REG |= (1<<I2C_TWCR_TWINT);
		return I2C_STATUS_SLAVE_STOP;
	}
	if(u8_status==I2C_SLV_ACK_BY_MASTER)
		return I2C_STATUS_SLAVE_ACK;
	if(u8_status==I2C_SLV_NACK_BY_MASTER)
	{
		I2C_TWCR_REG |= (1<<I2C_TWCR_TWINT);
		return I2C_STATUS_SLAVE_NACK;
	}
	if(u8_status==I2C_SLV_LAST_DATA_ACK)
		return I2C_STATUS_SLAVE_NACK;	
	return I2C_STATUS_ERROR_NOK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: I2C_SlaveSendByte
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_data - Pointer to Variable to hold the data received by the Master
* Return value: enuI2C_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to receive byte from the master.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuI2C_Status_t I2C_SlaveReceiveByte(uint8_t *pu8_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the I2C module is not initialized */
	if (I2C_STATUS_INIT != genuI2C_Status)
	{
		return I2C_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint8_t u8_status;
	
	I2C_ENABLE_FLAG_ACK	;
	while(!(I2C_TWCR_REG & (1<<I2C_TWCR_TWINT)));
	
	u8_status = I2C_readSTATUS();
	if((u8_status == I2C_SLV_ACK_DATA_RX) || (u8_status == I2C_SLV_ACK_GENERAL_CALL))
	{
		*pu8_data = I2C_TWDR_REG;
		return I2C_STATUS_SLAVE_ACK;
	}else if((u8_status == I2C_SLV_NACK_DATA_RX) || (u8_status == I2C_SLV_NACK_GENERAL_CALL))
	{
		*pu8_data = I2C_TWDR_REG;
		return I2C_STATUS_SLAVE_NACK;	
	}else if(u8_status == I2C_SLV_STOP_REP_START)		
	{
		I2C_TWCR_REG |= (1<<I2C_TWCR_TWINT);
		DIO_PORTB_DATA = 1<<0;
		return I2C_STATUS_SLAVE_STOP;
	}
	return I2C_STATUS_ERROR_NOK;
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: I2C_SlaveReceive
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_data - Pointer to Variable to hold the data received by the Master
* Return value: enuI2C_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to receive multiple bytes from the master.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuI2C_Status_t I2C_SlaveReceive(uint8_t *pu8_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
/* Check if the I2C module is not initialized */
if (I2C_STATUS_INIT != genuI2C_Status)
{
	return I2C_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
		
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	do
	{
		if (I2C_SlaveReceiveByte(pu8_data) == I2C_STATUS_SLAVE_STOP)
			return I2C_STATUS_ERROR_OK;
		if(*(pu8_data++) == '\0') return I2C_STATUS_ERROR_OK;
	}while (1);
	return I2C_STATUS_ERROR_NOK;
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: I2C_SlaveSend
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): pu8_data - Pointer to Data to be sent to the master
*                 u16_dataLength - Length of data to be received from the master
* Parameters (inout): None
* Parameters (out): None
* Return value: enuI2C_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to send multiple bytes to the master.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuI2C_Status_t I2C_SlaveSend(uint8_t *pu8_data, uint16_t u16_dataLength)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the I2C module is not initialized */
	if (I2C_STATUS_INIT != genuI2C_Status)
	{
		return I2C_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
		
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	do
	{
		if (I2C_SlaveSendByte(*pu8_data) != I2C_STATUS_SLAVE_ACK)
			return I2C_STATUS_ERROR_OK;
		if(*(pu8_data++) == '\0') return I2C_STATUS_ERROR_OK;
	}while (u16_dataLength--);
	return I2C_STATUS_ERROR_NOK;
}
#endif