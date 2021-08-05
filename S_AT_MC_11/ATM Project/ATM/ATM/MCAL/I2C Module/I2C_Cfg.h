/*
 * I2C_Cfg.h
 *
 * Created: 7/21/2021 8:35:21 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef I2C_CFG_H_
#define I2C_CFG_H_

#define F_OSC				8000000UL

/*
 * Output Clock on SCL Pin
 */
#define I2C_SCL_CLK			25000UL

/*
 * Prescaler Value
 * @Options: I2C_PRESACLER_1
 *			 I2C_PRESACLER_4
 *			 I2C_PRESACLER_16
 *			 I2C_PRESACLER_64
 */
#define PRESCALER_VALUE		I2C_PRESACLER_16

/*
 * I2C Mode
 * @Options: I2C_MODE_MASTER
 *			 I2C_MODE_SLAVE
 */
#define I2C_MODE			I2C_MODE_MASTER



#endif /* I2C_CFG_H_ */