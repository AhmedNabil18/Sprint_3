/*
 * I2C_Slave.c
 *
 * Created: 7/22/2021 3:09:12 AM
 * Author : Ahmed Nabil
 */ 

#include "MCAL/I2C Module/I2C.h"


int main(void)
{
	uint8_t u8_data[3]={0};
	DIO_PORTA_DIR = 0xFF;
	DIO_PORTB_DIR = 0xFF;
	DIO_PORTD_DIR = 0xFF;
	I2C_SlaveInit(0x10);
    /* Replace with your application code */
    while (1)
    {
	    switch(I2C_SlaveCheck())
	    {
			case I2C_STATUS_SLAVE_READ:	
				I2C_SlaveReceive(u8_data);
				break;
			case I2C_STATUS_SLAVE_WRITE:
				I2C_SlaveSend(u8_data,3);
				break;
			default:
				break;
		}
		DIO_PORTA_DATA = u8_data[0];
		DIO_PORTB_DATA = u8_data[1];
		DIO_PORTD_DATA = u8_data[2];
    }
}

