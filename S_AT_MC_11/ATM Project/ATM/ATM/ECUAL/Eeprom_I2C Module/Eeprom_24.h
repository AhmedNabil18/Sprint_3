/*
 * Eeprom_24.h
 *
 * Created: 7/22/2021 4:22:57 AM
 *  Author: Ahmed Nabil
 */ 


#ifndef Eeprom_24_H_
#define Eeprom_24_H_

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Eeprom_24_Cfg.h"

#define SINGLE_BYTE			1U

/*******************************************************************************
 *                          Module Data Types                                  *
 *******************************************************************************/
/*
 * Data Type for Eeprom_24 return status
 */
typedef enum
{
	EEPROM_24_STATUS_ERROR_NOK,
	EEPROM_24_STATUS_ERROR_OK,
	EEPROM_24_STATUS_NOT_INIT,
	EEPROM_24_STATUS_INIT,
	EEPROM_24_STATUS_ERROR_NULL,
	EEPROM_24_STATUS_INPUT_CHANGED
}enuEeprom_24_Status_t;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
/*- Initialize the EEPROM Module -*/
enuEeprom_24_Status_t Eeprom_24_init(void);

/*- Write Byte to specific location in the Memory -*/
enuEeprom_24_Status_t  Eeprom_24_writeByte(uint16_t u16_location, uint8_t u8_data);

/*- Read Byte from specific location in the Memory -*/
enuEeprom_24_Status_t  Eeprom_24_readByte(uint16_t u16_location, uint8_t* pu8_data);

/*- Write Page to specific location in the Memory -*/
enuEeprom_24_Status_t  Eeprom_24_writePage(uint8_t u8_page, uint8_t *pu8_data);

/*- Read Page from specific location in the Memory -*/
enuEeprom_24_Status_t  Eeprom_24_readPage(uint8_t u8_page, uint8_t *pu8_data);

/*- Write Multiple Bytes to specific location in the Memory -*/
enuEeprom_24_Status_t  Eeprom_24_writePacket(uint16_t u16_location, uint8_t *pu8_data, uint16_t u16_dataLen);

/*- Read Multiple Bytes from specific location in the Memory -*/
enuEeprom_24_Status_t  Eeprom_24_readPacket(uint16_t u16_location, uint8_t *pu8_data, uint16_t u16_dataLen);

#endif /* Eeprom_24_H_ */