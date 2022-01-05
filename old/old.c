#include "Board_LED.h"                  // ::Board Support:LED
#include "GPIO_LPC17xx.h"               // Keil::Device:GPIO
#include "PIN_LPC17xx.h"                // Keil::Device:PIN

//
// SYSTICK
//

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

//
// UART
//

void config_UART()
{
	PIN_Configure(0, 2, 1, PIN_PINMODE_TRISTATE, PIN_PINMODE_NORMAL);
	PIN_Configure(0, 3, 1, PIN_PINMODE_TRISTATE, PIN_PINMODE_NORMAL);
	
	LPC_UART0->LCR = (1 << 7) | (3 << 0);
	LPC_UART0->DLL = 13;
	LPC_UART0->DLM = 0;
	LPC_UART0->FDR = (1 << 0) | (15 << 4);
	LPC_UART0->LCR = 3;
}

void send_string(char str[])
{
	int i = 0;
	
	while(str[i] != '\0')
	{
		if ((LPC_UART0->LSR & (1 << 5)) != 0)
		{
			LPC_UART0->THR = str[i];
			i++;
		}
	}
}

char read_char()
{
	while((LPC_UART0->LSR & (1 << 0)) == 0)
		wait(1);
	return LPC_UART0->RBR;
}

//
// TIMER
//

void config_Timer()
{
	LPC_TIM0->PR = 0;
	LPC_TIM0->MR0 = SystemCoreClock / 8;
	LPC_TIM0->MCR = 3;
	LPC_TIM0->TCR = 1;
	NVIC_EnableIRQ(TIMER0_IRQn);
}

void TIMER0_IRQHandler(void)
{
	send_string("Ping\n\r");
	LPC_TIM0->IR = 1;
}

//
// RTC
//

int tik = 1;

void config_RTC ()
{
	LPC_RTC->CCR |= 1;
	LPC_RTC->ILR = 1;
	LPC_RTC->CIIR = 1;
	NVIC_EnableIRQ(RTC_IRQn);
}

void RTC_IRQHandler(void)
{
	if(tik)
	{
		send_string("tik\n\r");
		tik = 0;
	}
	else
	{
		send_string("tak\n\r");
		tik = 1;
	} 
	LPC_RTC->ILR = 1;
}

//
// EINT0
//

int led_blinking_enabled = 0;

void config_EINT()
{
	PIN_Configure(2, 10, 1, PIN_PINMODE_PULLUP, PIN_PINMODE_NORMAL);
	LPC_SC->EXTMODE = 1;
	LPC_SC->EXTPOLAR = 0;
	LPC_SC->EXTINT = 1;
	NVIC_EnableIRQ(EINT0_IRQn);
}

void EINT0_IRQHandler(void)
{
	led_blinking_enabled = 4;
	send_string("Click\n\r");
	
	LPC_SC->EXTINT = 1;
}

//
// LCD
//

void config_GPIOINT()
{
	LPC_GPIOINT->IO0IntEnF |= (1 << 19);
	NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler(void)
{
	send_string("Bang");
	LPC_GPIOINT->IO0IntClr |= (1 << 19);
}

//
// Config
//

void config()
{
	LED_Initialize();
	LED_SetOut(0);
	SysTick_Config(SystemCoreClock / 10);
	config_UART();
	config_Timer();
	config_RTC();
	config_EINT();
	config_GPIOINT();
}


int main()
{ 
	config();
	
	while(1)
	{
		if (led_blinking_enabled > 0)
		{
			led_blinking_enabled--;
			LED_On(2);
			wait(5);
			LED_Off(2);
			wait(5);
		}
	}
}
