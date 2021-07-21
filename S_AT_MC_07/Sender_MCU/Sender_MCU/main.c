/*
 * SPI Master.c
 *
 * Created: 7/21/2021 1:58:27 PM
 * Author : Ahmed Nabil
 */ 

#include "MCAL/Spi Module/Spi.h"

int main(void)
{
    Spi_init();
	uint8_t ch=0x01;
	uint32_t i=0;
	DIO_PORTA_DIR = 0xFF;
    while (1) 
    {
	    DIO_PORTA_DATA = ch;
		SPI_SS_ENABLE();
		Spi_MasterSendByte(ch++);
		SPI_SS_DISABLE();
		for(i=0;i<40000;i++);
    }
}