#ifndef __DISPLAY_H
#define __DISPLAY_H

/**
 *在显示屏上更新整个魔方的颜色
 */
void Cube_Display(char cube[6][9]);

/**
 *在显示屏上更新魔方1个面的颜色
 */
void display_1side(char side, char color[9]);

/**
 *更新按钮状态
 */
void Button_Display(unsigned char b);

#endif

