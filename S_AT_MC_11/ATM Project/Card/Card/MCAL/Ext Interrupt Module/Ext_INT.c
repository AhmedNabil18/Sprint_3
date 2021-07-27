/*
 * Ext_INT.c
 *
 * Created: 7/27/2021 3:02:47 PM
 *  Author: Ahmed Nabil
 */ 
#include "Ext_INT.h"

void Ext_INT0_init(uint8_t u8_mode)
{
	GICR_REG |= 1<<(GICR_INT0_BIT);
	MCUCR_REG |= u8_mode<<MCUCR_ISC00;
}

void Ext_INT1_init(uint8_t u8_mode)
{
	GICR_REG |= 1<<(GICR_INT1_BIT);
	MCUCR_REG |= u8_mode<<MCUCR_ISC10;
}
void Ext_INT2_init(uint8_t u8_mode)
{//ToDo
	GICR_REG |= 1<<(GICR_INT1_BIT);
	//MCUCSR_REG |= u8_mode<<MCUCR_ISC10;
}


static void (*INT0_CallBackPtr)(void);

void INT0_setCallBack(void (*ptrFun)(void))
{
	INT0_CallBackPtr=ptrFun;
}
ISR(INT0_IRQ)
{
	(*INT0_CallBackPtr)();
}

/*********************************************/
static void (*INT1_CallBackPtr)(void);

void INT1_setCallBack(void (*ptrFun)(void))
{
	INT1_CallBackPtr=ptrFun;
}
ISR(INT1_IRQ)
{
	(*INT1_CallBackPtr)();
}

/*********************************************/
static void (*INT2_CallBackPtr)(void);

void INT2_setCallBack(void (*ptrFun)(void))
{
	INT2_CallBackPtr=ptrFun;
}
ISR(INT2_IRQ)
{
	(*INT2_CallBackPtr)();
}
