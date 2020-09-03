#include "main.h"
#include "delay.h"
#include "move.h"
#include "tim.h"
#include <stdio.h>


char dir[6] = {0, 1, 2, 3, 4, 5};

void Step(unsigned short s)
{
	unsigned short i;
	while(s > 6400) s -= 6400;
	for(i = 0; i < s; i++)
	{
		HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_SET);
		delay_us(STEP_SPEED);
		HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_RESET);
		delay_us(STEP_SPEED);
	}
}

//读取固定电机现在的位置
char Get_HS_Sta()
{
	if(__HAL_TIM_GetCompare(&htim2, TIM_CHANNEL_1) == 110)
		return 0;
	return 1;
}

void Move(unsigned char k)
{
	unsigned char temp;
	switch(k)
	{
		case 0:
			HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
			Step(1600);
			if(!Get_HS_Sta())
			{
				temp = dir[0];
				dir[0] = dir[3];
				dir[3] = dir[5];
				dir[5] = dir[4];
				dir[4] = temp;
			}
			break;
			
		case 1:
			HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_SET);
			Step(1600);
			if(!Get_HS_Sta())
			{
				temp = dir[0];
				dir[0] = dir[4];
				dir[4] = dir[5];
				dir[5] = dir[3];
				dir[3] = temp;
			}
			break;
		case 2:
			HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
			Step(3200);
			if(!Get_HS_Sta())
			{
				temp = dir[0];
				dir[0] = dir[5];
				dir[5] = temp;
				temp = dir[3];
				dir[3] = dir[4];
				dir[4] = temp;
			}
			break;
		default:
			return;
	}
	
	//多转一点再回转，保证魔方底面旋转到位
	if(Get_HS_Sta())
	{
		Step(TURNBACK);
		HAL_GPIO_TogglePin(DIR_GPIO_Port, DIR_Pin);
		Step(TURNBACK);
	}
}

void Hold_Servo(char k)
{
	switch(k)
	{
		case 0:
			if(Get_HS_Sta())
			{
					__HAL_TIM_SetCompare(&htim2 ,TIM_CHANNEL_1, 110);
			}				
			break;
		case 1:
			if(!Get_HS_Sta())
			{
					__HAL_TIM_SetCompare(&htim2 ,TIM_CHANNEL_1, 205);
			}
			break;
	}
	delay_ms(500);
}

void Roll_Servo(char k)
{
	switch(k)
	{
		case 0:
			if(__HAL_TIM_GetCompare(&htim2, TIM_CHANNEL_3) == 105)
			{
				__HAL_TIM_SetCompare(&htim2 ,TIM_CHANNEL_3, 165);

			}
			break;
		case 1:
			if(__HAL_TIM_GetCompare(&htim2, TIM_CHANNEL_3) == 165)
			{
				__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 105);
			}
			break;
	}
}

void Sensor_Servo(char k)
{
	switch(k)
	{
		case 0:
			__HAL_TIM_SetCompare(&htim2 ,TIM_CHANNEL_2, 155);
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			break;
		case 1:
			__HAL_TIM_SetCompare(&htim2 ,TIM_CHANNEL_2, 125);
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
			break;
		case 2:
			__HAL_TIM_SetCompare(&htim2 ,TIM_CHANNEL_2, 119);
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
			break;
		case 3:
			__HAL_TIM_SetCompare(&htim2 ,TIM_CHANNEL_2, 102);
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
			break;
	}
	delay_ms(600);
}

void Step_Init(void)
{
	//舵机回位
	
	__HAL_TIM_SetCompare(&htim2 ,TIM_CHANNEL_1, 110);
	__HAL_TIM_SetCompare(&htim2 ,TIM_CHANNEL_2, 155);
	__HAL_TIM_SetCompare(&htim2 ,TIM_CHANNEL_3, 165);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);

	delay_ms(500);
	
	
	//步进电机角度校正
	HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
	if(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)Step(3000);
	while(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) != GPIO_PIN_RESET)
	{
		HAL_GPIO_TogglePin(STEP_GPIO_Port, STEP_Pin);
		delay_us(STEP_SPEED);
	}
	Step(STEP_BASE);
}

void Roll(void)
{
	unsigned char temp;
	
	Roll_Servo(1);
	delay_ms(300);
	Roll_Servo(0);
	delay_ms(500);
	
	temp = dir[0];
	dir[0] = dir[1];
	dir[1] = dir[5];
	dir[5] = dir[2];
	dir[2] = temp;
}

char Side_Down(char side)
{
	char i, j = 6;
	for(i = 0; i<6; i++)
	{
		if("FUDLRB"[dir[i]] == side)
		{
			j = i;
			break;
		}
	}
	
	switch(j)
	{
		case 0:
			Hold_Servo(0);
			Roll();
			break;
		
		case 1:
			Hold_Servo(0);
			Roll();
			Roll();
			break;
		
		case 2:
			break;
		
		case 3:
			Hold_Servo(0);
			Move(0);
			Roll();
			break;
		
		case 4:
			Hold_Servo(0);
			Move(1);
			Roll();
			break;
		
		case 5:
			Hold_Servo(0);
			Move(2);
			Roll();
			break;
		default:
			return 1;
	}
	return 0;
}

char Side_Turn(char n)
{
	Hold_Servo(1);
	switch(n)
	{
		case '1':
			Move(0);
			break;
		case '2':
			Move(2);
			break;
		case '3':
			Move(1);
			break;
		default:
			return 1;
	}
	return 0;
}

char Cube_Restore(char* res)
{
	char temp;
	
	//以现在状态为初始状态
	for(temp = 0; temp < 6; temp++)
	{
		dir[temp] = temp;
	}
	while(*res)
	{
		if(Side_Down(*res++))return 1;
		if(Side_Turn(*res++))return 1;
	}
	Hold_Servo(0);
	return 0;
}



