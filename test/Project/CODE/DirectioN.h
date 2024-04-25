#ifndef __DirectioN_H__
#define __DirectioN_H__
#include "headfile.h"


//舵机相关值
#define Steer_Duty_Max  810//
#define Steer_Duty_Midle 730  //750 //中值700
#define Steer_Duty_Min  650//
//--全局变量声明---

//---------变量声明-----------
extern uint8 Caiji_Flag;///电磁采集标志位
extern uint16  adc_date[5];                //储存电感采集值
extern uint16  adc_max[5];               //最大值
extern int16  ADC_Mid[3] ;                //相邻两个电感的中线
extern int16 adc_deviation;
extern int32 corner;              //车子的拐角（通过陀螺仪积分）
extern uint8 annulus_flag;
extern uint8 annulus_success;        //进环成功
extern  int16  Left_Adc,Right_Adc,Mid_Adc,Left_Xie_Adc,Right_Xie_Adc;//电感值

//电磁部分结构体变量
typedef struct   
{
  uint8 LeftInductance_Lost; //左电磁强度丢失
  uint8 RightInductance_Lost; //右电磁强度丢失
  uint8 AllInductance_Lose;//所有电磁丢失//////防撞保护标志位
  int16 Distance;         //电磁圆环测距
	uint8 SanChaKou_Flag;
	
}Electromagnetism;
extern Electromagnetism  Dianci;



//-----函数声明---
void init_Steer_PWM(void);
void Steering_Control_Out(int16 duty);

void ADC_Int();    //电磁ADC通道初始化
void ADC_MAX_Collect(void);//最值采集
void AD_Date_Fitier();/////////电感滑动滤波
void  normalize_date(); //电感归一化
 void STOP_Analyse (void);
void  Protect_Anticollision();//保护
void Electromagnetism_Control();




#endif