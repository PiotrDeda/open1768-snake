#ifndef __Snake_H
#define __Snake_H

#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "TP_Open1768.h"

#include "lcdDrawing.h"
#include "lcdTouch.h"
#include "sound.h"
#include "win.h"
#include "lose.h"

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
#define CELL_SIZE 9
#define APPLE_OFFSET 2
#define GRID_SIZE_X (LCD_MAX_X / 8)
#define GRID_SIZE_Y (LCD_MAX_Y / 8)
#define SAMPLE_SIZE_INGAME 8

enum MovementDirection {LEFT, RIGHT, UP, DOWN};
int snake_head_x = (LCD_MAX_X / 2) / CELL_SIZE;
int snake_head_y = (LCD_MAX_Y / 2) / CELL_SIZE;
int snake_size = 5;
short grid[GRID_SIZE_X][GRID_SIZE_Y];
int areYouWinningSon = 1;
int isAnAppleOnTheScreen = 0;
int appleX, appleY;
int touch_x, touch_y;
enum MovementDirection direction = LEFT;

int rand(int a, int b, int m){
    return (a*b)%m;
}

// Convert LCD coords to Snake coords
int lcd_to_snake(int x)
{
	return x / CELL_SIZE;
}

// Convert Snake coords to LCD coords
int snake_to_lcd(int x)
{
	return x * CELL_SIZE;
}

// Draw a body part of Snake from Snake coords
void draw_snake_part(int x, int y, int color)
{
	draw_rectangle(snake_to_lcd(x), snake_to_lcd(y), CELL_SIZE, CELL_SIZE, color);
}

// Draw an apple from Snake coords
void draw_apple(int seed1, int seed2);

void config_Timer();

void move_snake(enum MovementDirection direction);

void TIMER0_IRQHandler(void);

#endif