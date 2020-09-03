#ifndef __MOVE_H
#define  __MOVE_H

#define STEP_BASE 1010		//����΢�����غ������ת�Ĳ���
#define STEP_SPEED 150		//�������ÿ���������ʱ
#define TURNBACK 210		//ת������ʱ����ת�Ĳ�������֤ħ��������ת��λ

/**
 *���ݽⷨ��ԭħ��
 */
char Cube_Restore(char* res);

/**
 *�����λ
 */
void Step_Init(void);

/**
 *�������ת��
 *0:˳ʱ��ת90��   1����ʱ��ת90��    2��˳ʱ��ת180��
 */
void Move(unsigned char k);

/**
 *ħ������90��
 */
void Roll(void);

/**
 *�������ת��һ���Ƕ�
 *1.8�Ȳ���ǣ�32ϸ�֣�һȦ����6400��
 */
void Step(unsigned short s);
	
/**
 *��ɫ������λ���ƶ�
 *0:��λ		1:����ǿ�		2:�������		3:��������
 */
void Sensor_Servo(char k);

#endif
