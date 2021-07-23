/*
 * I2C.h
 *
 * Created: 7/21/2021 8:35:07 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef I2C_H_
#define I2C_H_

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "I2C_Cfg.h"
#include "..\Platform_Types.h"
#include "..\ATMega32_Interrupts.h"


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- CONSTANTS -*-*-*-*-*-*/

/*******************************************************
 * STATUS REGISTERS SYMBOLS
 ******************************************************/

#define I2C_PRESACLER_1					1U
#define I2C_PRESACLER_4					4U
#define I2C_PRESACLER_16				16U
#define I2C_PRESACLER_64				64U

#define I2C_MODE_MASTER					0U
#define I2C_MODE_SLAVE					1U

#define I2C_SYMB_START					0x08UL
#define I2C_SYMB_REP_START				0x10UL

#define I2C_SYMB_SLV_ADD_W_ACK			0x18UL
#define I2C_SYMB_SLV_ADD_W_NACK			0x20UL

#define I2C_SYMB_DATA_Tx_ACK			0x28UL
#define I2C_SYMB_DATA_Tx_NACK			0x30UL

#define I2C_SYMB_ARB_LOST				0x38UL

#define I2C_SYMB_SLV_ADD_R_ACK			0x40UL
#define I2C_SYMB_SLV_ADD_R_NACK			0x48UL

#define I2C_SYMB_DATA_Rx_ACK			0x50UL
#define I2C_SYMB_DATA_Rx_NACK			0x58UL

#define I2C_SLV_OWN_ADDRESS_W			0x60UL
#define I2C_SLV_ACK_RETURNED_W			0x68UL
#define I2C_SLV_OWN_ADDRESS_R			0xA8UL
#define I2C_SLV_ACK_RETURNED_R			0xB0UL
#define I2C_SLV_GNRL_CALL				0x70UL
#define I2C_SLV_GNRL_CALL_ARB_LST		0x78UL
#define I2C_SLV_ACK_DATA_RX				0x80UL
#define I2C_SLV_NACK_DATA_RX			0x88UL
#define I2C_SLV_ACK_GENERAL_CALL		0x98UL
#define I2C_SLV_NACK_GENERAL_CALL		0x98UL
#define I2C_SLV_STOP_REP_START			0xA0UL
#define I2C_SLV_ACK_BY_MASTER			0xB8UL
#define I2C_SLV_NACK_BY_MASTER			0xC0UL
#define I2C_SLV_LAST_DATA_ACK			0xC8UL
/*
 * TWCR Register Symbols
 */
#define I2C_TWCR_TWINT			7U
#define I2C_TWCR_TWEA			6U
#define I2C_TWCR_TWSTA			5U
#define I2C_TWCR_TWSTO			4U
#define I2C_TWCR_TWWC			3U
#define I2C_TWCR_TWEN			2U
#define I2C_TWCR_TWIE			0U

/*
 * TWAR Register Symbols
 */
#define I2C_TWAR_TWA6			7U
#define I2C_TWAR_TWA5			6U
#define I2C_TWAR_TWA4			5U
#define I2C_TWAR_TWA3			4U
#define I2C_TWAR_TWA2			3U
#define I2C_TWAR_TWA1			2U
#define I2C_TWAR_TWA0			1U
#define I2C_TWAR_TWGCE			0U

/*
 * TWSR Register Symbols
 */
#define I2C_TWSR_TWS7			7U
#define I2C_TWSR_TWS6			6U
#define I2C_TWSR_TWS5			5U
#define I2C_TWSR_TWS4			4U
#define I2C_TWSR_TWS3			3U
#define I2C_TWSR_TWPS1			1U
#define I2C_TWSR_TWPS0			0U


#define I2C_WRITE				0U
#define I2C_READ				1U
/*-*-*-*-*-*-*-*-*--*-*-*-*-*-*/
/*-*-*-*-*- MACROS -*-*-*-*-*-*/
#define I2C_CLR_TWINT			(I2C_TWCR_REG |= (1<<I2C_TWCR_TWINT))

#define I2C_ENABLE				(I2C_TWCR_REG |= (1<<I2C_TWCR_TWEN))
#define I2C_SET_START_BIT		(I2C_TWCR_REG |= (1<<I2C_TWCR_TWSTA))
#define I2C_SET_STOP_BIT		(I2C_TWCR_REG |= (1<<I2C_TWCR_TWSTO))
#define I2C_SET_ACK_BIT			(I2C_TWCR_REG |= (1<<I2C_TWCR_TWEA))

#define I2C_TWBR_VALUE()		(uint8_t)(((F_OSC/I2C_SCL_CLK)-16)/(2*PRESCALER_VALUE))
#define I2C_WAIT_TWINT()		((I2C_TWCR_REG & (1<<I2C_TWCR_TWINT)) == 0)
#define I2C_readSTATUS()		((I2C_TWSR_REG) & (0xF8))
#define I2C_START_ENABLE_FLAG	(I2C_TWCR_REG = (1<<I2C_TWCR_TWSTA)|(1<<I2C_TWCR_TWEN)|(1<<I2C_TWCR_TWINT))
#define I2C_STOP_ENABLE_FLAG	(I2C_TWCR_REG = (1<<I2C_TWCR_TWSTO)|(1<<I2C_TWCR_TWEN)|(1<<I2C_TWCR_TWINT))
#define I2C_ENABLE_FLAG			(I2C_TWCR_REG = (1<<I2C_TWCR_TWEN)|(1<<I2C_TWCR_TWINT))
#define I2C_ENABLE_FLAG_ACK		(I2C_TWCR_REG = (1<<I2C_TWCR_TWEN)|(1<<I2C_TWCR_TWINT)|(1<<I2C_TWCR_TWEA))


