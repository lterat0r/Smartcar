#ifndef _Standing_H
#define _Standing_H
#include "headfile.h"


extern uint16  K_P;
extern uint16 K_D ; 

/********��������*********/
void Complementary_Filter();
void datasend();
/*********ȫ�ֱ�������********/
extern int16  Angle_pwm ;
extern float Angle;
extern float  AngleIntegral;
extern int16 aim_angle;//Ŀ��Ƕ�



#endif

