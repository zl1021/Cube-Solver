#ifndef __COLOR_H
#define __COLOR_H

#define CUBE_COL_BLUE 0
#define CUBE_COL_YELLOW 1
#define CUBE_COL_WHITE 2
#define CUBE_COL_ORANGE 3
#define CUBE_COL_RED 4
#define CUBE_COL_GREEN 5

/**
 *ɨ��ħ����ɫ����������cube������ʽ��Ϊ��UF UR UB UL  DF DR DB DL  FR FL BR BL  UFR URB UBL ULF   DRF DFL DLB DBR���ĸ�ʽ
 */
int Getcolor(char cube[6][9],char col[20][3]);

#endif
