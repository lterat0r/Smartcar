#include "NewOther.h"
 
/*=========================出库入库处理================================================
 inout 1为入库 0为出库
 lr     1为右   0为左
 注意：如果设置了右出必定为右进，设置了左出必定为左进
 说明：已经修改为时间方式出入库了，由于之前使用编码器路程方法，发现很多人不清楚不懂不会找问题，那用土办法吧!
日期：2021-5-29-9:00
修改时间：2021-5-29-9:00
作者：  那个混子  
 *******************************************/
uint8 LEFT_RIGHT = 0;//       LEFT_RIGHT 1为右   0为左
uint32 T_ALready = 0;//全局计时（一直在计时的哦！）
void OutInGarage (unsigned char inout, unsigned char lr)
 {
     int16 T_New = 0;//当前时间获取
//------------------右边-右边-右边-右边-右边---------右边出库、入库操作----------------------------------------------------------------
     if (lr)           // 1右出入库
     {
         if (inout)    // 1右入库
         {
             T_New = T_ALready;//获取当前时间
            
//             go_motor(-4000,-4000); 
//           	 delay_ms(1200);//调用库自带的延时函数
//					   go_motor(3000,2000); 
					 
					 while(P32==0)
					 {	
						 	 pwm_duty(Steer_Pin, Steer_Duty_Midle);//回正方向
						  go_motor(-2500,-2500); 
					 }
							delay_ms(250);
					   go_motor(-6000,-3000);   //go_motor(4000,-2000);
					    pwm_duty(Steer_Pin, Steer_Duty_Min);//控制右打角
             while (T_ALready < T_New + 20)
             {
                  	delay_ms(10);//调用库自带的延时函数
             }
						 	 pwm_duty(Steer_Pin, Steer_Duty_Midle);//回正方向
						 go_motor(-3000,-3000); 
						 delay_ms(100);
             DisableGlobalIRQ();		//关闭总中断 
             while (1)
             {
                 //speed_measure();  //采样速度(需要编码器，如果没得编码器请勿打开)
							   //go_motor(-real_speed*1000,-real_speed*1000);//刹车（需要编码器，如果没得编码器请勿打开）
                delay_ms(10);//调用库自带的延时函数
                go_motor(0,0);//停止
            }                 // 入库完毕，永久停车
         }
		
         else  // 0右出库
         {
             T_New = T_ALready;//获取当前时间
             pwm_duty(Steer_Pin, Steer_Duty_Midle);//直走
             go_motor(2000,2000);       //固定速度出去
             while (T_ALready < T_New + 15)//根据实际调这个时间控制出库直走距离
             {
                 	delay_ms(10);//调用库自带的延时函数
             }
             T_New = T_ALready;//获取当前时间
             pwm_duty(Steer_Pin,Steer_Duty_Min); // 舵机向右打死为出库做准备
              go_motor(2000,2000);       //固定速度出去
             while (T_ALready < T_New + 12)  //(T_ALready < T_New + 20)
             {
                  	delay_ms(10);//调用库自带的延时函数
             }
         }
     }

//--------------------------左边-左边-左边-------------------左边出库、入库操作----------------------------------------------------------------
     else // 0：左出入库；
        {
            if (inout) // 1左入库
						{
             T_New = T_ALready;//获取当前时间
            
					 
					 while(P32==0)//退回来再次检测到干簧管，之前尝试用延时控制后退，不够精确改用此方法！！！！
					 {
						 	 pwm_duty(Steer_Pin, Steer_Duty_Midle);//回正方向
						 // go_motor(-1500,-1500); 
					 }
					 delay_ms(250);
					   go_motor(-3000,-6000); 
					    pwm_duty(Steer_Pin, Steer_Duty_Max);//控制右打角
             while (T_ALready < T_New + 21)
             {
                  	delay_ms(10);//调用库自带的延时函数
             }
						 	 pwm_duty(Steer_Pin, Steer_Duty_Midle);//回正方向
						 go_motor(2000,2000);  //1500 1500
						  delay_ms(50);
             DisableGlobalIRQ();		//关闭总中断 
             while (1)
             {
                 //speed_measure();  //采样速度(需要编码器，如果没得编码器请勿打开)
							   //go_motor(-real_speed*1000,-real_speed*1000);//刹车（需要编码器，如果没得编码器请勿打开）
                delay_ms(10);//调用库自带的延时函数
                go_motor(0,0);//停止
            }                 // 入库完毕，永久停车
            }
            else  // 0左出库
            {
					   T_New = T_ALready;//获取当前时间
             pwm_duty(Steer_Pin, Steer_Duty_Midle);//直走
             go_motor(2000,2000);       //固定速度出去
             while (T_ALready < T_New + 12)
             {
                 	delay_ms(10);//调用库自带的延时函数
             }
             T_New = T_ALready;//获取当前时间
             pwm_duty(Steer_Pin,Steer_Duty_Max); // 舵机向左打死为出库做准备
              go_motor(2000,2000);       //固定速度出去
             while (T_ALready < T_New + 12)
             {
                  	delay_ms(10);//调用库自带的延时函数
             }
       
            }
        }
 }
 
 /************停车检测处理*****
 函数：void STOP_Analyse ()
 功能：识别斑马线建立停车标志位
 参数：
 说明：
 返回值：
日期：2021-5-29-9:00
修改时间：2021-5-29-9:00
作者：  那个混子  
 *******************************************/
 uint16 STOP_T_Count = 0;
 unsigned char Only_Once = 0;
 uint8 INT_Flag=0;//干簧管外部中断标志位
 void STOP_Analyse (void)
 {
 if(Flag.start_go==1 && Flag.STOP==0&&annulus_flag==0&&Dianci.SanChaKou_Flag==0) 
 {
 if(INT_Flag)
 {
	 INT_Flag = 0;//触发后清零
	   BUZZ_ON
      if(Only_Once==0) {STOP_T_Count= T_ALready;  Only_Once = 1;}//第一次识别到读取一次时间
     if(( T_ALready>STOP_T_Count+10))//识别第一次后，让车子跑出去一段距离再进行下一次的识别
         {
             Flag.STOP=1;
         }
 }
 }
//----------------出库操作-----------------------------------------------
 if(Flag.STOP == 1) //入库
{
 int_OK =0;   //清零不再执行中断里其他程序
 Flag.start_go = 0;//停止中断内执行
 
 BUZZ_OFF
 OutInGarage (1, LEFT_RIGHT);

}

 }

 
/*################调试专用#############################################
                                                      	DisableGlobalIRQ();		//关闭总中断
                                                             while(1)
                                                             {
                                                                 go_motor(0,0);
                                                                 BUZZ_OFF;
                                                             }
    //################调试专用#############################################*/


