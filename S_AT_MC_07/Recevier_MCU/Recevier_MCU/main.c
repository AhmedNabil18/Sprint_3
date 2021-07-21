/*
 * SPI Slave.c
 *
 * Created: 7/21/2021 2:55:16 PM
 * Author : Ahmed Nabil
 */ 
#include "MCAL/Spi Module/Spi.h"

int main(void)
{
	Spi_init();
	uint8_t data=0;
	uint32_t i=0;
	DIO_PORTA_DIR = 0xFF;
	while (1)
	{
		/* Receive Data from master */
		Spi_SlaveReceiveByte(&data);
		/* Output the received data on PORT A */
		DIO_PORTA_DATA = data++;
	}
}
