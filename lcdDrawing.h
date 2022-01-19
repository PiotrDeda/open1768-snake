#ifndef __LcdDrawing_H
#define __LcdDrawing_H

#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"

void reset_ram()
{
	lcdWriteReg(HADRPOS_RAM_START, 0);
	lcdWriteReg(HADRPOS_RAM_END, LCD_MAX_X);
	
	lcdWriteReg(VADRPOS_RAM_START, 0);
	lcdWriteReg(VADRPOS_RAM_END, LCD_MAX_Y);
	
	lcdSetCursor(0, 0);
}

void draw_rectangle(int pos_x, int pos_y, int size_x, int size_y, int color)
{
	lcdWriteReg(HADRPOS_RAM_START, pos_x);
	lcdWriteReg(HADRPOS_RAM_END, pos_x + size_x - 1);
	
	lcdWriteReg(VADRPOS_RAM_START, pos_y);
	lcdWriteReg(VADRPOS_RAM_END, pos_y + size_y - 1);
	
	lcdSetCursor(pos_x, pos_y);
	lcdWriteIndex(DATA_RAM);
	for (int i = 0; i < size_x * size_y; i++)
	{
			lcdWriteData(color);
	}
	
	reset_ram();
}

void draw_point(int x, int y, int color)
{
	lcdSetCursor(LCD_MAX_X - x, LCD_MAX_Y - y);
	lcdWriteReg(DATA_RAM, color);
}

void fill_background(int color)
{
	lcdSetCursor(0, 0);
	lcdWriteIndex(DATA_RAM);
	for (int i = 0; i < LCD_MAX_X * LCD_MAX_Y; i++)
			lcdWriteData(color);
}

void draw_cross(int x, int y, int color)
{
	for (int i = x - 10; i < x + 11; i++)
		draw_point(i, y, color);
	for (int i = y - 10; i < y + 11; i++)
		draw_point(x, i, color);
}

#endif
