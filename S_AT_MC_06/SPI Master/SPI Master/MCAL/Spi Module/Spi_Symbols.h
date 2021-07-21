 /******************************************************************************
 *
 * Module: Spi
 *
 * File Name: Spi_Symbols.h
 *
 * Description: Header file for Spi Module Symbols.
 *
 * Author: Ahmed Nabil
 ******************************************************************************/


#ifndef SPI_SYMBOLS_H_
#define SPI_SYMBOLS_H_

/*
 * SPSR Register Symbols
 */
#define SPI_SPSR_SPIF			7U
#define SPI_SPSR_WCOL			6U
#define SPI_SPSR_SPI2X			0U

/*
 * SPCR Register Symbols
 */
#define SPI_SPCR_SPIE			7U
#define SPI_SPCR_SPE			6U
#define SPI_SPCR_DORD			5U
#define SPI_SPCR_MSTR			4U
#define SPI_SPCR_CPOL			3U
#define SPI_SPCR_CPHA			2U
#define SPI_SPCR_SPR1			1U
#define SPI_SPCR_SPR0			0U

#endif /* SPI_SYMBOLS_H_ */