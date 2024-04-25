#include "headfile.h"
#include  "speed.h"

/***********速度各变量声明**************/
int16  aim_speed,Set_Speed ;         //目标速度 

int16 real_speed;        //实时速度
int16 Speed_Min;

int16 right_speed;       //右轮速度
int16 left_speed;        //左轮速度
int16 last_speed;        //上一次速度
int16 Last_right_speed;//右轮上次速度
int16 Last_left_speed;//左轮上次速度
int32 distance=0;          //车子行走的路程


int16 Speed_PWM=0;         //速度环PWM
int16 All_PWM_left=0;     //左轮PWM
int16 All_PWM_right=0;    //右轮PWM


    
/*****************编码器初始化*****************
函数：void encoder_init(void)
功能：编码器初始化
参数：  无
说明：ctimer_count_init(CTIM0_P34);
      编码器使用TIM3和TIM4，如更改引脚只需修改宏定义即可 
      编码器使用带方向的编码器（好像不支持正交解码）
返回值：
日期： 2021-3-25晚00:59
修改日期：2021-3-26
作者：  混子    */
void encoder_init(void)
{
   ctimer_count_init(Left_Ecoder_Pin1);//初始化左编码器外部定时器
	 Left_Ecoder_Pin2=1; //初始化左编码器的方向引脚
	 ctimer_count_init(Right_Ecoder_Pin1);//初始化右编码器外部定时器
   Right_Ecoder_Pin2=1;//初始化右编码器的方向引脚
}    
/*****************速度测量*****************
函数：void speed_measure(void)
功能：测量实时速度，两个轮的平均速度 编码器采集值，
参数：  无
说明：  该函数可以测出速度，获取速度后无需再次复位清零，已包括了
返回值：
日期： 2021-1-12-20:30
修改日期：2021-1-12-20:30
作者： 混子    */
void speed_measure(void)
{ 
////////////////////////右轮测速//////////Right//////
	                      left_speed = ctimer_count_read(Right_Ecoder_Pin1);
                     ctimer_count_clean(Right_Ecoder_Pin1); 

                  
	
//////////////////// 左轮测速/////////Left///////////////
                       right_speed = ctimer_count_read(Left_Ecoder_Pin1);
                    ctimer_count_clean(Left_Ecoder_Pin1); 
                    
//////////////////////带方向编码器使用下面读取方向/////////////////
                    if ( 1==Left_Ecoder_Pin2)
                      left_speed = -left_speed;
                    else    left_speed = left_speed;
                    if ( 1==Right_Ecoder_Pin2)
                      right_speed = -right_speed;
                    else    right_speed = right_speed;
                      
          


     real_speed = (right_speed+left_speed)/2;        // 速度平均值
   //  Speed_Min = last_speed;
       ///  Speed_Min = range_protect(Speed_Min, -500, 500);
   // real_speed = limit(real_speed,2000); //限幅保护   
     last_speed=real_speed; 
               
    distance +=(right_speed+left_speed)*0.01;    //测距离  化为厘米为单位        	
}           

/*********************速度PI********************
仅供参考，没有使用到
**************************************/

static float SpeedErrorInteg = 0;       //偏差积分 
float I_Calculate=0;//积分I计算值
float P_Calculate=0;//比例P计算值
static int8 separate=1;    //积分分离系数
float Speed_P_I_Control(float *PID, int16 speed, int16 set_speed)//因为角度变化小，需要是浮点型
{   
    float  SpeedControlOut;//最终计算量
    int16 SpeedError;	// 当前误差
   
    static uint8 k;
    static float PreError[10]={0};   
    SpeedError = set_speed - speed;  //计算速度偏差
    SpeedErrorInteg=0;          //误差积分
    for(k=9;k>0;k--)          //20次误差（--比++快）
    {  
     PreError[k]=PreError[k-1];
     SpeedErrorInteg+=PreError[k];
    }
     PreError[0]=SpeedError;
     SpeedErrorInteg+=PreError[0]; //积分项
    
     I_Calculate = separate * PID[KI] * SpeedErrorInteg;  //积分I计算值
  
     P_Calculate=PID[KP]*SpeedError;//比例P计算值
		
		/////////////////////
		 if(SpeedError<0)//超速，
		 {
		    SpeedControlOut=0;
			 /// SpeedControlOut=-5;
			 
		 }
		 else
		 {
      SpeedControlOut = P_Calculate+I_Calculate;  // 最终输出 
		 }
		
		///////////////////////
                          
          
     return   SpeedControlOut;
     }

