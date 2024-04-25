#ifndef __speed_H__
#define __speed_H__
#include "headfile.h"



/********速度全局变量***********/
extern unsigned char MOTOR_MODE;//请勿修改删除
extern int16 right_speed;     //右轮
extern int16 left_speed;      //左轮
extern int16 real_speed;      //实时速度
extern int16 aim_speed,Set_Speed;    //目标速度
extern int16 All_PWM_left;     //左轮PWM
extern int16 All_PWM_right;    //右轮PWM
extern int16 Speed_PWM;       //速度环
extern int16 Last_Speed_PWM;


extern int32 distance;  //车子所跑路程
extern float Speed_Pid[4];		// 速度环PID

/******速度函数声明*********/
void encoder_init(void);
void speed_measure(void);//测速

void init_PWM(unsigned char Motor_Set);
void go_motor (int16 left_PWM,int16 right_PWM);     //电机控制                            
float Speed_P_I_Control(float *PID, int16 speed, int16 set_speed);//因为角度变化小，需要是浮点型
#endif