#include "color.h"
#include "main.h"
#include "delay.h"
#include "tim.h"
#include "display.h"
#include "move.h"
#include <stdio.h>

#define COL_FORMAT(a, b, c, d) col[a][b]="FUDLRB"[direction[cube[c][d]]]
#define SCAN_RED 0
#define SCAN_GREEN 1
#define SCAN_BLUE 2


unsigned short scancol;
char direction[6];

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	static char sta = 0;		//第一次捕获：sta置1，数据丢弃			第二次捕获：sta清零，数据保存并关闭中断
	
	if(htim->Instance == htim3.Instance)
	{
		if(sta == 0)
		{
			__HAL_TIM_SetCounter(htim, 0);
			sta = 1;
		}
		else if(sta == 10)
		{
			scancol = __HAL_TIM_GetCounter(htim);
			HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_1);
			sta = 0;
		}
		else
		{
			sta++;
		}
	}
}

unsigned short scan_1color(char color)
{
	scancol = 0;
	if(color == SCAN_RED)
	{
		HAL_GPIO_WritePin(S2_GPIO_Port,S2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(S3_GPIO_Port,S3_Pin,GPIO_PIN_RESET);
	}
	if(color == SCAN_GREEN)
	{
		HAL_GPIO_WritePin(S2_GPIO_Port,S2_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(S3_GPIO_Port,S3_Pin,GPIO_PIN_SET);
	}
	if(color == SCAN_BLUE)
	{
		HAL_GPIO_WritePin(S2_GPIO_Port,S2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(S3_GPIO_Port,S3_Pin,GPIO_PIN_SET);
	}
	delay_ms(5);

	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
	delay_ms(20);

	return  scancol;
}

char scan_1piece(void)
{
	unsigned short r, g, b;

	r = scan_1color(SCAN_RED);
	g = scan_1color(SCAN_GREEN);
	b = scan_1color(SCAN_BLUE);
	printf("r=%5d   g=%5d   b=%5d\r\n",r,g,b);		//打印数值，方便调试
	
/**
 *根据魔方的不同，需要调整不同颜色的判定条件
 */
	if(r > 4500 && g > 3500 && b < 3500)return 0;		//蓝色
	if(g > 5000 && b > 4500)return 4;		//红色
	if(r > 2500 && b > 2500)return 5;	//绿色	
	if(r > 1500 && g < 3500 && b < 3000)return 2;		//白色
	if(r < 1400 && g > 3000 && b > 3200)return 3;		//橙色	
	if(r > 1200 && g < 3000 && b < 4300)return 1;		//黄色

	return 6;
}

char scan_1side(char i, char side[9])
{
	unsigned char j;
	HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_SET);
	Sensor_Servo(3);
	delay_ms(400);
	side[4] = scan_1piece();
	
	Sensor_Servo(2);
	Step(150);
	side[5] = scan_1piece();
	
	Sensor_Servo(1);
	Step(800);
	side[2] = scan_1piece();
	
	
	Sensor_Servo(2);
	Step(800);
	side[1] = scan_1piece();

	
	Sensor_Servo(1);
	Step(800);
	side[0] = scan_1piece();

	
	Sensor_Servo(2);
	Step(800);
	side[3] = scan_1piece();
	
	
	Sensor_Servo(1);
	Step(800);
	side[6] = scan_1piece();

	
	Sensor_Servo(2);
	Step(800);
	side[7] = scan_1piece();

	
	Sensor_Servo(1);
	Step(800);
	side[8] = scan_1piece();

	
	Sensor_Servo(0);
	Step(650);
	
	for(j = 0; j < 9; j++)
	{
		printf("%c","FUDLRB "[side[j]]);
	}
	
	printf("\r\n");
	display_1side(i, side);
	for(j = 0; j < 9; j++)
	{
		if(side[j] == 6)return 1;
	}
	return 0;
}

char scan_color(char cube[6][9])
{
	int i;
	
	if(scan_1side(1, cube[1]))return 1;
	
	Roll();
	Move(2);
	delay_ms(200);
	if(scan_1side(5, cube[5]))return 1;
	
	Move(1);
	Roll();
	Move(0);
	delay_ms(200);
	if(scan_1side(4, cube[4]))return 1;
	
	Move(1);
	Roll();
	Move(0);
	delay_ms(200);
	if(scan_1side(0, cube[0]))return 1;
	
	Move(1);
	Roll();
	Move(0);
	delay_ms(200);
	if(scan_1side(3, cube[3]))return 1;
	
	Move(2);
	Roll();
	Move(1);
	delay_ms(200);
	if(scan_1side(2, cube[2]))return 1;
	
	Roll();
	Roll();

	for(i=0;i<6;i++)
	{
		direction[cube[i][4]]=i;
	}
	return 0;
}

char Checkcolor(char cube[6][9])
{
	char i, j, n[6] = {0};
	for(i = 0; i < 6; i++)
	{
		for(j = 0; j < 9; j++)
		{
			n[cube[i][j]] = n[cube[i][j]] + 1;
		}
	}
	for(i = 0; i < 6; i++)
	{
		if(n[i] != 9)return 1;
	}
	return 0;
	
}

int Getcolor(char cube[6][9],char col[20][3])
{	
	
	if(scan_color(cube))return 1;

	COL_FORMAT(0,0,1,7);    COL_FORMAT(0,1,0,1);
	COL_FORMAT(1,0,1,5);    COL_FORMAT(1,1,4,1);
	COL_FORMAT(2,0,1,1);    COL_FORMAT(2,1,5,1);
	COL_FORMAT(3,0,1,3);    COL_FORMAT(3,1,3,1);
	COL_FORMAT(4,0,2,1);    COL_FORMAT(4,1,0,7);
	COL_FORMAT(5,0,2,5);    COL_FORMAT(5,1,4,7);
	COL_FORMAT(6,0,2,7);    COL_FORMAT(6,1,5,7);
	COL_FORMAT(7,0,2,3);    COL_FORMAT(7,1,3,7);
	COL_FORMAT(8,0,0,5);    COL_FORMAT(8,1,4,3);
	COL_FORMAT(9,0,0,3);    COL_FORMAT(9,1,3,5);
	COL_FORMAT(10,0,5,3);   COL_FORMAT(10,1,4,5);
	COL_FORMAT(11,0,5,5);   COL_FORMAT(11,1,3,3);
	COL_FORMAT(12,0,1,8);   COL_FORMAT(12,1,0,2);   COL_FORMAT(12,2,4,0);
	COL_FORMAT(13,0,1,2);   COL_FORMAT(13,1,4,2);   COL_FORMAT(13,2,5,0);
	COL_FORMAT(14,0,1,0);   COL_FORMAT(14,1,5,2);   COL_FORMAT(14,2,3,0);
	COL_FORMAT(15,0,1,6);   COL_FORMAT(15,1,3,2);   COL_FORMAT(15,2,0,0);
	COL_FORMAT(16,0,2,2);   COL_FORMAT(16,1,4,6);   COL_FORMAT(16,2,0,8);
	COL_FORMAT(17,0,2,0);   COL_FORMAT(17,1,0,6);   COL_FORMAT(17,2,3,8);
	COL_FORMAT(18,0,2,6);   COL_FORMAT(18,1,3,6);   COL_FORMAT(18,2,5,8);
	COL_FORMAT(19,0,2,8);   COL_FORMAT(19,1,5,6);   COL_FORMAT(19,2,4,8);
	return 0;
}




