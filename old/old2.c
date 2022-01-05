#include "Board_LED.h"                  // ::Board Support:LED
#include "GPIO_LPC17xx.h"               // Keil::Device:GPIO
#include "PIN_LPC17xx.h"                // Keil::Device:PIN
#include "Board_Buttons.h"              // ::Board Support:Buttons
#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "TP_Open1768.h"

#include "systick.h"
#include "lcdDrawing.h"

#define SAMPLE_SIZE 5

volatile bool touched = false;
volatile int last_touch_x = 0;
volatile int last_touch_y = 0;
int x1, x2, y1, y2;

void wait_for_touch(int *x, int *y)
{
	while(1)
	{
		if (touched)
		{
			*x = last_touch_x;
			*y = last_touch_y;
			touched = false;
			break;
		}
	}
}

void read_touch(int *x, int *y)
{
	int sample_x, sample_y;
	*x = 0;
	*y = 0;
	for (int i = 0; i < SAMPLE_SIZE; i++)
	{
		touchpanelGetXY(&sample_x, &sample_y);
		*x += sample_x;
		*y += sample_y;
	}
	*x /= SAMPLE_SIZE;
	*y /= SAMPLE_SIZE;
}

void config_GPIOINT()
{
	LPC_GPIOINT->IO0IntEnF |= (1 << 19);
	NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler(void)
{
	int x, y;
	read_touch(&x, &y);
	if (!(x > 4000 || y < 100))
	{
		last_touch_x = x;
		last_touch_y = y;
		touched = true;
	}
	LPC_GPIOINT->IO0IntClr |= (1 << 19);
}

void clear_canvas()
{
	fill_background(LCDWhite);
	int colors[] = {LCDBlack, LCDRed, LCDGreen, LCDYellow, LCDBlue};
	int k = 0;
	for (int i = 0; i < 5; i++)
		draw_rectangle(i * 50, 0, 50, 50, colors[k++]);
}

void calibrate()
{
	//int x1, y1, x2, y2, x3, y3;
	int discard_x, discard_y;
	
	fill_background(LCDWhite);
	draw_cross(40, LCD_MAX_Y - 40, LCDBlack);
	wait_for_touch(&discard_x, &x1);
	
	fill_background(LCDWhite);
	draw_cross(LCD_MAX_X - 40, LCD_MAX_Y - 40, LCDBlack);
	wait_for_touch(&y1, &x2);
	
	fill_background(LCDWhite);
	draw_cross(LCD_MAX_X - 40, 40, LCDBlack);
	wait_for_touch(&y2, &discard_y);
	
	clear_canvas();
}

void touch_to_lcd(int *x, int *y)
{
	*x = (80 - LCD_MAX_X) * *x / (x1 - x2) + 40 - x1 * (80 - LCD_MAX_X) / (x1 - x2);
	*y = (80 - LCD_MAX_Y) * *y / (y1 - y2) + 40 - y1 * (80 - LCD_MAX_Y) / (y1 - y2);
}

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
	
	// rysowanie kwadratow i tekstu
	/*int colors[] = {LCDWhite, LCDBlack, LCDRed, LCDBlue, LCDYellow, LCDCyan, LCDMagenta, LCDGrey};
	int k = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 2; j++)
			draw_rectangle(i * 50, j * 50, 50, 50, colors[k++]);
	draw_string(0, 0, "Ladowanie watykanczyk.exe...", 0);*/
	
	calibrate();
	
	int x, y;
	int color = LCDBlack;
	//char s[2];
	
	while(1)
	{
		//wait(1);
		if (Buttons_GetState() == 1)
			clear_canvas();
		read_touch(&y, &x);
		touch_to_lcd(&x, &y);
		if (y > 50)
			//draw_point(x, LCD_MAX_Y - y, color);
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
		
		// wypisywanie dotknietych koordynatow x,y (trzeba tez odkomentowac char s[2] wyzej)
		/*read_touch(&x, &y);
		sprintf(s, "%d %d", x, y);
		fill_background(LCDYellow);
		draw_string(0, 0, s, 0);
		wait(1);*/
	}
}
