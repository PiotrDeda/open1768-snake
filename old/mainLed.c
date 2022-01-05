#include "Board_LED.h"                  // ::Board Support:LED
#include "Board_Buttons.h"              // ::Board Support:Buttons
#include "GPIO_LPC17xx.h"               // Keil::Device:GPIO
#include "PIN_LPC17xx.h"                // Keil::Device:PIN

void config_Timer1()
{
	LPC_TIM1->PR = 0;
	LPC_TIM1->MR0 = SystemCoreClock / 8;
	LPC_TIM1->MCR = 3;
	LPC_TIM1->TCR = 1;
	NVIC_EnableIRQ(TIMER1_IRQn);
}

char leds[4] = {1, 2, 3, 10};

void TIMER1_IRQHandler(void)
{
	LPC_TIM1->IR = 1;
}

void config_DMA()
{
	// globalna konfiguracja
	LPC_SC->PCONP |= (1 << 29);
	LPC_GPDMA->DMACConfig = 1;
	LPC_SC->DMAREQSEL = 4;
	LPC_GPDMA->DMACIntTCClear = 1;
	LPC_GPDMA->DMACIntErrClr = 1;
	
	// kanal 0
	LPC_GPDMACH0->DMACCSrcAddr = (int)leds;
	LPC_GPDMACH0->DMACCDestAddr = (int)&LPC_GPIO0->FIODIR;
	LPC_GPDMACH0->DMACCLLI = 0;
	LPC_GPDMACH0->DMACCControl = 4 | (1 << 26) | (1UL << 31);
	NVIC_EnableIRQ(DMA_IRQn);
	LPC_GPDMACH0->DMACCConfig = 1 | (10 << 6) | (1 << 11) | (1 << 14) | (1 << 15);
}

void DMA_IRQHandler(void)
{
	LPC_GPDMACH0->DMACCSrcAddr = (int)&leds;
	LPC_GPDMACH0->DMACCDestAddr = (int)&LPC_GPIO0->FIODIR;
	LPC_GPDMACH0->DMACCControl = 4 | (1 << 26) | (1UL << 31);
	LPC_GPDMACH0->DMACCConfig = 1 | (10 << 6) | (1 << 11) | (1 << 14) | (1 << 15);
	LPC_GPDMA->DMACIntTCClear = 1;
	LPC_GPDMA->DMACIntErrClr = 1;
}

void config()
{
	config_Timer1();
	config_DMA();
}

int main()
{ 
	config();
	
	while(1);
}
