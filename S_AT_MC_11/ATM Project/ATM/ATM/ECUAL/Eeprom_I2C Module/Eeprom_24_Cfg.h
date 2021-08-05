/*
 * Eeprom_24_Cfg.h
 *
 * Created: 7/22/2021 4:23:18 AM
 *  Author: Ahmed Nabil
 */ 


#ifndef EEPROM_24_CFG_H_
#define EEPROM_24_CFG_H_

#include "../../MCAL/I2C Module/I2C.h"

/* Model of the EEPROM Used */
#define AT24C16B		0
#define AT24C256B		1

#define EEPROM_24_MODEL					AT24C16B

#if EEPROM_24_MODEL == AT24C16B
	/* Number of Words in the Memory */
	#define EEPROM_24_WORDS_NUM				2048U

	/* Number of bits in a word */
	#define EEPROM_24_WORD_BITS				8U

	/* Slave Address of the Chip */
	#define EEPROM_24_SLV_ADDRESS			0x50U

	/* Number of Pages in the Memory */
	#define EEPROM_24_PAGES_NUM				128U

	/* Number fo Bytes in a Page */
	#define EEPROM_24_PAGE_BYTES			16U

	/* Mask of the Page Num in the Address */
	#define EEPROM_24_PAGE_MASK				0x7F0U

	/* Number of Bits for the Byte Mask in the Address */
	#define EEPROM_24_PAGE_SHIFT_BITS			4U

	/* Mask of the Byte offset in the Address */
	#define EEPROM_24_BYTE_MASK				0x00FU
	
	/* Number of Bytes used to address a location in the memory */
	#define EEPROM_24_LOC_SIZE				1U
/*******************************************************************************/
#elif EEPROM_24_MODEL == AT24C256B
	/* Number of Words in the Memory */
	#define EEPROM_24_WORDS_NUM				(uint32_t)32768U

	/* Number of bits in a word */
	#define EEPROM_24_WORD_BITS				8U

	/* Slave Address of the Chip */
	#define EEPROM_24_SLV_ADDRESS			0x50U

	/* Number of Pages in the Memory */
	#define EEPROM_24_PAGES_NUM				512U

	/* Number fo Bytes in a Page */
	#define EEPROM_24_PAGE_BYTES			64U

	/* Mask of the Page Num in the Address */
	#define EEPROM_24_PAGE_MASK				0x7FC0U

	/* Number of Bits for the Byte Mask in the Address */
	#define EEPROM_24_PAGE_SHIFT_BITS			6U

	/* Mask of the Byte offset in the Address */
	#define EEPROM_24_BYTE_MASK				0x03FU
	
	/* Number of Bytes used to address a location in the memory */
	#define EEPROM_24_LOC_SIZE				2U
#endif
#endif /* EEPROM_24_CFG_H_ */