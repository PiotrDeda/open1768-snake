#include "Open1768_LCD.h"

volatile uint32_t ticks = 0;

void SysTick_Handler(void)
{
	ticks++;
}

// t = 10 -> 1 sekunda
void wait(int t)
{
	ticks = 0;
	while(ticks < t)
		__WFI();
}
