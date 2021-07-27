/*
 * Spi.h
 *
 * Created: 7/21/2021 1:44:33 AM
 *  Author: Ahmed Nabil
 */ 


#ifndef SPI_H_
#define SPI_H_

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Spi_Cfg.h"
#include "..\Platform_Types.h"
#include "Spi_Symbols.h"
#include "..\ATMega32_Interrupts.h"


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- CONSTANTS -*-*-*-*-*-*/

#define SPI_MASTER						1U/*******************************************/
#define SPI_SLAVE						0U/*******************************************/
/*******************************************/
#define SPI_LSB_ON						1U/*******************************************/
#define SPI_LSB_OFF						0U/*******************************************/
#define SPI_CS_ACTIVE_HIGH				1U/*******************************************/
#define SPI_CS_ACTIVE_LOW				0U/*******************************************/
/*******************************************/
#define SPI_MODE0						0U/*******************************************/
#define SPI_MODE1						1U/*******************************************/
#define SPI_MODE2						2U/*******************************************/
#define SPI_MODE3						3U/*******************************************/
/*******************************************/
#define SPI_FOSC_DIV4					0x00U/****************************************/
#define SPI_FOSC_DIV16					0x01U/****************************************/
#define SPI_FOSC_DIV64					0x02U/****************************************/
#define SPI_FOSC_DIV128					0x03U/****************************************/
#define SPI_FOSC_DIV2					0x04U/****************************************/
#define SPI_FOSC_DIV8					0x05U/****************************************/
#define SPI_FOSC_DIV32					0x06U/****************************************/
/*********************************************************************/
/*********************************************************************/
#define SPI_MOSI_PIN						5U
#define SPI_MISO_PIN						6U
#define SPI_SS_PIN							4U
#define SPI_SCK_PIN							7U

#define SPI_DUMMY_DATA						0x00
/*-*-*-*-*-*-*-*-*--*-*-*-*-*-*/
/*-*-*-*-*- MACROS -*-*-*-*-*-*/
#if SPI_SLAVE_CS == SPI_CS_ACTIVE_HIGH
#define SPI_SS_DISABLE()	DIO_PORTB_DATA &= ~(1<<SPI_SS_PIN)			/* Define Slave enable */
#define SPI_SS_ENABLE()		DIO_PORTB_DATA |= (1<<SPI_SS_PIN)			/* Define Slave disable */
#else
#define SPI_SS_ENABLE()		DIO_PORTB_DATA &= ~(1<<SPI_SS_PIN)			/* Define Slave enable */
#define SPI_SS_DISABLE()	DIO_PORTB_DATA |= (1<<SPI_SS_PIN)			/* Define Slave disable */
#endif
/*
 * Data Type for a pointer to notification call back function
 * Parameter (In) : None
 * Parameter (Out): None
 * Return Values : None
 */
typedef void (*pfSpi_CallBack_t)(void);

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- ENUMS -*-*-*-*-*-*/

typedef enum{
	SPI_STATUS_ERROR_NOK,
	SPI_STATUS_ERROR_OK,
	SPI_STATUS_PIN_ID_INVALID,
	SPI_STATUS_PIN_NUM_INVALID,
	SPI_STATUS_PORT_NUM_INVALID,
	SPI_STATUS_NULL_ARGUMENT,
	SPI_STATUS_NOT_INIT,
	SPI_STATUS_INIT,
	SPI_STATUS_ALREADY_INIT
}enuSpi_Status_t;


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- STRUCTS -*-*-*-*-*-*/

typedef struct
{
	uint8_t u8_Enable;
	uint8_t u8_DataFrameSize;
	uint8_t u8_DoubleSpeedEnable;
	uint8_t u8_ParityMode;
	uint8_t u8_StopBits;
}strSpi_Config_t;


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*-*-*-*-*- FUNCTIONS PROTOTYPES -*-*-*-*-*-*-*-*-*/

/*- Initialize SPI Module -*/
enuSpi_Status_t Spi_init(void);

#if SPI_ROLE == SPI_MASTER
/*- Send Single Data Byte -*/
enuSpi_Status_t Spi_MasterSendByte(uint8_t u8_Data);

/*- Send Multiple Data Bytes -*/
enuSpi_Status_t Spi_MasterSendPacket(uint8_t *pu8_Data, uint16_t u16_packetSize);

/*- Receive Single Data Byte -*/
enuSpi_Status_t Spi_MasterReceiveByte(uint8_t *pu8_Data);

/*- Receive Multiple Bytes -*/
enuSpi_Status_t Spi_MasterReceivePacket(uint8_t *pu8_Data, uint16_t u16_packetSize);
#endif

#if SPI_ROLE == SPI_SLAVE
/*- Send Single Data Byte -*/
enuSpi_Status_t Spi_SlaveSendByte(uint8_t u8_Data);

/*- Send Multiple Data Bytes -*/
enuSpi_Status_t Spi_SlaveSendPacket(uint8_t *pu8_Data, uint16_t u16_packetSize);

/*- Receive Single Data Byte -*/
enuSpi_Status_t Spi_SlaveReceiveByte(uint8_t *pu8_Data);

/*- Receive Multiple Bytes -*/
enuSpi_Status_t Spi_SlaveReceivePacket(uint8_t *pu8_Data, uint16_t u16_packetSize);
#endif

/*- Enable Interrupt Notification for SPIE by calling Callback function -*/
enuSpi_Status_t Spi_EnableNotification(pfSpi_CallBack_t Spi_Callback);

/*- Disable Interrupt Notification -*/
enuSpi_Status_t Spi_DisableNotification(void);

/*- Read the data register directly -*/
uint8_t Spi_DataRegister(void);
/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

#endif /* SPI_H_ */