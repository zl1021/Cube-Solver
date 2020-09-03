#include "lcd.h"
#include "i2c.h"
#include "font.h"
#include "main.h"
#include "delay.h"

#define CMD_X 0xD0
#define CMD_Y 0x90
#define SAVE_ADDR_BASE 40
#define ADDR_Write 0xA0
#define ADDR_Read 0xA1

unsigned short POINT_X, POINT_Y;
float xfac, yfac;
unsigned short xoff, yoff;
unsigned char TS_STA = 0;


unsigned short BACK_COLOR = WHITE;
unsigned short POINT_COLOR = BLACK;



void LCD_SetCursor(unsigned short X, unsigned short Y)
{	     
		WR_CMD(0x2A); 
		WR_DATA(X>>8);WR_DATA(X&0XFF); 			 
		WR_CMD(0x2B); 
		WR_DATA(Y>>8);WR_DATA(Y&0XFF); 		
	
} 	


void LCD_SetWindow(unsigned short x,unsigned short w,unsigned short y,unsigned short h)
{
	w=w+x-1;
	h=h+y-1;
	WR_CMD(0x2A); 
	WR_DATA(x>>8);WR_DATA(x&0XFF); 
	WR_DATA(w>>8);WR_DATA(w&0XFF); 	
	WR_CMD(0x2B); 
	WR_DATA(y>>8);WR_DATA(y&0XFF); 
	WR_DATA(h>>8);WR_DATA(h&0XFF); 
}


void LCD_ClearWindow(unsigned short x,unsigned short w,unsigned short y,unsigned short h,unsigned short color)
{
	uint32_t i=w*h;
	LCD_SetWindow(x,w,y,h);
	WR_CMD(0x2C);
	for(;i>0;i--) WR_DATA(color);
}



void LCD_Clear(unsigned short color)
{
	LCD_ClearWindow(0,240,0,320,color);
}  
  
void LCD_ShowChar(unsigned short x,unsigned short y,unsigned char h,unsigned char c)
{
	unsigned char i;
	unsigned char j;
	unsigned char temp;
	unsigned char* p;
	if(h==64)
	{
		i=64;
		j=32;
		p = (unsigned char*)asc2_6432[c-' '];
	}
	else if(h==48)
	{
		i=48;
		j=24;
		p = (unsigned char*)asc2_4824[c-' '];
	}
	else
		return;
	if((c>125)||(c<32))
	{
	LCD_ClearWindow(x,j,y,i,BACK_COLOR);
	return;
	}
	LCD_SetWindow(x,j,y,i);
	WR_CMD(0x2C);
	for(;i;i--)
	{
		for(unsigned char m=j/8;m;m--)
		{
			temp=*p;
			for(unsigned char k=8;k;k--)
			{
				if(temp&1)
				{
					WR_DATA(POINT_COLOR);
				}
				else
				{
					WR_DATA(BACK_COLOR);
				}
				temp=temp>>1;
			}
		p++;
		}
	}
}

void LCD_ShowString(unsigned short x,unsigned short y,unsigned char h,unsigned char* s,unsigned char l)
{
	if(y+h>320)return;
	for(;l;l--)
	{
		if((x+h/2)>240)
		{
			x=0;
			y+=h;
			if(y+h>320)return;
		}
		LCD_ShowChar(x,y,h,*s);
		x=h/2+x;
		
		s++;
	}
}

