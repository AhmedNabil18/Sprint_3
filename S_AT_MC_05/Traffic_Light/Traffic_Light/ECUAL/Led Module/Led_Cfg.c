/*
 * Led_Cfg.c
 *
 * Created: 7/20/2021 7:01:14 PM
 *  Author: Ahmed Nabil
 */ 
#include "Led.h"

/* Array holds the used Leds' DIO Channels */
uint8_t gau8_ledsUsed[LEDS_USED_NUM] =
{
	DIO_LED1_CHANNEL_ID,
	DIO_LED2_CHANNEL_ID,
	DIO_LED3_CHANNEL_ID
};