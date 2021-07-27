/*
 * Delay.c
 *
 * Created: 7/24/2021 11:03:45 AM
 *  Author: Ahmed Nabil
 */ 
#include "Delay.h"


void Delay_us(uint64_t u64_time_us)
{

}
void Delay_ms(uint64_t u64_time_ms)
{
	uint64_t i,j;
	for (i=0;i<u64_time_ms;i++) // this is For(); loop delay used to define delay value in Embedded C
	{
		for (j=0;j<DELAY_MS_ITERATION;j++);
	}
}
void Delay_s(uint32_t u32_time_s)
{
	
}