/*******************************************************************************
 * File : StringManipulation.c
 * Source File for String Manipulation
 * Description: Various of Functions that serve and manipulate with strings.
 *******************************************************************************/


/**------------------**
 **---- Includes ----**
 **------------------**/
#include "StringManipulation.h"


/**************************************
 **---- Functions Implementation ----**
 **************************************/

/*************************************************************************************
* Service Name: stringLength
* Parameters (in): pau32_array -> Pointer to array of data to search in.
*                  u8_arraySize -> Size of the array pau32_array.
*                  u32_number -> The number to be searched.
* Parameters (inout): None
* Parameters (out):None
* Return value: uint8_t -> unsigned character returns the index of the u32_number.
*                          (-1) returned if the u32_number not found.
* Description: Function to search for u32_number inside the pau32_array and return its index
*              if found, otherwise returns (-1).
* *************************************************************************************/

static uint32_t power(uint8_t base, uint8_t power_val)
{
	uint8_t i;
	uint32_t result=1;
	if(power_val == 0) return 1;
	if(base == 0) return 0;
	for (i=0;i<power_val;i++)
	{
		result *= base;
	}
	return result;
}
uint8_t stringLength(uint8_t* string1)
{
    uint8_t u8_loopIndex=0;
    while(string1[u8_loopIndex] != '\0')
    {
        u8_loopIndex++;
    }
    return u8_loopIndex+1;
}

uint8_t stringCompare(uint8_t* string1, uint8_t* string2)
{
    uint8_t u8_loopIndex=0;
    if(stringLength(string1) > stringLength(string2))
        return 2;
    if(stringLength(string1) < stringLength(string2))
        return 3;
    while(string1[u8_loopIndex] != '\0')
    {
        if(string1[u8_loopIndex] != string2[u8_loopIndex])
            return 0;
        u8_loopIndex++;
    }
    return 1;
}


uint8_t Max_String_Num(uint8_t* string1, uint8_t* string2)
{
    uint8_t u8_retValue = stringCompare(string1,string2);

    switch(u8_retValue)
    {
    case 1:
        /* Two Numbers are equal */
        return 0;
    case 2:
        /* string 1 > string 2 */
        return 1;
    case 3:
        /* string 2 > string 1 */
        return 2;
    case 0:
        break;
    default:
        break;
    }
    uint8_t u8_loopIndex = 0;

    while(string1[u8_loopIndex] != '\0')
    {
        if(string1[u8_loopIndex] > string2[u8_loopIndex])
            return 1;
        else if(string1[u8_loopIndex] < string2[u8_loopIndex])
            return 2;
        u8_loopIndex++;
    }
    return 3;
}

void EmptyString(uint8_t *string)
{
	uint8_t u8_loopIndex=0;
	while(string[u8_loopIndex] != '\0')
		string[u8_loopIndex++] = '\0';
}

void stringCopy(uint8_t* source, uint8_t* destination)
{
    uint8_t u8_loopIndex=0;
    while(source[u8_loopIndex] != '\0')
    {
	    destination[u8_loopIndex] = source[u8_loopIndex];
	    u8_loopIndex++;
    }
}
void stringHexToNum(uint8_t *pu8_String, uint16_t *pu16_Num)
{
	sint8_t s8_loopIndex=0;
	uint8_t u8_powerValue=0;
	uint8_t temp_val=0;
	uint8_t u8_stringLength = stringLength(pu8_String)-1;
	*pu16_Num = 0;
	for(s8_loopIndex=u8_stringLength-1 ; s8_loopIndex>=0 ; s8_loopIndex--)
	{
		if ((pu8_String[s8_loopIndex]>='0') && (pu8_String[s8_loopIndex]<='9'))
		{
			temp_val = pu8_String[s8_loopIndex] - '0';
			*pu16_Num += temp_val*power(16,u8_powerValue++);
		}else if((pu8_String[s8_loopIndex]>='A') && (pu8_String[s8_loopIndex]<='F'))
		{
			temp_val = pu8_String[s8_loopIndex] - 55;
			*pu16_Num += temp_val*power(16,u8_powerValue++);
		}else if((pu8_String[s8_loopIndex]>='a') && (pu8_String[s8_loopIndex]<='f'))
		{
			temp_val = pu8_String[s8_loopIndex] - 87;
			*pu16_Num += temp_val*power(16,u8_powerValue++);
		}

	}
}

/*
uint16_t arrayToInteger(uint8_t* pu8_Array)
{
    uint8_t u8_arrLength = stringLength(pu8_Array);
    uint8_t u8_loopIndex=0;
    for(u8_loopIndex=0; u8_loopIndex<u8_arrLength; u8_loopIndex++)
    {

    }
}*/
