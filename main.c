#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "TP_Open1768.h"

#include "lcdDrawing.h"
#include "lcdTouch.h"
#include "sound.h"
#include "win.h"
#include "snake.h"
//#include "lose.h"


int main()
{ 
	config_GPIOINT();
	config_sound();
	lcdConfiguration();
	init_ILI9325();
	touchpanelInit();
	calibrate();
	
	for (int i = 0; i < GRID_SIZE_X; i++)
		for (int j = 0; j < GRID_SIZE_Y; j++)
			grid[i][j] = 0;
	
	// Starting snake position
	grid[snake_head_x][snake_head_y] = 5;
	grid[snake_head_x][snake_head_y + 1] = 4;
	grid[snake_head_x][snake_head_y + 2] = 3;
	grid[snake_head_x][snake_head_y + 3] = 2;
	grid[snake_head_x][snake_head_y + 4] = 1;
	
	config_Timer();
	
	//play_sound(win, WIN_LEN);
	//play_sound(lose, LOSE_LEN);
	while(1);
}
