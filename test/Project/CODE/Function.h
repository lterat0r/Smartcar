#ifndef _Function_h
#define _Function_h


#include "common.h"
#include "headfile.h"
#include "math.h"
//PID
//这几个类似于数组下标号，方便看
#define KP 0
#define KI 1
#define KD 2
#define KT 3   //积分限幅项
//变量


typedef struct PID   //用来PID参数计算变量
{
	float SumError;	//误差累计	
	int32 LastError;	// 上次误差
	int32 PrevError;	// 预测误差	
	int32 LastData;	// 上次数据
} PID;

/********PID中间变量声明************/
extern PID SpeedPID;//速度PID储存（只是储存的中间量，无需关心）
extern PID TurnPID;//转向PID储存（只是储存的中间量，无需关心）

/********PID参数变量外部声明******/
extern float Speed_Pid[4];		// 速度环PID
extern float Turn_Pid[][4];
extern uint8 Turn_Suquence ;//转向PID选择



//*******************函数声明***********************

// PID参数初始化
void PID_Parameter_Init(PID *sptr);
int16 limit(int16 x, int y);                //限幅

/**************限幅，两端不一样值*****/
//#define  limit_ab( x,  min,  max)  ( (x<min) ? min : ( (x>max) ? max : x ))
int32 range_protect(int32 duty, int32 min, int32 max);//限幅保护	
void  soft_delay(long t);                         //延时

int   myabs(int dat);                        //求绝对值


int16  filter(int16 new_value,int16 value) ;     //一阶低通滤波

int16  Turn_Out_Filter(float turn_out)     ;         //加权平均滤波

float Weights_Of_Filter(float Date,float value_1,float value_2,float value_3);//加权滤波  

float Cha_BI_He(float Date_1,float Date_2,float Date_3,int16 X);  //两个数据的差比和

float Cha_BI_He_Sqrt(int16 date_1,int16 date_2,int16 x);//两个数的1.5次方

float Cha_BI_Ji(float date_1,float date_2,int16 x);  //两个数差比积

int16 Cubic_Function(int16 DATE,float A,float B);   //对一个数进行三次方

// PID参数初始化
void PID_Parameter_Init(PID *sptr);


// 位置式动态PID控制
int32 PlacePID_Control(PID *sprt, float *PID, int32 NowPiont, int32 SetPoint);

// 位置式PID控制
int32 PID_Realize(PID *sptr, float *PID, int32 NowData, int32 Point);

// 增量式PID控制
int32 PID_Increase(PID *sptr, float *PID, int32 NowData, int32 Point);
//去极值求平均
int16 I_Median_Average_Filter(int16 *DATE);


#endif
