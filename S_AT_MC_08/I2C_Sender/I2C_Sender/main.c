/*
 * I2C_Sender.c
 *
 * Created: 7/22/2021 2:53:16 AM
 * Author : Ahmed Nabil
 */ 

#include "MCAL/I2C Module/I2C.h"


int main(void)
{
	/* Receiving Multiple Bytes from the Slave*/
// 	uint8_t u8_data[4]={0};
// 	I2C_MasterInit();
// 	I2C_MasterReceivePacket(0x10,u8_data,3);
// 	DIO_PORTA_DIR = 0xFF;
// 	DIO_PORTB_DIR = 0xFF;
// 	DIO_PORTD_DIR = 0xFF;
// 	DIO_PORTA_DATA = u8_data[0];
// 	DIO_PORTB_DATA = u8_data[1];
// 	DIO_PORTD_DATA = u8_data[2];

	/* Receiving Single Byte from the slave */
// 	uint8_t u8_data=0;
// 	I2C_MasterInit();
// 	I2C_MasterReceivePacket(0x10,&u8_data,1);
// 	DIO_PORTA_DIR = 0xFF;
// 	DIO_PORTA_DATA = u8_data;
	
	/* Sending String to the slave */
	uint8_t u8_data[4] = {255,7,6,4};
	I2C_MasterInit();
	I2C_MasterSendPacket(0x10,u8_data,3);
	
	/* Sending Character to the slave */
// 	uint8_t u8_data = 3;
// 	I2C_MasterInit();
// 	I2C_MasterSendPacket(0x10,&u8_data,1);
	while (1)
	{
	}
}