unsigned short TS_Readval(char cmd)
{
	unsigned short count=0, num=0; 
	HAL_GPIO_WritePin(TS_CLK_GPIO_Port, TS_CLK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(TS_DIN_GPIO_Port, TS_DIN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(TS_CS_GPIO_Port, TS_CS_Pin, GPIO_PIN_RESET);

	for(count = 0; count < 8; count++)
	{
		if(cmd &0x80)
		{
			HAL_GPIO_WritePin(TS_DIN_GPIO_Port, TS_DIN_Pin, GPIO_PIN_SET);
		}			
		else
		{
			HAL_GPIO_WritePin(TS_DIN_GPIO_Port, TS_DIN_Pin, GPIO_PIN_RESET);
		}
		cmd <<= 1;    
		HAL_GPIO_WritePin(TS_CLK_GPIO_Port, TS_CLK_Pin, GPIO_PIN_RESET); 
		delay_us(1);
		HAL_GPIO_WritePin(TS_CLK_GPIO_Port, TS_CLK_Pin, GPIO_PIN_SET);
	}
	delay_us(6);
	HAL_GPIO_WritePin(TS_CLK_GPIO_Port, TS_CLK_Pin, GPIO_PIN_RESET); 	     	    
	delay_us(1);    	   
	HAL_GPIO_WritePin(TS_CLK_GPIO_Port, TS_CLK_Pin, GPIO_PIN_SET); 
	delay_us(1);
	HAL_GPIO_WritePin(TS_CLK_GPIO_Port, TS_CLK_Pin, GPIO_PIN_RESET);

	
	for(count=0;count<16;count++)
	{ 				  
		num<<=1; 	 
		HAL_GPIO_WritePin(TS_CLK_GPIO_Port, TS_CLK_Pin, GPIO_PIN_RESET); 
		delay_us(1);    
 		HAL_GPIO_WritePin(TS_CLK_GPIO_Port, TS_CLK_Pin, GPIO_PIN_SET); 
 		if(HAL_GPIO_ReadPin(TS_DOUT_GPIO_Port, TS_DOUT_Pin))num++;
	}  	
	num>>=4;
	HAL_GPIO_WritePin(TS_CS_GPIO_Port, TS_CS_Pin, GPIO_PIN_SET);
	return num; 
}

unsigned short TS_ReadXY(char cmd)
{
	unsigned char i, j;
	unsigned short val[5], temp, sum = 0;
	for(i = 0; i < 5; i++)
	{
		val[i] = TS_Readval(cmd);
	}
	for(i = 0; i < 4; i++)
	{
		for(j = i + 1; j < 5; j++)
		{
			if(val[i] > val[j])
			{
				temp = val[i];
				val[i] = val[j];
				val[j] = temp;
			}
		}
	}	  
	sum = val[1] + val[2] + val[3];
	return sum/3; 
}

void TS_GetXY(unsigned short *x, unsigned short *y)
{
	*x = TS_ReadXY(CMD_X);
	*y = TS_ReadXY(CMD_Y);
}

int TS_Scan(void)
{
	if(!HAL_GPIO_ReadPin(TS_PEN_GPIO_Port, TS_PEN_Pin))
	{
		if(!TS_STA)
		{
			TS_STA = 1;
			POINT_X =240 - (TS_ReadXY(CMD_X) * xfac - xoff);
			POINT_Y =320 - (TS_ReadXY(CMD_Y) * yfac - yoff);
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		TS_STA = 0;
		return 0;
	}
}




void TS_Memwrite(unsigned short addr, unsigned int data, unsigned char len)
{
	unsigned char i;
	unsigned char temp;
	for(i = 0; i < len; i++)
	{
		temp = (data >> (i*8)) & 0xff; 
		HAL_I2C_Mem_Write(&hi2c1, ADDR_Write, addr + i, I2C_MEMADD_SIZE_8BIT, &temp, 1, 0x100);
		HAL_Delay(5);
	}
}

unsigned int TS_Memread(unsigned short addr, unsigned char len)
{
	if(len > 4)return 0;
	unsigned char i;
	unsigned int ret = 0;
	unsigned char temp;
	for(i = 0; i < len; i++)
	{
		ret <<= 8;
		HAL_I2C_Mem_Read(&hi2c1, ADDR_Read, addr + len - 1 - i, I2C_MEMADD_SIZE_8BIT, &temp, 1, 0x100);
		HAL_Delay(5);
		ret += temp;
	}
	return ret;
}


void TS_Saveadjdata(void)
{
	int temp;					  
	temp = xfac * 100000000;
	TS_Memwrite(SAVE_ADDR_BASE,temp,4);   
	temp = yfac * 100000000; 
  TS_Memwrite(SAVE_ADDR_BASE+4,temp,4);
	TS_Memwrite(SAVE_ADDR_BASE+8,xoff,2);		    
	TS_Memwrite(SAVE_ADDR_BASE+10,yoff,2);	
	temp=0X01;       /*校准完成标记*/
	TS_Memwrite(SAVE_ADDR_BASE+12,temp,1); 
	
}



unsigned char TS_Getadjdata(void)
{					  
	int temp;
	temp = TS_Memread(SAVE_ADDR_BASE + 12, 1);
//	printf("%d\r\n",temp);
	if(temp == 0X01)		   
	{    												 
		temp = TS_Memread(SAVE_ADDR_BASE, 4);		 	
		xfac = (float)temp / 100000000;
		temp = TS_Memread(SAVE_ADDR_BASE + 4, 4);			          
		yfac = (float)temp / 100000000;
		xoff = TS_Memread(SAVE_ADDR_BASE + 8, 2);			   	  
		yoff = TS_Memread(SAVE_ADDR_BASE + 10, 2);				   
		return 1;	 
	}
	return 0;
}	 

void TS_Adjust(void)
{								 
	unsigned short temp[2][2];
	LCD_Clear(BLACK);
	LCD_ClearWindow(20,1,10,21,WHITE);
	LCD_ClearWindow(10,21,20,1,WHITE);
	while(HAL_GPIO_ReadPin(TS_PEN_GPIO_Port, TS_PEN_Pin));
	TS_GetXY(temp[0], temp[0] + 1);
	LCD_ClearWindow(20,1,10,21,BLACK);
	LCD_ClearWindow(10,21,20,1,BLACK);
	
	LCD_ClearWindow(220,1,290,21,WHITE);
	LCD_ClearWindow(210,21,300,1,WHITE);
	HAL_Delay(1000);
	while(HAL_GPIO_ReadPin(TS_PEN_GPIO_Port, TS_PEN_Pin));
	TS_GetXY(temp[1], temp[1] + 1);
	
	LCD_Clear(BACK_COLOR);
	
	xfac=(float)200/(temp[0][0]-temp[1][0]);
	yfac=(float)280/(temp[0][1]-temp[1][1]);
	xoff=temp[1][0]*xfac - 20;
	yoff=temp[1][1]*yfac - 20;
	
	TS_Saveadjdata();
}

void TS_Init(void)
{
	if(TS_Getadjdata())
	{
		return;
	}
	TS_Adjust();
}

void LCD_Init(void)
{
	WR_CMD(0xCF);  
	WR_DATA(0x00); 
	WR_DATA(0xC1); 
	WR_DATA(0X30); 
	WR_CMD(0xED);  
	WR_DATA(0x64); 
	WR_DATA(0x03); 
	WR_DATA(0X12); 
	WR_DATA(0X81); 
	WR_CMD(0xE8);  
	WR_DATA(0x85); 
	WR_DATA(0x10); 
	WR_DATA(0x7A); 
	WR_CMD(0xCB);  
	WR_DATA(0x39); 
	WR_DATA(0x2C); 
	WR_DATA(0x00); 
	WR_DATA(0x34); 
	WR_DATA(0x02); 
	WR_CMD(0xF7);  
	WR_DATA(0x20); 
	WR_CMD(0xEA);  
	WR_DATA(0x00); 
	WR_DATA(0x00); 
	WR_CMD(0xC0);    //Power control 
	WR_DATA(0x1B);   //VRH[5:0] 
	WR_CMD(0xC1);    //Power control 
	WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
	WR_CMD(0xC5);    //VCM control 
	WR_DATA(0x30); 	 //3F
	WR_DATA(0x30); 	 //3C
	WR_CMD(0xC7);    //VCM control2 
	WR_DATA(0XB7); 
	WR_CMD(0x36);    // Memory Access Control 
	WR_DATA(0xC8); 
	WR_CMD(0x3A);   
	WR_DATA(0x55); 
	WR_CMD(0xB1);   
	WR_DATA(0x00);   
	WR_DATA(0x1A); 
	WR_CMD(0xB6);    // Display Function Control 
	WR_DATA(0x0A); 
	WR_DATA(0xA2); 
	WR_CMD(0xF2);    // 3Gamma Function Disable 
	WR_DATA(0x00); 
	WR_CMD(0x26);    //Gamma curve selected 
	WR_DATA(0x01); 
	WR_CMD(0xE0);    //Set Gamma 
	WR_DATA(0x0F); 
	WR_DATA(0x2A); 
	WR_DATA(0x28); 
	WR_DATA(0x08); 
	WR_DATA(0x0E); 
	WR_DATA(0x08); 
	WR_DATA(0x54); 
	WR_DATA(0XA9); 
	WR_DATA(0x43); 
	WR_DATA(0x0A); 
	WR_DATA(0x0F); 
	WR_DATA(0x00); 
	WR_DATA(0x00); 
	WR_DATA(0x00); 
	WR_DATA(0x00); 		 
	WR_CMD(0XE1);    //Set Gamma 
	WR_DATA(0x00); 
	WR_DATA(0x15); 
	WR_DATA(0x17); 
	WR_DATA(0x07); 
	WR_DATA(0x11); 
	WR_DATA(0x06); 
	WR_DATA(0x2B); 
	WR_DATA(0x56); 
	WR_DATA(0x3C); 
	WR_DATA(0x05); 
	WR_DATA(0x10); 
	WR_DATA(0x0F); 
	WR_DATA(0x3F); 
	WR_DATA(0x3F); 
	WR_DATA(0x0F); 
	WR_CMD(0x2B); 
	WR_DATA(0x00);
	WR_DATA(0x00);
	WR_DATA(0x01);
	WR_DATA(0x3f);
	WR_CMD(0x2A); 
	WR_DATA(0x00);
	WR_DATA(0x00);
	WR_DATA(0x00);
	WR_DATA(0xef);	 
	WR_CMD(0x11); //Exit Sleep
	HAL_Delay(120);
	WR_CMD(0x29); //display on
	LCD_Clear(BACK_COLOR);
	HAL_GPIO_WritePin(BL_GPIO_Port,BL_Pin,GPIO_PIN_SET);
	
	TS_Init();

}
