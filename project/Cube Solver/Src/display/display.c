#include "lcd.h"

unsigned short display_col(char col)
{
	switch(col)
	{
		case 0:
			return BLUE;
		case 1:
			return YELLOW;
		case 2:
			return WHITE;
		case 3:
			return ORANGE;
		case 4:
			return RED;
		case 5:
			return GREEN;
		default:
			return 0;
	}
}

void display_1piece(unsigned short xbase, unsigned short ybase, char i, char color)
{
	unsigned short x, y;
	x = i % 3 * 17 + xbase;
	y = i / 3 * 17 + ybase;
	LCD_ClearWindow(x, 18, y, 18, BLACK);
	LCD_ClearWindow(x+ 1, 16, y + 1, 16, display_col(color));
}

void display_1side(char side, char color[9])
{
	unsigned short xbase, ybase;
	unsigned char i;
	switch(side)
	{
		case 0:
			xbase = 69;
			ybase = 81;
			break;
		case 1:
			xbase = 69;
			ybase = 30;
			break;
		case 2:
			xbase = 69;
			ybase = 132;
			break;
		case 3:
			xbase = 18;
			ybase = 81;
			break;
		case 4:
			xbase = 120;
			ybase = 81;
			break;
		case 5:
			xbase = 171;
			ybase = 81;
			break;
	}
	for(i = 0; i < 9; i++)
	{
		display_1piece(xbase, ybase, i, color[i]);
	}
}

void Cube_Display(char cube[6][9])
{
	unsigned char i;
	for(i = 0; i < 6; i++)
	{
		display_1side(i ,cube[i]);
	}
}

void Button_Display(unsigned char b)
{
	BACK_COLOR = CYAN;
	POINT_COLOR = BLACK;
	LCD_ClearWindow(24, 192, 215, 70, BACK_COLOR);
	if(b == 0)
		LCD_ShowString(60, 226, 48, (unsigned char*)"START",5);
	else if(b == 1)
		LCD_ShowString(36, 226, 48, (unsigned char*)"RUNNING",7);
	else if(b == 2)
		LCD_ShowString(60, 226, 48, (unsigned char*)"ERROR",5);
}







