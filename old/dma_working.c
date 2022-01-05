#include "Board_LED.h"                  // ::Board Support:LED
#include "Board_Buttons.h"              // ::Board Support:Buttons
#include "GPIO_LPC17xx.h"               // Keil::Device:GPIO
#include "PIN_LPC17xx.h"                // Keil::Device:PIN

#define TAB_SIZE 128

int32_t sound[TAB_SIZE] = {(256 << 6), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void config_DMA()
{
	// globalna konfiguracja
	LPC_SC->PCONP |= (1 << 29);
	LPC_GPDMA->DMACConfig = 1;
	LPC_GPDMA->DMACIntTCClear = 1;
	LPC_GPDMA->DMACIntErrClr = 1;
	
	// kanal 0
	LPC_GPDMACH0->DMACCSrcAddr = (int)sound;
	LPC_GPDMACH0->DMACCDestAddr = (int)&LPC_DAC->DACR;
	LPC_GPDMACH0->DMACCLLI = 0;
	LPC_GPDMACH0->DMACCControl = TAB_SIZE | (2 << 18) | (2 << 21) | (1 << 26) | (1UL << 31);
	NVIC_EnableIRQ(DMA_IRQn);
	LPC_GPDMACH0->DMACCConfig = 1 | (7 << 6) | (1 << 11) | (1 << 14) | (1 << 15);
}

void DMA_IRQHandler(void)
{
	LPC_GPDMACH0->DMACCSrcAddr = (int)sound;
	LPC_GPDMACH0->DMACCDestAddr = (int)&LPC_DAC->DACR;
	LPC_GPDMACH0->DMACCControl = TAB_SIZE | (2 << 18) | (2 << 21) | (1 << 26) | (1UL << 31);
	LPC_GPDMACH0->DMACCConfig = 1 | (7 << 6) | (1 << 11) | (1 << 14) | (1 << 15);
	LPC_GPDMA->DMACIntTCClear = 1;
	LPC_GPDMA->DMACIntErrClr = 1;
}

void config_DAC()
{
	PIN_Configure(0, 26, 2, PIN_PINMODE_TRISTATE, PIN_PINMODE_NORMAL);
	LPC_DAC->DACCTRL |= (1 << 2) | (1 << 3);
	LPC_DAC->DACCNTVAL = 568; //25 MHz -> 44 kHz
}

void config()
{
	config_DMA();
	config_DAC();
}

int main()
{ 
	config();
	
	while(1);
}
