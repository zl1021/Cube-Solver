#ifndef __MOVE_H
#define  __MOVE_H

#define STEP_BASE 1010		//触发微动开关后继续旋转的步数
#define STEP_SPEED 150		//步进电机每个脉冲的延时
#define TURNBACK 210		//转动底面时多旋转的步数，保证魔方底面旋转到位

/**
 *根据解法还原魔方
 */
char Cube_Restore(char* res);

/**
 *电机归位
 */
void Step_Init(void);

/**
 *步进电机转动
 *0:顺时针转90度   1：逆时针转90度    2：顺时针转180度
 */
void Move(unsigned char k);

/**
 *魔方翻滚90度
 */
void Roll(void);

/**
 *步进电机转动一定角度
 *1.8度步距角，32细分，一圈等于6400步
 */
void Step(unsigned short s);
	
/**
 *颜色传感器位置移动
 *0:归位		1:对齐角块		2:对齐棱块		3:对齐中心
 */
void Sensor_Servo(char k);

#endif
