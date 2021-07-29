/*
 * Spi.c
 *
 * Created: 7/21/2021 1:44:23 AM
 *  Author: Ahmed Nabil
 */ 

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Spi.h"
#include "..\ATMega32_Registers.h"


static void (*SPI_STC_CallBackPtr)(void);
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- GLOBAL STATIC VARIABLES *-*-*-*-*-*/
static enuSpi_Status_t genuSpi_Status = SPI_STATUS_NOT_INIT;


/*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- FUNCTIONS IMPLEMENTATION -*-*-*-*-*-*/

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Spi_init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSpi_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Initialize the Spi module.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuSpi_Status_t Spi_init(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Spi module is already initialized */
	if (SPI_STATUS_INIT == genuSpi_Status)
	{
		return SPI_STATUS_ALREADY_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Set the Direction and the Role of the SPI Module */
	#if SPI_ROLE == SPI_MASTER	/* If chosen to be master */
	DIO_PORTB_DIR |= (1<<SPI_MOSI_PIN) | (1<<SPI_SCK_PIN) | (1<<SPI_SS_PIN);
	DIO_PORTB_DIR &= ~(1<<SPI_MISO_PIN);
	SPI_SS_DISABLE();
	SPI_SPCR_REG |= 1<<SPI_SPCR_MSTR; //master enable
	/***********************************/
	/* Set the First bit to be shifted MSB or LSB */
	#if SPI_LSB_FIRST == SPI_LSB_OFF
	SPI_SPCR_REG &= ~(1<<SPI_SPCR_DORD); //msb first
	#else
	SPI_SPCR_REG |= (1<<SPI_SPCR_DORD); //lsb first
	#endif
	/***********************************/
	/* Set the Polarity and Phase Modes of the Module */
	#if SPI_MODE == SPI_MODE0
	SPI_SPCR_REG &= ~(1<<SPI_SPCR_CPOL);
	SPI_SPCR_REG &= ~(1<<SPI_SPCR_CPHA);
	#elif SPI_MODE == SPI_MODE1
	SPI_SPCR_REG &= ~(1<<SPI_SPCR_CPOL);
	SPI_SPCR_REG |= (1<<SPI_SPCR_CPHA);
	#elif SPI_MODE == SPI_MODE2
	SPI_SPCR_REG |= (1<<SPI_SPCR_CPOL);
	SPI_SPCR_REG &= ~(1<<SPI_SPCR_CPHA);
	#else
	SPI_SPCR_REG |= (1<<SPI_SPCR_CPOL);
	SPI_SPCR_REG |= (1<<SPI_SPCR_CPHA);
	#endif
	/***********************************/
	/* Set the Frequency Divisor */
	#if SPI_FOSC_DIV > SPI_FOSC_DIV128
	SPI_SPSR_REG |= (1<<SPI_SPSR_SPI2X);
	#endif
	SPI_SPCR_REG &= ~(1<<SPI_SPCR_SPR0); //fosc/2
	SPI_SPCR_REG &= ~(1<<SPI_SPCR_SPR1); //fosc/2
	SPI_SPCR_REG |= ((SPI_FOSC_DIV & 0x03)<<SPI_SPCR_SPR0); //fosc/2
	#else		/* If chosen to be slave */
	DIO_PORTB_DIR |= (1<<SPI_MISO_PIN);
	DIO_PORTB_DIR &= ~(1<<SPI_MOSI_PIN);
	DIO_PORTB_DIR &= ~(1<<SPI_SS_PIN);
	DIO_PORTB_DIR &= ~(1<<SPI_SCK_PIN);
	SPI_SPCR_REG &= ~(1<<SPI_SPCR_MSTR); //slave enable
	#endif
	/* Enable the SPI Module */
	SPI_SPCR_REG |= 1<<SPI_SPCR_SPE; //SPI enable

	
	/* Set the SPI Module State to Initialized*/
	genuSpi_Status = SPI_STATUS_INIT;
	return SPI_STATUS_ERROR_OK;
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Spi_MasterSendByte
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_data - Data to be sent by the SPI Master to the slave
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSpi_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Send Byte by the Master to the slave.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if SPI_ROLE == SPI_MASTER
enuSpi_Status_t Spi_MasterSendByte(uint8_t u8_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Spi module is not initialized */
	if (SPI_STATUS_INIT != genuSpi_Status)
	{
		return SPI_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	volatile uint8_t u8_buffer;
	/* Put the data in the Data Register */
	SPI_SPDR_REG = u8_data;
	/* Wait for the transfer to complete */
	while(BIT_IS_CLR(SPI_SPSR_REG, SPI_SPSR_SPIF));
	/* Flush the buffer to clear the SPIF bit */
	u8_buffer = SPI_SPDR_REG;
	
	return SPI_STATUS_ERROR_OK;
}
#endif

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Spi_MasterSendPacket
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): pu8_Data - Pointer Data to be sent by the SPI Master to the slave
*				  u16_packetSize - Size of the given data string.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSpi_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Send Byte by the Master to the slave.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if SPI_ROLE == SPI_MASTER
enuSpi_Status_t Spi_MasterSendPacket(uint8_t *pu8_Data, uint16_t u16_packetSize)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Spi module is not initialized */
	if (SPI_STATUS_INIT != genuSpi_Status)
	{
		return SPI_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
		
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint8_t u8_loopIndex=0;
	
	for(u8_loopIndex=0; u8_loopIndex<u16_packetSize; u8_loopIndex++)
	{
		if(Spi_MasterSendByte(pu8_Data[u8_loopIndex]) != SPI_STATUS_ERROR_OK)
			return SPI_STATUS_ERROR_NOK;
	}
		
	return SPI_STATUS_ERROR_OK;
}
#endif

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Spi_MasterReceiveByte
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_data - Pointer to variable holds the data received.
* Return value: enuSpi_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to receive a single byte from a slave.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if SPI_ROLE == SPI_MASTER
enuSpi_Status_t Spi_MasterReceiveByte(uint8_t* pu8_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Spi module is not initialized */
	if (SPI_STATUS_INIT != genuSpi_Status)
	{
		return SPI_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
		
	/* Check if the Input pointer is Null */
	if (NULL_PTR == pu8_data)
	{
		return SPI_STATUS_NULL_ARGUMENT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/*
	 * Push dummy data in the Data register 
	 * to make a transfer and receive data from Slave 
	 */
	SPI_SPDR_REG = SPI_DUMMY_DATA;
	/* Wait for the transfer to complete */
	while(BIT_IS_CLR(SPI_SPSR_REG, SPI_SPSR_SPIF));
	/* Read the received Data from the Data register */
	*pu8_data = SPI_SPDR_REG;
	return SPI_STATUS_ERROR_OK;
}
#endif

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Spi_SlaveSendByte
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_data - Data to be sent by the SPI Master to the slave
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSpi_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Send Byte by the Slave to the master.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if SPI_ROLE == SPI_SLAVE
enuSpi_Status_t Spi_SlaveSendByte(uint8_t u8_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Spi module is not initialized */
	if (SPI_STATUS_INIT != genuSpi_Status)
	{
		return SPI_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Put the data in the Data Register */
	SPI_SPDR_REG = u8_data;
	/* Wait for the transfer to complete */
	while(BIT_IS_CLR(SPI_SPSR_REG, SPI_SPSR_SPIF));
	
	return SPI_STATUS_ERROR_OK;
}
#endif

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Spi_SlaveReceiveByte
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): pu8_data - Pointer to variable holds the data received.
* Return value: enuSpi_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Receive Single Data byte from master.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if SPI_ROLE == SPI_SLAVE
enuSpi_Status_t Spi_SlaveReceiveByte(uint8_t* pu8_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Spi module is not initialized */
	if (SPI_STATUS_INIT != genuSpi_Status)
	{
		return SPI_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
		
	/* Check if the Input pointer is Null */
	if (NULL_PTR == pu8_data)
	{
		return SPI_STATUS_NULL_ARGUMENT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Wait for the transfer to complete */
	while(BIT_IS_CLR(SPI_SPSR_REG, SPI_SPSR_SPIF));
	/* Read the received Data from the Data register */
	*pu8_data = SPI_SPDR_REG;
	return SPI_STATUS_ERROR_OK;
}
#endif
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Spi_MasterSendPacket
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u16_packetSize - Size of the given data string.
* Parameters (inout): None
* Parameters (out): pu8_Data - Pointer Data to be received by the SPI slave
* Return value: enuSpi_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Send Byte by the Master to the slave.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if SPI_ROLE == SPI_SLAVE
enuSpi_Status_t Spi_SlaveReceivePacket(uint8_t *pu8_Data, uint16_t u16_packetSize)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Spi module is not initialized */
	if (SPI_STATUS_INIT != genuSpi_Status)
	{
		return SPI_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
		
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint8_t u8_loopIndex=0;
		
	for(u8_loopIndex=0; u8_loopIndex<u16_packetSize; u8_loopIndex++)
	{
		if(Spi_SlaveReceiveByte(&pu8_Data[u8_loopIndex]) != SPI_STATUS_ERROR_OK)
			return SPI_STATUS_ERROR_NOK;
		if(pu8_Data[u8_loopIndex] == '\0')
			break;
	}
		
	return SPI_STATUS_ERROR_OK;
}
#endif

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Spi_EnableNotification
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Spi_Callback - Pointer to function to be called after sending 1 byte.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSpi_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Enable the Interrupt STC and assign Spi_Callback as a
*              callback function to it.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuSpi_Status_t Spi_EnableNotification(pfSpi_CallBack_t Spi_Callback)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Spi module is not initialized */
	if (SPI_STATUS_INIT != genuSpi_Status)
	{
		return SPI_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
		
	/* Check if the Input pointer is Null */
	if (NULL_PTR == Spi_Callback)
	{
		return SPI_STATUS_NULL_ARGUMENT;
	}else{/*Nothing to here*/}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/*- Enable the SPI Interrupt -*/
	SPI_SPCR_REG |= 1<<SPI_SPCR_SPIE;
	/* Assign the Callback function to the SPI's ISR Handler Caller */
	SPI_STC_CallBackPtr = Spi_Callback;
			
	return SPI_STATUS_ERROR_OK;
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Spi_DisableNotification
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuSpi_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Disable the SPI Interrupt.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuSpi_Status_t Spi_DisableNotification(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Spi module is not initialized */
	if (SPI_STATUS_INIT != genuSpi_Status)
	{
		return SPI_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/*- Disable the SPIE Interrupt -*/
	SPI_SPCR_REG &= ~(1<<SPI_SPCR_SPIE);
				
	return SPI_STATUS_ERROR_OK;
}
			
			
uint8_t Spi_DataRegister(void)
{
	return SPI_SPDR_REG;
}


ISR(SPI_STC_IRQ){(*SPI_STC_CallBackPtr)();}