#ifndef __LCD_H
#define __LCD_H



#define WHITE          0xFFFF
#define BLACK          0x0000
#define BLUE           0x001F
#define RED            0xF800
#define MAGENTA        0xF81F
#define ORANGE         0xFD00
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0


	
#define WR_CMD(x) *(volatile unsigned short *)0x6C00007e = x
#define WR_DATA(x) *(volatile unsigned short *)0x6C000080 = x

extern unsigned short POINT_X;
extern unsigned short POINT_Y;
extern unsigned short BACK_COLOR;
extern unsigned short POINT_COLOR;
extern unsigned char TS_STA;

void LCD_SetWindow(unsigned short x,unsigned short w,unsigned short y,unsigned short h);
void LCD_Clear(unsigned short color);
void LCD_Init(void);
void LCD_ClearWindow(unsigned short x,unsigned short w,unsigned short y,unsigned short h,unsigned short color);
void LCD_ShowChar(unsigned short x,unsigned short y,unsigned char h,unsigned char c);
void LCD_ShowString(unsigned short x,unsigned short y,unsigned char h,unsigned char* s,unsigned char l);

void TS_Adjust(void);
int TS_Scan(void);


#endif
