/*
 * Lcd_Cfg.h
 *
 * Created: 7/24/2021 10:38:14 AM
 *  Author: Ahmed Nabil
 */ 


#ifndef LCD_CFG_H_
#define LCD_CFG_H_

#include "..\..\MCAL\Dio Module\Dio.h"

#define LCD_ROWS_NUM				2U
#define LCD_COLUMNS_NUM				16U
#define LCD_MODE					LCD_MODE_4_BIT

#define LCD_RS						DIO_LCD_RS_CHANNEL_ID
#define LCD_RW						DIO_LCD_RW_CHANNEL_ID
#define LCD_E						DIO_LCD_E_CHANNEL_ID
#define LCD_D4						DIO_LCD_D4_CHANNEL_ID
#define LCD_D5						DIO_LCD_D5_CHANNEL_ID
#define LCD_D6						DIO_LCD_D6_CHANNEL_ID
#define LCD_D7						DIO_LCD_D7_CHANNEL_ID

/*Entry Mode*/
#define INC_DEC					  1
#define SHIFT					  0

/*Display ON/OFF*/
#define DISPLAY_ON					1
#define CURSOR_ON					1
#define BLINKING_ON					0

#endif /* LCD_CFG_H_ */