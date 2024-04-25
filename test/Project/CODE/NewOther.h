#ifndef _NEW_OTHER_H
#define _NEW_OTHER_H

#include "headfile.h"


//---全局变量外部声明-------
extern uint8 LEFT_RIGHT ;//       LEFT_RIGHT 1为右   0为左
extern uint32 T_ALready ;//全局计时（一直在计时的哦！）
extern  uint8 INT_Flag;//干簧管外部中断标志位

//-----函数声明--------------
void OutInGarage (unsigned char inout, unsigned char lr);








#endif