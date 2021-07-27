/*
 * SWPwm.h
 *
 * Created: 7/15/2021 3:10:45 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef SWPWM_H_
#define SWPWM_H_

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "..\Platform_Types.h"
#include "..\Dio Module\Dio.h"
#include "..\Gpt Module\Gpt.h"
#include "SWPwm_Cfg.h"


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- CONSTANTS -*-*-*-*-*-*/
#define SWPWM_MAX_DUTYCYCLE			100U
#define SWPWM_MIN_DUTYCYCLE			0U

/*******************************************************************************
 *                          Module Data Types                                  *
 *******************************************************************************/
/*
 * Data Type for PWM return status
 */
typedef enum
{
	SWPWM_STATUS_ERROR_NOK,
	SWPWM_STATUS_ERROR_OK,
	SWPWM_STATUS_NOT_INIT,
	SWPWM_STATUS_PWM_ID_INVALID,
	SWPWM_STATUS_DC_OUTRANGE,
	SWPWM_STATUS_RUNNING
}enuSWPwm_Status_t;

/*
 * Data Type for GPT state
 */
typedef enum
{
	SWPWM_UNINITIALIZED = 0,
	SWPWM_INITIALIZED,
	SWPWM_RUNNING,
	SWPWM_STOPPED,
	SWPWM_EXPIRED
}enuSWPwm_State_t;

/*
 * Data Type for the numeric ID for a PWM Channel 
 */
typedef uint8_t u8SWPwm_Channel_t;

/*
 * Data Type for the Channel Run Data
 */
typedef struct
{
	u8SWPwm_Channel_t  u8_ChannelID;
	uint32_t  u32_ONTicks;
	uint32_t  u32_OFFTicks;
	uint32_t  u32_NxtTicks;
	enuSWPwm_State_t enuChannelState;
}strSWPwm_ChannelData_t;



/*
 * Data Type for the PWM Channel Configuration
 */
typedef struct  
{
	uint8_t u8_DioChannelID;
	uint8_t u8_GptChannelID;
}strSWPwm_ChannelConfig_t;
/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/* Initialize the Software Pwm Module */
enuSWPwm_Status_t SWPwm_Init(void); 

/* Start the wave generation of the channel */
enuSWPwm_Status_t SWPwm_Start(u8SWPwm_Channel_t u8_ChannelID, uint32_t u32_Freq, uint8_t u8_DutyCycle);

/* Stop the wave generation of the channel */
enuSWPwm_Status_t SWPwm_Stop(u8SWPwm_Channel_t u8_ChannelID); 


/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/
/* extern for the PWM Configuration Structures */
extern const strSWPwm_ChannelConfig_t SWPwm_Channels[SWPWM_CHANNELS_NUM];

#endif /* SWPWM_H_ */