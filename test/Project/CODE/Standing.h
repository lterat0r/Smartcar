#ifndef _Standing_H
#define _Standing_H
#include "headfile.h"


extern uint16  K_P;
extern uint16 K_D ; 

/********函数声明*********/
void Complementary_Filter();
void datasend();
/*********全局变量声明********/
extern int16  Angle_pwm ;
extern float Angle;
extern float  AngleIntegral;
extern int16 aim_angle;//目标角度



#endif

