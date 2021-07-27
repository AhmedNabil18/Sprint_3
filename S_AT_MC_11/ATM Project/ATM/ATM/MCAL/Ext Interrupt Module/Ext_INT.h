/*
 * Ext_INT.h
 *
 * Created: 7/27/2021 3:03:04 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef EXT_INT_H_
#define EXT_INT_H_

#include "../ATMega32_Registers.h"
#include "../ATMega32_Interrupts.h"

#define EXT_INT0_LVL_LOW				0U
#define EXT_INT0_EDGE_FALL_RISE			1U
#define EXT_INT0_EDGE_FALL				2U
#define EXT_INT0_EDGE_RISE				3U

#define EXT_INT1_LVL_LOW				0U
#define EXT_INT1_EDGE_FALL_RISE			1U
#define EXT_INT1_EDGE_FALL				2U
#define EXT_INT1_EDGE_RISE				3U

#define EXT_INT2_EDGE_FALL				0U
#define EXT_INT2_EDGE_RISE				1U

typedef struct
{
	uint8_t int_pin;
	uint8_t trigg_state;
}EXT_intCfg;

void INT0_setCallBack(void (*ptrFun)(void));
void INT1_setCallBack(void (*ptrFun)(void));
void INT2_setCallBack(void (*ptrFun)(void));

void Ext_INT0_init(uint8_t u8_mode);
void Ext_INT1_init(uint8_t u8_mode);
void Ext_INT2_init(uint8_t u8_mode);


#endif /* EXT_INT_H_ */