/*
 * Data Type for a pointer to notification call back function
 * Parameter (In) : None
 * Parameter (Out): None
 * Return Values : None
 */
typedef void (*pfI2C_CallBack_t)(void);

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- ENUMS -*-*-*-*-*-*/

typedef enum{
	I2C_STATUS_ERROR_NOK,
	I2C_STATUS_ERROR_OK,
	I2C_STATUS_INVALID_ARGUMENT,
	I2C_STATUS_INVALID_SLV_ADDR,
	I2C_STATUS_NULL_ARGUMENT,
	I2C_STATUS_NOT_INIT,
	I2C_STATUS_INIT,
	I2C_STATUS_ALREADY_INIT,
	I2C_STATUS_SLAVE_STOP,
	I2C_STATUS_SLAVE_ACK,
	I2C_STATUS_SLAVE_NACK,
	I2C_STATUS_SLAVE_READ,
	I2C_STATUS_SLAVE_WRITE,
	I2C_STATUS_SLAVE_GNRL_CALL,
	I2C_STATUS_SLAVE_DEAF
	
}enuI2C_Status_t;


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*-*-*-*-*- FUNCTIONS PROTOTYPES -*-*-*-*-*-*-*-*-*/
#if I2C_MODE == I2C_MODE_MASTER
/*- Initialize I2C Master Module -*/
enuI2C_Status_t I2C_MasterInit(void);

/*- Send Start Bit on the Bus -*/
enuI2C_Status_t I2C_MasterSendSTART(void);

/*- Send the Slave Address on the Bus -*/
enuI2C_Status_t I2C_MasterSendSlaveAddress(uint8_t salve_address, uint8_t R_W);

/*- Send Repeated Start Bit on the Bus -*/
enuI2C_Status_t I2C_MasterSendRepSTART(void);

/*- Send Stop Bit on the Bus -*/
enuI2C_Status_t I2C_MasterSendSTOP(void);

/*- Send Single Data Byte to Slave -*/
enuI2C_Status_t I2C_MasterSendByte(uint8_t u8_data);

/*- Receive Single Data Byte from Slave with ACK -*/
enuI2C_Status_t I2C_MasterReceiveByte_ACK(uint8_t *pu8_data);

/*- Receive Single Data Byte from Slave with NACK -*/
enuI2C_Status_t I2C_MasterReceiveByte_NACK(uint8_t *pu8_data);

/*- Send Multiple Data Bytes to Slave -*/
enuI2C_Status_t I2C_MasterSendPacket(uint8_t u8_slaveAddress, uint8_t * pu8_data, uint16_t u16_dataLen);

/*- Receive Multiple Data Bytes from Slave -*/
enuI2C_Status_t I2C_MasterReceivePacket(uint8_t u8_slaveAddress, uint8_t * pu8_data, uint16_t u16_dataLen);

/*- Receive Multiple Data Bytes with multiple Commands from Slave -*/
enuI2C_Status_t I2C_MasterReceiveGeneral(uint8_t u8_slaveAddress, uint8_t * pu8_source, uint16_t u16_sourceLen,\
										 uint8_t * pu8_destination, uint16_t u16_destinationLen);

/*- Send Multiple Data Bytes to a specific location to the Slave -*/
enuI2C_Status_t I2C_MasterSendToLocation(uint8_t u8_slaveAddress, uint8_t u8_location ,\
										uint8_t * pu8_data, uint16_t u16_dataLen);

/*- Receive Multiple Data Bytes from specific location from Slave -*/
enuI2C_Status_t I2C_MasterReceiveFromLocation(uint8_t u8_slaveAddress, uint8_t u8_location ,\
												uint8_t * pu8_data, uint16_t u16_dataLen);
#endif

#if I2C_MODE == I2C_MODE_SLAVE
/*- Initialize I2C Slave Module -*/
enuI2C_Status_t I2C_SlaveInit(uint8_t u8_OwnSlaveAddress);

/*- Check any action came to the Slave -*/
enuI2C_Status_t I2C_SlaveCheck(void);

/*- Send Single Data Byte to Master -*/
enuI2C_Status_t I2C_SlaveSendByte(uint8_t u8_data);

/*- Send Multiple Data Bytes to Master -*/
enuI2C_Status_t I2C_SlaveSend(uint8_t *pu8_data, uint16_t u16_dataLength);

/*- Receive Single Data Byte from Master -*/
enuI2C_Status_t I2C_SlaveReceiveByte(uint8_t *pu8_data);

/*- Receive Multiple Data Bytes from Master -*/
enuI2C_Status_t I2C_SlaveReceive(uint8_t *pu8_data);
#endif
/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/


#endif /* I2C_H_ */