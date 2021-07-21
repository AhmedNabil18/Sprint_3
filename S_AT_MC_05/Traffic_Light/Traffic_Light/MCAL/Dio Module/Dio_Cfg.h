/*
 * Dio_Cfg.h
 *
 * Created: 7/13/2021 7:54:56 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef DIO_CFG_H_
#define DIO_CFG_H_

#include "..\ATMega32_Registers.h"

#define DIO_USED_PINS_NUM		3

/* DIO Configured Port ID's  */
#define DioConf_LED1_PORT_NUM               PORT_B 
#define DioConf_LED2_PORT_NUM				PORT_B
#define DioConf_LED3_PORT_NUM				PORT_B

/* DIO Configured Channel ID's */
#define DioConf_LED1_PIN_NUM				 PIN_0 /* Pin 0 in PORTB */
#define DioConf_LED2_PIN_NUM				 PIN_1 /* Pin 1 in PORTB */
#define DioConf_LED3_PIN_NUM				 PIN_2 /* Pin 2 in PORTB */

/* Channel Index in the array of structures in Dio_Cfg.c */
#define DIO_LED1_CHANNEL_ID				0
#define DIO_LED2_CHANNEL_ID				1
#define DIO_LED3_CHANNEL_ID				2

#endif /* DIO_CFG_H_ */