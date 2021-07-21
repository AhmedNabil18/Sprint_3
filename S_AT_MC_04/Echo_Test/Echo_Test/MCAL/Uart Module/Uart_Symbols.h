 /******************************************************************************
 *
 * Module: Uart
 *
 * File Name: Uart_Symbols.h
 *
 * Description: Header file for Uart Module Symbols.
 *
 * Author: Ahmed Nabil
 ******************************************************************************/



#ifndef UART_SYMBOLS_H_
#define UART_SYMBOLS_H_

/*
 * UCSRC Register Symbols
 */
#define UART_UCSRC_URSEL		7U
#define UART_UCSRC_UMSEL		6U
#define UART_UCSRC_UPM1			5U
#define UART_UCSRC_UPM0			4U
#define UART_UCSRC_USBS			3U
#define UART_UCSRC_UCSZ1		2U
#define UART_UCSRC_UCSZ0		1U
#define UART_UCSRC_UCPOL		0U


/*
 * UCSRA Register Symbols
 */
#define UART_UCSRA_RXC			7U
#define UART_UCSRA_TXC			6U
#define UART_UCSRA_UDRE			5U
#define UART_UCSRA_FE			4U
#define UART_UCSRA_DOR			3U
#define UART_UCSRA_PE			2U
#define UART_UCSRA_U2X			1U
#define UART_UCSRA_MPCM			0U

/*
 * UCSRB Register Symbols
 */
#define UART_UCSRB_RXCIE		7U
#define UART_UCSRB_TXCIE		6U
#define UART_UCSRB_UDRIE		5U
#define UART_UCSRB_RXEN			4U
#define UART_UCSRB_TXEN			3U
#define UART_UCSRB_UCSZ2		2U
#define UART_UCSRB_RXB8			1U
#define UART_UCSRB_TXB8			0U

#endif /* UART_SYMBOLS_H_ */