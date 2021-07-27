 /******************************************************************************
 *
 * Module: Gpt
 *
 * File Name: Gpt_Symbols.h
 *
 * Description: Header file for Gpt Module Symbols.
 *
 * Author: Ahmed Nabil
 ******************************************************************************/


#ifndef GPT_SYMBOLS_H_
#define GPT_SYMBOLS_H_

/* GPT TIMER IDs */
#define GPT_TIMER_0		(0U)
#define GPT_TIMER_1		(1U)
#define GPT_TIMER_2		(2U)

/*
 * TIMSK Register Symbols
 */
#define GPT_TIMSK_OCIE2		7U
#define GPT_TIMSK_TOIE2		6U
#define GPT_TIMSK_TICIE1	5U
#define GPT_TIMSK_OCIE1A	4U
#define GPT_TIMSK_OCIE1B	3U
#define GPT_TIMSK_TOIE1		2U
#define GPT_TIMSK_OCIE0		1U
#define GPT_TIMSK_TOIE0		0U


/*
 * TIFR Register Symbols
 */
#define GPT_TIFR_OCF2		7U
#define GPT_TIFR_TOV2		6U
#define GPT_TIFR_ICF1		5U
#define GPT_TIFR_OCF1A		4U
#define GPT_TIFR_OCF1B		3U
#define GPT_TIFR_TOV1		2U
#define GPT_TIFR_OCF0		1U
#define GPT_TIFR_TOV0		0U

/*
 * TCCR2 Register Symbols
 */
#define GPT_TCCR2_FOC2		7U
#define GPT_TCCR2_WGM20		6U
#define GPT_TCCR2_COM21		5U
#define GPT_TCCR2_COM20		4U
#define GPT_TCCR2_WGM21		3U
#define GPT_TCCR2_CS22		2U
#define GPT_TCCR2_CS21		1U
#define GPT_TCCR2_CS20		0U


/*
 * TCCR1A Register Symbols
 */
#define GPT_TCCR1A_COM1A1		7U
#define GPT_TCCR1A_COM1A0		6U
#define GPT_TCCR1A_COM1B1		5U
#define GPT_TCCR1A_COM1B0		4U
#define GPT_TCCR1A_FOC1A		3U
#define GPT_TCCR1A_FOC1B		2U
#define GPT_TCCR1A_WGM11		1U
#define GPT_TCCR1A_WGM10		0U

/*
 * TCCR1B Register Symbols
 */
#define GPT_TCCR1B_ICNC1		7U
#define GPT_TCCR1B_ICES1		6U
#define GPT_TCCR1B_RSRVD		5U
#define GPT_TCCR1B_WGM13		4U
#define GPT_TCCR1B_WGM12		3U
#define GPT_TCCR1B_CS12			2U
#define GPT_TCCR1B_CS11			1U
#define GPT_TCCR1B_CS10			0U

/*
 * TCCR0 Register Symbols
 */
#define GPT_TCCR0_FOC0		7U
#define GPT_TCCR0_WGM00		6U
#define GPT_TCCR0_COM01		5U
#define GPT_TCCR0_COM00		4U
#define GPT_TCCR0_WGM01		3U
#define GPT_TCCR0_CS02		2U
#define GPT_TCCR0_CS01		1U
#define GPT_TCCR0_CS00		0U

#endif



















