/*
 * Motor_Cfg.h
 *
 * Created: 7/15/2021 11:13:13 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef MOTOR_CFG_H_
#define MOTOR_CFG_H_

/* Number of Motors Used */
#define MOTORS_USED_NUM				1U

/* Motor Right Configurations */
#define MOTOR_FAN_OPERATING_FREQ		100U				/* Opearting Frequency */
#define MOTOR_FAN_SWPWM_CHANNEL			SWPWM_CHANNEL_0_ID  /* SWPWMM Used for the Motor */
#define MOTOR_FAN_DIO1_CHANNEL			DIO_IN1_CHANNEL_ID	/* 1st Direction Pin for the Motor */
#define MOTOR_FAN_DIO2_CHANNEL			DIO_IN2_CHANNEL_ID	/* 2nd Direction Pin for the Motor */

/* ID for the Motors Used */
#define MOTOR_CASH_ID				0U

#endif /* MOTOR_CFG_H_ */