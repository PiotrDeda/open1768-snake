#include "Board_LED.h"                  // ::Board Support:LED
#include "GPIO_LPC17xx.h"               // Keil::Device:GPIO
#include "PIN_LPC17xx.h"                // Keil::Device:PIN
#include "Board_Buttons.h"              // ::Board Support:Buttons
#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "TP_Open1768.h"

#include "systick.h"
#include "lcdDrawing.h"
#include "lcdTouch.h"

void config()
{
	lcdConfiguration();
	init_ILI9325();
	touchpanelInit();
	SysTick_Config(SystemCoreClock / 10);
	config_GPIOINT();
	Buttons_Initialize();
}

int main()
{ 
	config();
	
	calibrate();
	
	int x, y;
	int color = LCDBlack;
	
	while(1)
	{
		if (Buttons_GetState() == 1)
			clear_canvas();
		read_touch(&y, &x);
		touch_to_lcd(&x, &y);
		if (y > 50)
			draw_cross(x, LCD_MAX_Y - y, color);
		else if (x <= 50)
			color = LCDBlue;
		else if (x <= 100)
			color = LCDYellow;
		else if (x <= 150)
			color = LCDGreen;
		else if (x <= 200)
			color = LCDRed;
		else if (x <= 250)
			color = LCDBlack;
	}
}
