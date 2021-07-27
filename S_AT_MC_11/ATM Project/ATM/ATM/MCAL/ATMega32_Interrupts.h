/*
 * ATMega32_Interrupts.h
 *
 * Created: 7/15/2021 10:55:35 AM
 *  Author: Ahmed Nabil
 */ 


#ifndef ATMEGA32_INTERRUPTS_H_
#define ATMEGA32_INTERRUPTS_H_

#include "ATMega32_Registers.h"

#define INT0_IRQ				__vector_1
#define INT1_IRQ				__vector_2
#define INT2_IRQ				__vector_3
#define TIMER2_COMP_IRQ			__vector_4
#define TIMER2_OVF_IRQ			__vector_5
#define TIMER1_CAPT_IRQ			__vector_6
#define TIMER1_COMPA_IRQ		__vector_7
#define TIMER1_COMPB_IRQ		__vector_8
#define TIMER1_OVF_IRQ			__vector_9
#define TIMER0_COMP_IRQ			__vector_10
#define TIMER0_OVF_IRQ			__vector_11
#define SPI_STC_IRQ				__vector_12
#define USART_RXC_IRQ			__vector_13
#define USART_UDRE_IRQ			__vector_14
#define USART_TXC_IRQ			__vector_15
#define ADC_IRQ					__vector_16
#define EE_RDY_IRQ				__vector_17
#define ANA_COMP_IRQ			__vector_18
#define TWI_IRQ					__vector_19
#define SPM_RDY_IRQ				__vector_20


#  define ISR(vector, ...)            \
void vector (void) __attribute__ ((signal,used)) __VA_ARGS__; \
void vector (void)


#define EnableGlbl_Interrupt()		REG_SET_BIT(SREG_REG, SREG_I_BIT)
#define DisableGlbl_Interrupt()		REG_CLR_BIT(SREG_REG, SREG_I_BIT)

#endif /* ATMEGA32_INTERRUPTS_H_ */