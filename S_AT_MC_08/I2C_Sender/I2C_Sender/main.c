/*
 * I2C_Sender.c
 *
 * Created: 7/22/2021 2:53:16 AM
 * Author : Ahmed Nabil
 */ 

#include "MCAL/I2C Module/I2C.h"


int main(void)
{
	
	/* Sending String to the slave */
	uint8_t u8_data[3] = {1,2,3};
	I2C_MasterInit();
	I2C_MasterSendPacket(0x10,u8_data,3);
	
	while (1)
	{
	}
}

