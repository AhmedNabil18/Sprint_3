/*
 * Motor.c
 *
 * Created: 7/15/2021 11:12:37 PM
 *  Author: Ahmed Nabil
 */ 

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Motor.h"

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- GLOBAL STATIC VARIABLES *-*-*-*-*-*/
u8_MotorState_t	gau8_MotorsState[MOTORS_USED_NUM] = {MOTOR_STOPPED};
uint8_t gu8_MotorModuleStatus = MOTOR_STATUS_UNINIT;
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*- FUNCTIONS IMPLEMENTATION -*-*-*-*-*-*/

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Motor_init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: enuMotor_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Initialize the Motor module.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/* Function to initialize the motor module */
enuMotor_Status_t Motor_init(void)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Motor module is already initialized */
	if(gu8_MotorModuleStatus == MOTOR_STATUS_INIT)
	{
		return MOTOR_STATUS_INIT;
	}else{/*Nothing to here*/}
		
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/
/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Initialize the Software PWM Module */
	if(SWPwm_Init() != SWPWM_STATUS_ERROR_OK)
		return MOTOR_STATUS_ERROR_NOK;
	
	/* Change the state of the module to initialized */
	gu8_MotorModuleStatus = MOTOR_STATUS_INIT;
	return MOTOR_STATUS_ERROR_OK;
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Motor_run
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_motorID - Index for the motor in the module
*					u8_speed - Speed of the motor in %
*					u8_direction - Direction of the motor (CLOCK WISE - ANTI CLOCK WISE)
* Parameters (inout): None
* Parameters (out): None
* Return value: enuMotor_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Move the motor in the given direction.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/* Function to move the motor forward with given speed in % */
enuMotor_Status_t Motor_run(uint8_t u8_motorID, uint8_t u8_speed, uint8_t u8_direction)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the Motor module is not initialized */
	if(gu8_MotorModuleStatus != MOTOR_STATUS_INIT)
	{
		return MOTOR_STATUS_UNINIT;
	}else{/*Nothing to here*/}
	/* Check if the motor index is invalid */
	if (MOTORS_USED_NUM <= u8_motorID)
	{
		return MOTOR_STATUS_ERROR_ID_INVALID;
	}else{/*Nothing to here*/}
		
	/* Check if the speed is out of range */
	if (100 < u8_speed)
	{
		return MOTOR_STATUS_ERROR_SPD_INVALID;
	}else{/*Nothing to here*/}
		
	/* Check if the motor index is invalid */
	if (MOTOR_DIR_ANTI_CLK_WISE < u8_direction)
	{
		return MOTOR_STATUS_ERROR_DIR_INVALID;
	}else{/*Nothing to here*/}
	
	/* Check if the motor is already running */
	if(gau8_MotorsState[u8_motorID] == MOTOR_RUNNING)		
	{
		return MOTOR_STATUS_ERROR_RUNNING;
	}else{/*Nothing to here*/}
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	if(u8_speed == 0) 
	{
		Motor_stop(u8_motorID);
		return MOTOR_STATUS_ERROR_OK;
	}
	if(u8_direction == MOTOR_DIR_CLK_WISE) /* Activate the Motor in the Clock Wise Direction */
	{
		Dio_writePin(str_MotorsConfig[u8_motorID].u8_MotorDirPin1, PIN_HIGH);
		Dio_writePin(str_MotorsConfig[u8_motorID].u8_MotorDirPin2, PIN_LOW);
	}else if(u8_direction == MOTOR_DIR_ANTI_CLK_WISE) /* Activate the Motor in the Anti Clock Wise Direction */
	{
		Dio_writePin(str_MotorsConfig[u8_motorID].u8_MotorDirPin1, PIN_LOW);
		Dio_writePin(str_MotorsConfig[u8_motorID].u8_MotorDirPin2, PIN_HIGH);
	}
	
	/* Change the state of the Motor to Running */
	gau8_MotorsState[u8_motorID] = MOTOR_RUNNING;
	
	/* Start the PWM Wave for the given speed and frequency */
	SWPwm_Start(str_MotorsConfig[u8_motorID].u8_MotorPwmChannel, str_MotorsConfig[u8_motorID].u16_Frequency, u8_speed);
	
	return MOTOR_STATUS_ERROR_OK;
}

/* Function to move the motor backward with given speed in % */
//enuMotor_Status_t Motor_moveBackward(uint8_t u8_motorID, uint8_t u8_speed);

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Service Name: Motor_stop
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): u8_motorID - Index for the motor in the module
* Parameters (inout): None
* Parameters (out): None
* Return value: enuMotor_Status_t - return the status of the function ERROR_OK or NOT_OK
* Description: Function to Stop the motor.
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/* Function to stop motor */
enuMotor_Status_t Motor_stop(uint8_t u8_motorID)
{
/**************************************************************************************/
/*								Start of Error Checking								  */
/**************************************************************************************/
	/* Check if the motor index is invalid */
	if (MOTORS_USED_NUM <= u8_motorID)
	{
		return MOTOR_STATUS_ERROR_ID_INVALID;
	}else{/*Nothing to here*/}
				
/**************************************************************************************/
/*								End of Error Checking								  */
/**************************************************************************************/

/**************************************************************************************/
/*								Function Implementation								  */
/**************************************************************************************/
	/* Stop the Motor by driving the pins to LOW */
	Dio_writePin(str_MotorsConfig[u8_motorID].u8_MotorDirPin1, PIN_LOW);
	Dio_writePin(str_MotorsConfig[u8_motorID].u8_MotorDirPin2, PIN_LOW);
	
	uint8_t u8_loopIndex = 0;
	
	/*
	 * Check if there is another motor that uses the same PWM Channel
	 * If any motor found using the same PWM Channel so we won't stop
	 * the PWM wave to prevent the other motor from stopping
	 * and only stop the enable pins for this motor
	 */
	for(u8_loopIndex = 0; u8_loopIndex<MOTORS_USED_NUM; u8_loopIndex++)
	{
		if(u8_loopIndex == u8_motorID) /* Skip the current motor */
		{
			continue;
		}
		if(str_MotorsConfig[u8_motorID].u8_MotorPwmChannel == str_MotorsConfig[u8_loopIndex].u8_MotorPwmChannel)
		{ /* Another motor with the ID u8_loopIndex is using the same PWM Channel --> then break to save its Index (u8_loopIndex) */
			break;
		}
	}
	
	
	if(u8_loopIndex == MOTORS_USED_NUM)
	{ /* In case that there isn't any motor using the same PWM Channel --> Stop the PWM */
		SWPwm_Stop(str_MotorsConfig[u8_motorID].u8_MotorPwmChannel);
	}else
	{/* In case that there is another motor using the same PWM Channel */
		if (gau8_MotorsState[u8_loopIndex] == MOTOR_STOPPED) /* Check if the other motor is not running */
		{/* In case that the other motor is not running --> Stop the PWM */
			SWPwm_Stop(str_MotorsConfig[u8_motorID].u8_MotorPwmChannel);
		} 
	}
	/* Change the state of this motor to Stopped */
	gau8_MotorsState[u8_motorID] = MOTOR_STOPPED;
	return MOTOR_STATUS_ERROR_OK;
}