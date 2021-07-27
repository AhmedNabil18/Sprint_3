/*
 * Eeprom_24_Cfg.h
 *
 * Created: 7/22/2021 4:23:18 AM
 *  Author: Ahmed Nabil
 */ 


#ifndef EEPROM_24_CFG_H_
#define EEPROM_24_CFG_H_

#include "../../MCAL/I2C Module/I2C.h"

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

#endif /* EEPROM_24_CFG_H_ */