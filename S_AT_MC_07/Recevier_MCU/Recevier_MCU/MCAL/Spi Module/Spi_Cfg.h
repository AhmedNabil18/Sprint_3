/*
 * Spi_Cfg.h
 *
 * Created: 7/21/2021 1:44:43 AM
 *  Author: Ahmed Nabil
 */ 


#ifndef SPI_CFG_H_
#define SPI_CFG_H_

/************************** CONFIGURATIONS ****************************/
/*- SPI Mode Master/Slave -*/
/* :Options:
 * SPI_MASTER
 * SPI_SLAVE
 */
#define SPI_ROLE		SPI_SLAVE

/*- SPI LSB First On/Off -*/
/* :Options:
 * SPI_LSB_OFF
 * SPI_LSB_ON
 */
#define	SPI_LSB_FIRST	SPI_LSB_OFF

/*- SPI Polarity and Phase Mode -*/
/* :Options:
 * SPI_MODE0
 * SPI_MODE1
 * SPI_MODE2
 * SPI_MODE3
 */
#define SPI_MODE		SPI_MODE0

/*- SPI Frequency Divider -*/
/* :Options:
 * SPI_FOSC_DIV4
 * SPI_FOSC_DIV16
 * SPI_FOSC_DIV64
 * SPI_FOSC_DIV128
 * SPI_FOSC_DIV2
 * SPI_FOSC_DIV8
 * SPI_FOSC_DIV32
 */
#define SPI_FOSC_DIV	SPI_FOSC_DIV2

/*- SPI Slave Select Initial -*/
/* :Options:
 * SPI_CS_ACTIVE_LOW
 * SPI_CS_ACTIVE_HIGH
 */
#define SPI_SLAVE_CS	SPI_CS_ACTIVE_LOW



#endif /* SPI_CFG_H_ */