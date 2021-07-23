/*
 * Uart.h
 *
 * Created: 7/18/2021 2:53:31 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef UART_H_
#define UART_H_


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "Uart_Cfg.h"
#include "..\Platform_Types.h"
#include "Uart_Symbols.h"
#include "..\ATMega32_Interrupts.h"

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- CONSTANTS -*-*-*-*-*-*/

#define UART_NOT_INITIALIZED			0U
#define UART_INITIALIZED				1U

#define UART_TX_EN					(1<<3)
#define UART_RX_EN					(1<<4)
#define UART_TX_RX_EN				(UART_TX_EN | UART_RX_EN)

#define UART_SIZE_5_BITS			0U
#define UART_SIZE_6_BITS			1U
#define UART_SIZE_7_BITS			2U
#define UART_SIZE_8_BITS			3U
#define UART_SIZE_9_BITS			7U

#define UART_U2X_ENABLE				1U
#define UART_U2X_DISABLE			0U

#define UART_PARITY_DISABLE			0U
#define UART_PARITY_EVEN			2U
#define UART_PARITY_ODD				3U

#define UART_STOP_1_BIT				0U
#define UART_STOP_2_BITS			1U


#define UART_INTERRUPT_DISABLE		0U
#define UART_INTERRUPT_RXCIE		(1<<7)
#define UART_INTERRUPT_TXCIE		(1<<6)
#define UART_INTERRUPT_UDRIE		(1<<5)

/*-*-*-*-*-*-*-*-*--*-*-*-*-*-*/
/*-*-*-*-*- MACROS -*-*-*-*-*-*/
#define BAUD_RATE					(((F_OSC / (USART_BAUDRATE * 16UL))) - 1)
#define BAUD_RATE_U2X				(((F_OSC / (USART_BAUDRATE * 8UL))) - 1)

/*
 * Data Type for a pointer to notification call back function
 * Parameter (In) : None
 * Parameter (Out): None
 * Return Values : None
 */
typedef void (*pfUart_CallBack_t)(void);

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- ENUMS -*-*-*-*-*-*/

typedef enum{
	UART_STATUS_ERROR_NOK,
	UART_STATUS_ERROR_OK,
	UART_STATUS_PIN_ID_INVALID,
	UART_STATUS_PIN_NUM_INVALID,
	UART_STATUS_PORT_NUM_INVALID,
	UART_STATUS_NULL_ARGUMENT,
	UART_STATUS_NOT_INIT,
	UART_STATUS_ALREADY_INIT
}enuUart_Status_t;


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- STRUCTS -*-*-*-*-*-*/

typedef struct
{
	uint8_t u8_Enable;
	uint8_t u8_DataFrameSize;
	uint8_t u8_DoubleSpeedEnable;
	uint8_t u8_ParityMode;
	uint8_t u8_StopBits;
}strUart_Config_t;


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*--*-*-*-*-*-*-*- FUNCTIONS PROTOTYPES -*-*-*-*-*-*-*-*-*/

/*- Initialize Uart Module -*/
enuUart_Status_t Uart_init(void);

/*- Send Single Data Frame 5~9 bits -*/
enuUart_Status_t Uart_sendByte(uint8_t u8_Data);

/*- Send Multiple Data Frames -*/
enuUart_Status_t Uart_sendPacket(uint8_t *pu8_Data, uint16_t u16_packetSize);

/*- Receive Single Data Frame 5~8 bits -*/
enuUart_Status_t Uart_receiveByte(uint8_t *pu8_Data);

/*- Receive Multiple Data Frames -*/
enuUart_Status_t Uart_receivePacket(uint8_t *pu8_Data, uint16_t u16_packetSize);

/*- Enable Interrupt Notification for TXC by calling Callback function -*/
enuUart_Status_t Uart_EnableNotification_TXC(pfUart_CallBack_t Uart_Callback);

/*- Enable Interrupt Notification for RXC by calling Callback function -*/
enuUart_Status_t Uart_EnableNotification_RXC(pfUart_CallBack_t Uart_Callback);

/*- Enable Interrupt Notification for UDRE by calling Callback function -*/
enuUart_Status_t Uart_EnableNotification_UDRE(pfUart_CallBack_t Uart_Callback);

/*- Disable Interrupt Notification for the given Interrupt Flag -*/
enuUart_Status_t Uart_DisableNotification(uint8_t u8_InterruptFlags);

/*- Read the data register directly -*/
uint8_t Uart_DataRegister(void);
/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/




#endif /* UART_H_ */