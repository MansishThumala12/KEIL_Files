/**
  ******************************************************************************
  * @file		delay.c
  * @author	Yohanes Erwin Setiawan
  * @date		10 January 2016
  ******************************************************************************
  */
	
#include "delay.h"

#define Static_delay

#if defined(Static_delay)
static __IO uint32_t sysclk;

#else
// For store tick counts in us
static __IO uint32_t usTicks;

// SysTick_Handler function will be called every 1 us
void SysTick_Handler()
{
	if (usTicks != 0)
	{
		usTicks--;
	}
}
#endif


void DelayInit()
{
	
	#if defined(Static_delay)
	SystemCoreClockUpdate();
	sysclk = SystemCoreClock;
	#else
	// Update SystemCoreClock value
	SystemCoreClockUpdate();
	// Configure the SysTick timer to overflow every 1 us
	SysTick_Config(SystemCoreClock / 1000000);
	#endif
}

void DelayUs(uint32_t us)
{
	#if defined(Static_delay)
	uint32_t cnt = (sysclk/6000000)*us;
	while(cnt-- > 0);
	#else
	// Reload us value
	usTicks = us;
	// Wait until usTick reach zero
	while (usTicks);
	#endif
}


void DelayMs(uint32_t ms)
{
	#if defined(Static_delay)
	uint32_t cnt = (sysclk/6000)*ms;
	while(cnt-->0){
		__nop();
	}
	#else
	// Wait until ms reach zero
	while (ms--)
	{
		// Delay 1ms
		DelayUs(1000);
	}
	#endif
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
