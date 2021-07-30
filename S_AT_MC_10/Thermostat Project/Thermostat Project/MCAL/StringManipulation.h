/*******************************************************************************
 * File : StringManipulation.h
 * Header File for String Manipulation
 * Description: Various of Functions that serve and manipulate with strings.
 *******************************************************************************/

#ifndef _STRINGMANIPULATION_H_
#define _STRINGMANIPULATION_H_


/**------------------**
 **---- Includes ----**
 **------------------**/
#include "Platform_Types.h"



#define DEC     1U
#define BIN     2U
#define HEX     3U

/*********************************
 **---- Functions Prototype ----**
 *********************************/

/**
 ** FUNCTIONS PROTOTYPE
 **/
void stringCopy(uint8_t* source, uint8_t* destination);
uint8_t stringCompare(uint8_t* string1, uint8_t* string2);
uint8_t stringLength(uint8_t* string1);
uint8_t Max_String_Num(uint8_t* string1, uint8_t* string2);
void EmptyString(uint8_t *string);
void stringHexToNum(uint8_t *pu8_String, uint16_t *pu16_Num);
void stringToInteger(uint32_t *pu32_Num, uint8_t *pu8_String);
void integerToString(uint16_t u16_Num, uint8_t *pu8_String, uint8_t u8_base);
void stringConcatenate(uint8_t* destination, uint8_t* source);
float32_t stringToFloat(uint8_t* string);
void floatToString(float32_t f32_num, uint8_t* string);
#endif // _STRINGMANIPULATION_H_




