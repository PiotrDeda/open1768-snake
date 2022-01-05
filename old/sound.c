#include "sound.h"

bool sound_playing = false;
int sound_counter = 0;
int sound_length = 0;
int sound_max = 0;
const int32_t *sound_played;

void config_DMA()
{
	LPC_SC->PCONP |= (1 << 29);
	LPC_GPDMA->DMACConfig = 1;
	LPC_GPDMA->DMACIntTCClear = 1;
	LPC_GPDMA->DMACIntErrClr = 1;
}

void DMA_IRQHandler(void)
{
	if (sound_playing == false)
		return;
	
	LPC_GPDMACH0->DMACCSrcAddr = (int)(sound_played + (sound_max - sound_counter) * 4095);
	LPC_SC->PCONP |= (1 << 29);
	LPC_GPDMA->DMACConfig = 1;
	LPC_GPDMA->DMACIntTCClear = 1;
	LPC_GPDMA->DMACIntErrClr = 1;
	
	LPC_GPDMACH0->DMACCDestAddr = (int)&LPC_DAC->DACR;
	LPC_GPDMACH0->DMACCLLI = 0;
	LPC_GPDMACH0->DMACCControl = 4095 | (2 << 18) | (2 << 21) | (1 << 26) | (1UL << 31);
	NVIC_EnableIRQ(DMA_IRQn);
	LPC_GPDMACH0->DMACCConfig = 1 | (7 << 6) | (1 << 11) | (1 << 14) | (1 << 15);
	
	sound_counter -= 1;
	
	LPC_GPDMA->DMACIntTCClear = 1;
	LPC_GPDMA->DMACIntErrClr = 1;
	
	if (sound_counter < 0)
		sound_playing = false;
}

void config_DAC()
{
	PIN_Configure(0, 26, 2, PIN_PINMODE_TRISTATE, PIN_PINMODE_NORMAL);
	LPC_DAC->DACCTRL |= (1 << 2) | (1 << 3);
	LPC_DAC->DACCNTVAL = 568; //25 MHz -> 44 kHz
}

void config_sound()
{
	config_DMA();
	config_DAC();
}

void play_sound(const int32_t sound[], int sound_len)
{
	if (sound_playing == true)
		return;
	sound_playing = true;
	
	LPC_GPDMACH0->DMACCSrcAddr = (int)sound;
	LPC_SC->PCONP |= (1 << 29);
	LPC_GPDMA->DMACConfig = 1;
	LPC_GPDMA->DMACIntTCClear = 1;
	LPC_GPDMA->DMACIntErrClr = 1;
	
	LPC_GPDMACH0->DMACCDestAddr = (int)&LPC_DAC->DACR;
	LPC_GPDMACH0->DMACCLLI = 0;
	LPC_GPDMACH0->DMACCControl = sound_len | (2 << 18) | (2 << 21) | (1 << 26) | (1UL << 31);
	NVIC_EnableIRQ(DMA_IRQn);
	LPC_GPDMACH0->DMACCConfig = 1 | (7 << 6) | (1 << 11) | (1 << 14) | (1 << 15);
}

void play_long_sound(const int32_t sound[], int sound_len)
{
	if (sound_playing == true)
		return;
	sound_playing = true;
	
	sound_counter = sound_len / 4095 - 1;
	sound_length = sound_len;
	sound_max = sound_counter + 1;
	sound_played = sound;
	
	LPC_GPDMACH0->DMACCSrcAddr = (int)sound;
	LPC_SC->PCONP |= (1 << 29);
	LPC_GPDMA->DMACConfig = 1;
	LPC_GPDMA->DMACIntTCClear = 1;
	LPC_GPDMA->DMACIntErrClr = 1;
	
	LPC_GPDMACH0->DMACCDestAddr = (int)&LPC_DAC->DACR;
	LPC_GPDMACH0->DMACCLLI = 0;
	LPC_GPDMACH0->DMACCControl = 4095 | (2 << 18) | (2 << 21) | (1 << 26) | (1UL << 31);
	NVIC_EnableIRQ(DMA_IRQn);
	LPC_GPDMACH0->DMACCConfig = 1 | (7 << 6) | (1 << 11) | (1 << 14) | (1 << 15);
	
	/*for (int i = 0; i < sound_len; i += 4095)
	{
		LPC_GPDMACH0->DMACCSrcAddr = (int)(sound+i);
		LPC_SC->PCONP |= (1 << 29);
		LPC_GPDMA->DMACConfig = 1;
		LPC_GPDMA->DMACIntTCClear = 1;
		LPC_GPDMA->DMACIntErrClr = 1;
		
		LPC_GPDMACH0->DMACCDestAddr = (int)&LPC_DAC->DACR;
		LPC_GPDMACH0->DMACCLLI = 0;
		LPC_GPDMACH0->DMACCControl = sound_len | (2 << 18) | (2 << 21) | (1 << 26) | (1UL << 31);
		NVIC_EnableIRQ(DMA_IRQn);
		LPC_GPDMACH0->DMACCConfig = 1 | (7 << 6) | (1 << 11) | (1 << 14) | (1 << 15);
	}*/
}
