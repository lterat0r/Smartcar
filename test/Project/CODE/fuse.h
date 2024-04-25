#ifndef __FUSE_H__
#define __FUSE_H__
#include "headfile.h"

extern unsigned char   int_OK;//初始化成功标志位
extern int16   Turn_PWM ;   //最终转向PWM
//核心关键的标志位结构体定义
typedef struct 
{
      unsigned char start_go;           //开始发车跑
      
      unsigned char T_Inmost;           //最内环控制周期
      unsigned char T_Turn ;           //转向控制周期 
      unsigned char T_Speed ;            //速度控制周期
      unsigned char  STOP ;            //停车结束
	    unsigned char OUT_Garage;     //出库标志位
	unsigned char Game;
    
}Body;
extern Body Flag;

/*********函数声明**************/
void Fuse_result(void)  ;
//核心关键的标志位结构体初始化
void Flag_Init();



#endif
