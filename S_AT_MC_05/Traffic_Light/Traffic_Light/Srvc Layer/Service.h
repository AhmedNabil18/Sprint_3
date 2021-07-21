/*
 * Service.h
 *
 * Created: 7/16/2021 2:09:25 PM
 *  Author: Ahmed Nabil
 */ 


#ifndef SERVICE_H_
#define SERVICE_H_

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- INCLUDES *-*-*-*-*-*/
#include "../ECUAL/Terminal Module/Terminal.h"
#include "../ECUAL/Led Module/Led.h"
#include "Service_Cfg.h"

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*- CONSTANTS -*-*-*-*-*-*/

#define TERM_SRVC_OFF			0U
#define TERM_SRVC_AT			1U
#define TERM_SRVC_START			2U
#define TERM_SRVC_WAIT			3U
#define TERM_SRVC_STOP			4U

#define SRVC_ACTION_IDLE		0U
#define SRVC_ACTION_OK			1U
#define SRVC_ACTION_GREEN		2U
#define SRVC_ACTION_YELLOW		3U
#define SRVC_ACTION_RED			4U

/*******************************************************************************
 *                          Module Data Types                                  *
 *******************************************************************************/
/*
 * Data Type for App return status
 */
typedef enum
{
	SRVC_STATUS_ERROR_NOK,
	SRVC_STATUS_ERROR_OK,
	SRVC_STATUS_ERROR_ID_INVALID,
	SRVC_STATUS_ERROR_NULL,
	SRVC_STATUS_INITIALIZED,
	SRVC_STATUS_UNINITIALIZED
}enuSrvc_Status_t;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
/* Function to initialize the Service Layer */
enuSrvc_Status_t Service_init(void);

/* Function to report back the terminal input */
enuSrvc_Status_t Service_ReportTerminal(uint8_t* pu8_terminal);

/* Function to update the led Action */
enuSrvc_Status_t Service_UpdateLeds(uint8_t u8_ledAction);

/* Function to update the terminal output */
enuSrvc_Status_t Service_UpdateTerminal(uint8_t u8_terminalAction);

#endif /* SERVICE_H_ */