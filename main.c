#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "TP_Open1768.h"

#include "lcdDrawing.h"
#include "lcdTouch.h"

// Cell:     Apple:
// #########         /|\ /|\ Apple offset
// #########          |  \|/
// #########  #####   |
// #########  #####   |
// #########  #####   | Cell size
// #########  #####   |
// #########  #####   |
// #########          |
// #########         \|/
#DEFINE CELL_SIZE 9
#DEFINE APPLE_OFFSET 2

int snake_head_x = lcd_to_snake(LCD_MAX_X / 2);
int snake_head_y = lcd_to_snake(LCD_MAX_Y / 2);

// Convert LCD coords to Snake coords
int lcd_to_snake(int x, int *y)
{
	return x / CELL_SIZE;
}

// Convert Snake coords to LCD coords
int snake_to_lcd(int x)
{
	return x * CELL_SIZE;
}

// Draw a body part of Snake from Snake coords
void draw_snake_part(int x, int y)
{
	draw_rectangle(snake_to_lcd(x), snake_to_lcd(y), CELL_SIZE, CELL_SIZE, LCDBlack);
}

// Draw an apple from Snake coords
void draw_apple(int x, int y)
{
	draw_rectangle(snake_to_lcd(x) + APPLE_OFFSET, snake_to_lcd(y) + APPLE_OFFSET, CELL_SIZE - 2 * APPLE_OFFSET, CELL_SIZE - 2 * APPLE_OFFSET, LCDBlack);
}

void config_Timer()
{
	LPC_TIM0->PR = 0;
	LPC_TIM0->MR0 = SystemCoreClock / 8000;
	LPC_TIM0->MCR = 3;
	LPC_TIM0->TCR = 1;
	NVIC_EnableIRQ(TIMER0_IRQn);
}

int touch_x, touch_y;

void TIMER0_IRQHandler(void)
{
	touch_x = last_touch_x;
	touch_y = last_touch_y;
	touch_to_lcd(&touch_x, &touch_y);
	touch_x = lcd_to_snake(touch_x);
	touch_y = lcd_to_snake(touch_y);
	
	if (touch_x < snake_head_x && touch_y > snake_head_y - touch_x && touch_y < snake_head_y + touch_x)
	{
		// move left
	}
	else if (touch_x > snake_head_x && touch_y > snake_head_y - (touch_x - snake_head_x) && touch_y < snake_head_y + (touch_x - snake_head_x))
	{
		// move right
	}
	else if (touch_y < snake_head_y)
	{
		// move up
	}
	else if (touch_y > snake_head_y)
	{
		// move down
	}
	
	LPC_TIM0->IR = 1;
}

void config()
{
	lcdConfiguration();
	init_ILI9325();
	touchpanelInit();
	calibrate();
	config_Timer();
}

int main()
{ 
	config();
	
	while(1);
}
