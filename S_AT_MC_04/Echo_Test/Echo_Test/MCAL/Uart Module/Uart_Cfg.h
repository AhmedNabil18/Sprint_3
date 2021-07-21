/*
 * Uart_Cfg.h
 *
 * Created: 7/18/2021 2:54:00 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef UART_CFG_H_
#define UART_CFG_H_

/*- The Frequency of the Controller -*/
#define F_OSC					1000000UL
/*- The Operation Baudrate of the UART -*/
#define USART_BAUDRATE			9600UL


/*- UART Enable Transmission and/or Reception -*/
/* :Options:
 * UART_TX_EN
 * UART_RX_EN
 * UART_TX_RX_EN
 */
#define UART_ENABLE				UART_TX_RX_EN

/*- Size of Uart Frame data bits -*/
/* :Options:
 * UART_SIZE_5_BITS
 * UART_SIZE_6_BITS
 * UART_SIZE_7_BITS
 * UART_SIZE_8_BITS
 * UART_SIZE_9_BITS
 */
#define UART_SIZE				UART_SIZE_8_BITS

/*- Enable the Double baudrate speed -*/
/* :Options:
 * UART_U2X_ENABLE
 * UART_U2X_DISABLE
 */
#define UART_DBL_SPEED			UART_U2X_ENABLE

/*- UART Parity Disable/Enable Even/Odd -*/
/* :Options:
 * UART_PARITY_DISABLE
 * UART_PARITY_EVEN
 * UART_PARITY_ODD
 */
#define UART_PARITY				UART_PARITY_DISABLE

/*- UART Stop bits Number -*/
/* :Options:
 * UART_STOP_1_BIT
 * UART_STOP_2_BITS
 */
#define UART_STOP_BITS			UART_STOP_1_BIT

// /*- UART Interrupt Options -*/
// /* :Options:
//  * UART_INTERRUPT_DISABLE
//  * UART_INTERRUPT_RXCIE
//  * UART_INTERRUPT_TXCIE
//  * UART_INTERRUPT_UDRIE
//  * Or using multiple options ORed ( || ) together
//  * @example: (UART_INTERRUPT_RXCIE || UART_INTERRUPT_TXCIE)
//  *			to enable TX and RX interrupts together.
//  */
// #define UART_INTERRUPT			UART_INTERRUPT_DISABLE

#endif /* UART_CFG_H_ */