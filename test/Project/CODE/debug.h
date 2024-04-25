#ifndef _debug_h
#define _debug_h
#include "headfile.h"

#define BUZZ_ON            BUZZPin = 1;          //开蜂鸣器   
#define BUZZ_OFF           BUZZPin = 0;         //关蜂鸣器

//外部函数声明
void datasend(void);
void display( int hang,int date);
void Switch_Key_init(void);          //拨码开关初始化
void Strategy_Slect(void) ;          //拨码开关策略选择
void  Key_Scan_Deal (void );          //按键处理函数
void BUZZ_DiDiDi(uint16 PinLV);              //蜂鸣器滴滴滴
void Test_Servo_Hardware(void);
void Test_Motor_Hardware (void);
void Test_Electric_Hardware (void);

//外部变量申明
extern unsigned char TiaoCan;            //调参标志位
extern unsigned char TFT_SHOW;           //屏幕开关
extern unsigned char Switch1,Switch2,Switch3,Switch4,Switch5,Switch6;//拨码


#endif  
