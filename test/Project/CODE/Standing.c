#include "Standing.h"
//转向用icm_gyro_z
//直立用icm_gyro_y  icm_acc_x
//icm_acc_x -460----1550
//直立PD
 uint16  K_P=300;
 uint16 K_D=20;
 
#define   CONSTANT      2     //补偿系数  减小要适当增大D
#define   DT          0.0166 //积分系数      决定了跟踪速度
#define   R_Gyro      0.04  //角速度系数0.4
#define   R_Acc       0.025  //角度系数
float Angle;
float  AngleIntegral=30.0;
int16  Angle_pwm=0;
 int16 aim_angle=0;//目标角度
/******************角度更新融合*************
名称：采集的角度的处理
说明：该函数包含了对角度和加速度进行归零处理、归一化处理、角度融合
思路： 采集值→→→→调零位→→→→归一化→→→→融合 →→→→积分 →→→→最终角度
                                          ↘    ↗
                                            补偿（通过实时的速度与积分的角度作一个跟踪互补）
*/
void Complementary_Filter()
{ 	
    float  fDeltaValue = 0;                              //补偿量
  ICM_Real.gyro.y = (ICMOffset.gyro.y - icm_gyro_y)*R_Gyro;//角速度
  ICM_Real.acc.x =  (icm_acc_x-  ICMOffset.acc.x )*R_Acc;//角度
	ICM_Real.gyro.z = (icm_gyro_y - ICMOffset.gyro.z );

  Angle = AngleIntegral;                                 //最终融合角度
  fDeltaValue = ( ICM_Real.acc.x - Angle) / CONSTANT;        //时间系数矫正、补偿量
  AngleIntegral += (ICM_Real.gyro.y + fDeltaValue) * DT; //角速度积分融合后的角度 
}

/****************数据传输******************
函数：void datasend()
参数：  无
说明： 可以同时传输6个数  icm_gyro_x   icm_acc_x ICM_Real.gyro.y  ICM_Real.acc.z 
常看的变量：icm_acc_x  icm_gyro_y  Angle

返回值：无
日期：2020-7-10
作者：  那个混子     */
void datasend()
{  
   short send_data[6];                      

   send_data[0]=aim_angle ; ////////MPU_Treated.acc.x 
   send_data[1]= left_speed ;    //////////////////    MpuStart.gyro.x   Angle  
   send_data[2]= icm_acc_x;//  ////////
   send_data[3]= icm_gyro_z  ;//
   send_data[4]= aim_angle;
	 send_data[5]= ADC_PWM ;
   Data_Send(UART_1,send_data);
	
}



