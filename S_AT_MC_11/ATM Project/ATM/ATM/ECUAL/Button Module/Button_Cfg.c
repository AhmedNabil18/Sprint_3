/*
 * Button_Cfg.c
 *
 * Created: 7/16/2021 9:08:21 PM
 *  Author: Ahmed Nabil
 */ 
#include "Button.h"

/* Array holds the used Buttons' DIO Channels */
uint8_t gau8_buttonsUsed[BUTTONS_USED_NUM] =
{
	DIO_BTN1_CHANNEL_ID
};