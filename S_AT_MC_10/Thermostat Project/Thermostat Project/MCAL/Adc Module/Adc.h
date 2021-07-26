/*
 * Adc.h
 *
 * Created: 7/24/2021 11:25:14 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef ADC_H_
#define ADC_H_

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Adc_Cfg.h"
#include "..\Platform_Types.h"
#include "..\ATMega32_Interrupts.h"


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- CONSTANTS -*-*-*-*-*-*/
#define ADC_PORT				DIO_PORTA_DATA
			
#define ADC0					0U
#define ADC1					1U
#define ADC2					2U
#define ADC3					3U
#define ADC4					4U
#define ADC5					5U
#define ADC6					6U
#define ADC7					7U

#define ADC_ADMUX_MUX0			0U
#define ADC_ADMUX_MUX1			1U
#define ADC_ADMUX_MUX2			2U
#define ADC_ADMUX_MUX3			3U
#define ADC_ADMUX_MUX4			4U
#define ADC_ADMUX_ADLAR			5U
#define ADC_ADMUX_REFS0			6U
#define ADC_ADMUX_REFS1			7U

#define ADC_ADCSRA_ADPS0			0U
#define ADC_ADCSRA_ADPS1			1U
#define ADC_ADCSRA_ADPS2			2U
#define ADC_ADCSRA_ADIE				3U
#define ADC_ADCSRA_ADIF				4U
#define ADC_ADCSRA_ADATE			5U
#define ADC_ADCSRA_ADSC				6U
#define ADC_ADCSRA_ADEN				7U

#define ADC_STEPS_NUM			1024

#define ADC_VREF_AREF			0U
#define ADC_VREF_VCC			1U
#define ADC_VREF_2p56V			3U

#define ADC_PRESCALER_2			0U
#define ADC_PRESCALER_4			2U
#define ADC_PRESCALER_8			3U
#define ADC_PRESCALER_16		4U
#define ADC_PRESCALER_32		5U
#define ADC_PRESCALER_64		6U
#define ADC_PRESCALER_128		7U

#define ADC_INTERRUPT_ON		1U
#define ADC_INTERRUPT_OFF		0U

#define ADC_AUTO_TRIGGER_ON		1U
#define ADC_AUTO_TRIGGER_OFF	0U

/*-*-*-*-*-*-*-*-*--*-*-*-*-*-*/
/*-*-*-*-*- MACROS -*-*-*-*-*-*/

/*
 * Data Type for a pointer to notification call back function
 * Parameter (In) : None
 * Parameter (Out): None
 * Return Values : None
 */
typedef void (*pfAdc_CallBack_t)(void);

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- ENUMS -*-*-*-*-*-*/

typedef enum{
	ADC_STATUS_ERROR_NOK,
	ADC_STATUS_ERROR_OK,
	ADC_STATUS_PIN_ID_INVALID,
	ADC_STATUS_PIN_NUM_INVALID,
	ADC_STATUS_PORT_NUM_INVALID,
	ADC_STATUS_NULL_ARGUMENT,
	ADC_STATUS_NOT_INIT,
	ADC_STATUS_INIT,
	ADC_STATUS_ALREADY_INIT
}enuAdc_Status_t;


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- STRUCTS -*-*-*-*-*-*/

typedef struct
{
	uint8_t u8_pinID;
	uint8_t u8_Vref;
}strAdc_Config_t;


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*-*-*-*-*- FUNCTIONS PROTOTYPES -*-*-*-*-*-*-*-*-*/

/*- Initialize Adc Module -*/
enuAdc_Status_t Adc_init(void);

/* Function to start the ADC Conversion and Poll for Data */
enuAdc_Status_t Adc_start(uint8_t u8_channelID, uint32_t* pu32_data); // Start conversion and read data

/* Function to start the ADC Conversion and enables the interrupt */
enuAdc_Status_t ADC_startWithInterrupt(uint8_t u8_channelID, pfAdc_CallBack_t pf_callback);

/* Function to Read Data from ISR Functions == CALLBACK functions */
enuAdc_Status_t ADC_readFromISR(uint8_t u8_channelID, uint32_t* pu32_data);

/* Function to stop the ADC from running */
enuAdc_Status_t ADC_stop(void);

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*-*-*-*-*- EXTERN VARIABLES -*-*-*-*-*-*-*-*-*/
extern strAdc_Config_t str_ADCchannels[ADC_CHANNELS_USED];

#endif /* ADC_H_ */