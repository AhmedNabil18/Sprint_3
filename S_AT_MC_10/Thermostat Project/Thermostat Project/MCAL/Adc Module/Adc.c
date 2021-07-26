/*
 * Adc.c
 *
 * Created: 7/24/2021 11:24:59 PM
 *  Author: Ahmed Nabil
 */ 
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Adc.h"
#include "..\ATMega32_Registers.h"


static void (*ADC_CallBackPtr)(void);
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- GLOBAL STATIC VARIABLES *-*-*-*-*-*/
static enuAdc_Status_t genuAdc_Status = ADC_STATUS_NOT_INIT;


/*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- FUNCTIONS IMPLEMENTATION -*-*-*-*-*-*/

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Adc_init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuAdc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Initialize the Adc module.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuAdc_Status_t Adc_init(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the ADC module is already initialized */
	if (ADC_STATUS_INIT == genuAdc_Status)
	{
		return ADC_STATUS_ALREADY_INIT;
	}else{/*Nothing to here*/}
	
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	uint8_t u8_loopIndex=0;
	for (u8_loopIndex=0; u8_loopIndex<ADC_CHANNELS_USED; u8_loopIndex++)
	{
		ADC_PORT &= ~(1<<str_ADCchannels[u8_loopIndex].u8_pinID);	//Set the corresponding pins as input
	}
	ADC_ADCSRA_REG = ADC_PRESCALER << ADC_ADCSRA_ADPS0;						//Set the prescaler
	ADC_ADCSRA_REG &= ~(1<<ADC_ADCSRA_ADATE);					//Force Disable auto trigger mode
	ADC_ADCSRA_REG |= ADC_AUTO_TRIGG_SRC<<ADC_ADCSRA_ADATE;		//set the auto trigger mode as chosen in configuration
	
	/* Set the ADC Module State to Initialized*/
	genuAdc_Status = ADC_STATUS_INIT;
	return ADC_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: ADC_stop
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuAdc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to stop the ADC from running.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuAdc_Status_t ADC_stop(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the ADC module is already initialized */
	if (ADC_STATUS_INIT == genuAdc_Status)
	{
		return ADC_STATUS_ALREADY_INIT;
	}else{/*Nothing to here*/}
		
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Disable the ADC Block */
	ADC_ADCSRA_REG &= ~(1<<ADC_ADCSRA_ADEN);
	
	/* Stop Conversion */
	ADC_ADCSRA_REG &= ~(1<<ADC_ADCSRA_ADSC);
	
	/* Disable the Interrupt Option */
	ADC_ADCSRA_REG &= ~(1<<ADC_ADCSRA_ADIE);
	
	return ADC_STATUS_ERROR_OK;
}
	
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Adc_start
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_channelID - Index of Channel in the config struct
* Parameters (inout): None
* Parameters (out): pu32_data - Pointer to a variable to hold the analog voltage in mV.
* Return value: enuAdc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to start the ADC Conversion and Poll for Data.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuAdc_Status_t Adc_start(uint8_t u8_channelID, uint32_t* pu32_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the ADC module is not initialized */
	if (ADC_STATUS_INIT != genuAdc_Status)
	{
		return ADC_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	/* Check if the data pointer is NULL */
	if (NULL_PTR == pu32_data)
	{
		return ADC_STATUS_NULL_ARGUMENT;
	}else{/*Nothing to here*/}
		
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	ADC_ADMUX_REG |= (str_ADCchannels[u8_channelID].u8_pinID)<<ADC_ADMUX_MUX0;					//set the chosen ADC channel from the mux
	ADC_ADMUX_REG |= (str_ADCchannels[u8_channelID].u8_Vref)<<ADC_ADMUX_REFS0;		//set the voltage reference
	
	/* Enable the ADC Block */
	ADC_ADCSRA_REG |= 1<<ADC_ADCSRA_ADEN;
			
	/* Start Conversion */
	ADC_ADCSRA_REG |= 1<<ADC_ADCSRA_ADSC;
		
	while ((ADC_ADCSRA_REG & (1<<ADC_ADCSRA_ADIF)) == 0);
	uint8_t u8_lowByte = ADC_ADCL_REG;
	uint16_t u16_Result = ADC_ADCH_REG<<8 | u8_lowByte;
	float32_t f32_Resolution=0;
	if(str_ADCchannels[u8_channelID].u8_Vref == ADC_VREF_2p56V)
	{
		f32_Resolution = ((float32_t)1000*2.56)/ADC_STEPS_NUM;
	}else if(str_ADCchannels[u8_channelID].u8_Vref == ADC_VREF_VCC)
	{
		f32_Resolution = ((float32_t)1000*ADC_AVCC_VALUE)/ADC_STEPS_NUM;
	}else if(str_ADCchannels[u8_channelID].u8_Vref == ADC_VREF_AREF)
	{
		f32_Resolution = ((float32_t)1000*ADC_AREF_VALUE)/ADC_STEPS_NUM;
	}
	*pu32_data = u16_Result*f32_Resolution;
	ADC_ADCSRA_REG |= 1<<ADC_ADCSRA_ADIF;

	return ADC_STATUS_ERROR_OK;
}
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: ADC_startWithInterrupt
* Sync/Async: ASynchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_channelID - Index of Channel in the config struct
*			       pf_callback - Pointer to the callback funtion.
* Parameters (inout): None
* Parameters (out): None
* Return value: enuAdc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to start the ADC Conversion and enables the interrupt.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuAdc_Status_t ADC_startWithInterrupt(uint8_t u8_channelID, pfAdc_CallBack_t pf_callback)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the ADC module is not initialized */
	if (ADC_STATUS_INIT != genuAdc_Status)
	{
		return ADC_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
			
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	ADC_ADMUX_REG |= (str_ADCchannels[u8_channelID].u8_pinID)<<ADC_ADMUX_MUX0;					//set the chosen ADC channel from the mux
	ADC_ADMUX_REG |= (str_ADCchannels[u8_channelID].u8_Vref)<<ADC_ADMUX_REFS0;		//set the voltage reference
	
	/* Set the callback function */
	ADC_CallBackPtr = pf_callback;
	
	/* Enable the Interrupt Option */
	ADC_ADCSRA_REG |= 1<<ADC_ADCSRA_ADIE;
			
	/* Enable the ADC Block */
	ADC_ADCSRA_REG |= 1<<ADC_ADCSRA_ADEN;
			
	/* Start Conversion */
	ADC_ADCSRA_REG |= 1<<ADC_ADCSRA_ADSC;
			
	return ADC_STATUS_ERROR_OK;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: ADC_readFromISR
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_channelID - Index of Channel in the config struct
* Parameters (inout): None
* Parameters (out): pu32_data - Pointer to a variable to hold the analog voltage in mV.
* Return value: enuAdc_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Read Data from ISR Functions == CALLBACK functions.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
enuAdc_Status_t ADC_readFromISR(uint8_t u8_channelID, uint32_t* pu32_data)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the ADC module is not initialized */
	if (ADC_STATUS_INIT != genuAdc_Status)
	{
		return ADC_STATUS_NOT_INIT;
	}else{/*Nothing to here*/}
	/* Check if the data pointer is NULL */
	if (NULL_PTR == pu32_data)
	{
		return ADC_STATUS_NULL_ARGUMENT;
	}else{/*Nothing to here*/}
			
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/

	uint8_t u8_lowByte = ADC_ADCL_REG;
	uint16_t u16_Result = ADC_ADCH_REG<<8 | u8_lowByte;
	float32_t f32_Resolution=0;
	if(str_ADCchannels[u8_channelID].u8_Vref == ADC_VREF_2p56V)
	{
		f32_Resolution = (1000*2.56)/ADC_STEPS_NUM;
	}else if(str_ADCchannels[u8_channelID].u8_Vref == ADC_VREF_VCC)
	{
		f32_Resolution = (1000*ADC_AVCC_VALUE)/ADC_STEPS_NUM;
	}else if(str_ADCchannels[u8_channelID].u8_Vref == ADC_VREF_AREF)
	{
		f32_Resolution = (1000*ADC_AREF_VALUE)/ADC_STEPS_NUM;
	}
	*pu32_data = u16_Result*f32_Resolution;
		
	return ADC_STATUS_ERROR_OK;
}

ISR(ADC_IRQ)
{
	(*ADC_CallBackPtr)();
}