/******************** 电机初始化***************
函数：void init_PWM(void)
参数：  无
说明：  分母10000
		    pwm_init(PWMA_CH1P_P60, 10000, 0);     //初始化PWM5  使用引脚P2.5  输出PWM频率10000HZ   占空比为百分之 pwm_duty / PWM_DUTY_MAX * 100
返回值：无
日期：2021-5-27-晚23:00
修改日期：2021-5-27-晚23:00
作者： 混子    */
unsigned char MOTOR_MODE=0;//请勿修改删除
void init_PWM(unsigned char Motor_Set)
{
	MOTOR_MODE = Motor_Set;
	if (MOTOR_MODE==0) 
	{
//-----MOS驱动-----------
  pwm_init(Left_Z_Pin, 17000,0);//左轮初始化
	pwm_init(Left_F_Pin, 17000,0);
	pwm_init(Right_Z_Pin, 17000,0);//右轮初始化
	pwm_init(Right_F_Pin, 17000,0);
	}
		else
	{
//------DRV驱动-------------
	pwm_init(Left_PWM_Pin, 17000,0);//左轮初始化
  gpio_mode(Left_DIR_Pin,GPO_PP);       // 设置DRV方向引脚为为推挽输出
	pwm_init(Right_PWM_Pin, 17000,0);//右轮初始化
	gpio_mode(Right_DIR_Pin,GPO_PP);       // 设置DRV方向引脚为为推挽输出

	}
 
}
/****************************电机输出**********************
函数：void go_motor (int16 left_PWM,int16 right_PWM)
参数：  int16 left_PWM,int16 right_PWM
说明：pwm_duty(PWMA_CH1P_P60, duty);
      务必将车子的电机逻辑调像这个一样，第一个参数控制左电机，第二个参数控制右电机
      传入的参数如果为正数，电机正转，负值反转！！！！！
返回值：无
日期：2021-5-27-晚23:00
修改日期：2021-5-27-晚23:00
作者： 混子    */
#define Duty_Max  7000   //限幅最大值9500

void go_motor (int16 left_PWM,int16 right_PWM)
{
if  (MOTOR_MODE==0)
{	
//-------MOS驱动--------	
    if (left_PWM>0)           //左轮
  {
		left_PWM = left_PWM<=Duty_Max ? left_PWM : Duty_Max;
     pwm_duty(Left_Z_Pin,left_PWM);
     pwm_duty(Left_F_Pin,0);			 //正转
   } 	
else 
 {
    left_PWM = left_PWM>=-Duty_Max ? (-left_PWM) : Duty_Max;  
     pwm_duty(Left_Z_Pin,0);	
     pwm_duty(Left_F_Pin,left_PWM);			 //反转
  }

    if (right_PWM>0)           //右轮
  {
   right_PWM = right_PWM<=Duty_Max ? right_PWM : Duty_Max; 
	 pwm_duty(Right_Z_Pin,right_PWM);	
	 pwm_duty(Right_F_Pin,0);			 //正转
   } 
else 
 {
    right_PWM = right_PWM>=-Duty_Max ? (-right_PWM) : Duty_Max;  
	  pwm_duty(Right_Z_Pin,0);	
	  pwm_duty(Right_F_Pin,right_PWM); //反转
  }
}
else
{
//-------DRV驱动-----------
   if (left_PWM>0)           //左轮
  {
		left_PWM = left_PWM<=Duty_Max ? left_PWM : Duty_Max;
		 P64=0;			 
     pwm_duty(Left_PWM_Pin,left_PWM);//正转
   } 	
else 
 {
    left_PWM = left_PWM>=-Duty_Max ? (-left_PWM) : Duty_Max;  
     P64=1;	
     pwm_duty(Left_PWM_Pin,left_PWM);//反转
  }

    if (right_PWM>0)           //右轮
  {
   right_PWM = right_PWM<=Duty_Max ? right_PWM : Duty_Max;
   P60=0;			 
	 pwm_duty(Right_PWM_Pin,right_PWM);	//正转		
	
   } 
else 
 {
    right_PWM = right_PWM>=-Duty_Max ? (-right_PWM) : Duty_Max;  
	  P60=1;
	  pwm_duty(Right_PWM_Pin,right_PWM); //反转
  }
}

}

