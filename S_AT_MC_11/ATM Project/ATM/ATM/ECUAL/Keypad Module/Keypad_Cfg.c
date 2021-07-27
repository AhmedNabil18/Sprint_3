/*
 * Keypad_Cfg.c
 *
 * Created: 7/23/2021 11:21:40 PM
 *  Author: Ahmed Nabil
 */ 
#include "Keypad.h"

uint8_t gau8_Columns[KEYPAD_COLUMNS_NUM] = {KEYPAD_COL_1, KEYPAD_COL_2, KEYPAD_COL_3};
uint8_t gau8_Rows[KEYPAD_ROWS_NUM] = {KEYPAD_ROW_A, KEYPAD_ROW_B, KEYPAD_ROW_C, KEYPAD_ROW_D};
	
uint8_t gau8_ButtonSymbols[KEYPAD_ROWS_NUM][KEYPAD_COLUMNS_NUM] =
{
	{'1','2','3'},
	{'4','5','6'},
	{'7','8','9'},
	{'*','0','#'}
} ;
