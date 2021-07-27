/*
 * Delay.h
 *
 * Created: 7/24/2021 11:03:34 AM
 *  Author: Ahmed Nabil
 */ 


#ifndef DELAY_H_
#define DELAY_H_

#include "Delay_Cfg.h"
#include "../Platform_Types.h"

#if F_DELAY_OSC == 1000000
#define DELAY_MS_ITERATION		13
#elif F_DELAY_OSC == 8000000
#define DELAY_MS_ITERATION		140
#endif

void Delay_us(uint64_t u64_time_us);
void Delay_ms(uint64_t u64_time_ms);
void Delay_s(uint32_t u32_time_s);

#endif /* DELAY_H_ */