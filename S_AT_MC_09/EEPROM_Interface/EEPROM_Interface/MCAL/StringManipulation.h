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
#include <stdlib.h>
#include "../../MCAL/Platform_Types.h"



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
#endif // _STRINGMANIPULATION_H_




