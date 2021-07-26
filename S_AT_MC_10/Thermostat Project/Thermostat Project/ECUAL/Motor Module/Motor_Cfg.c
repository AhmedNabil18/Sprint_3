/*
 * Motor_Cfg.c
 *
 * Created: 7/15/2021 11:12:52 PM
 *  Author: Ahmed Nabil
 */ 

#include "Motor.h"

/*
 * Array of Structures, each holds the SWPWM Channel, The two Direction pins, 
 * and the Operating frequency connected with the Motor
 */
const strMotor_Config_t str_MotorsConfig[MOTORS_USED_NUM] =
{
	{MOTOR_FAN_SWPWM_CHANNEL,       MOTOR_FAN_DIO1_CHANNEL,       MOTOR_FAN_DIO2_CHANNEL,       MOTOR_FAN_OPERATING_FREQ}
};