/*
 * Keypad_Cfg.h
 *
 * Created: 7/23/2021 10:44:54 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef KEYPAD_CFG_H_
#define KEYPAD_CFG_H_

#include "..\..\MCAL\Dio Module\Dio.h"

#define KEYPAD_ROWS_NUM				4U
#define KEYPAD_COLUMNS_NUM			3U

#define KEYPAD_COL_1				DIO_COL_1_CHANNEL_ID
#define KEYPAD_COL_2				DIO_COL_2_CHANNEL_ID
#define KEYPAD_COL_3				DIO_COL_3_CHANNEL_ID
#define KEYPAD_ROW_A				DIO_ROW_A_CHANNEL_ID
#define KEYPAD_ROW_B				DIO_ROW_B_CHANNEL_ID
#define KEYPAD_ROW_C				DIO_ROW_C_CHANNEL_ID
#define KEYPAD_ROW_D				DIO_ROW_D_CHANNEL_ID



#endif /* KEYPAD_CFG_H_ */