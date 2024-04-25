#include "fuse.h"
Body Flag;                  //结构体定义各类关键标志位
int16 Turn_PWM = 0;   //最终转向PWM
/*******************PIT定时中断******************
函数：void Fuse_result(void)      
功能：速度环、转向环控制
参数：  无
说明：
返回值：无
日期： 2021-5-30
作者：  那个混子    */
unsigned char int_OK=0;//初始化成功标志位
void Fuse_result(void)      
{
  if(int_OK)                //初始化成功
  {
 			 // P62=0;// 输出低 亮灯  
        if(!Flag.start_go) // 保护（不动）  
       {
       go_motor(0,0);
       }
///////////////////////以下就是控制的执行代码//////////////////////
        else
        {
//5ms控制 
           if(1==Flag.T_Inmost)   
          {  
           Flag.T_Inmost = 0;
					//All_PWM_left = Speed_PWM-Turn_PWM*0.5;
					//All_PWM_right = Speed_PWM+Turn_PWM*0.5;
					All_PWM_left = Speed_PWM;
					All_PWM_right = Speed_PWM;
				if( !Dianci.SanChaKou_Flag)//非三岔口路段
          go_motor(All_PWM_left,All_PWM_right);          //动力输出

          } 
      
//10ms控制 方向
         if(1==Flag.T_Turn)
         {
            Flag.T_Turn=0;
         
				    Electromagnetism_Control();//电磁采集所有
					   // adc_deviation =Cha_BI_He(Left_Adc,Right_Adc,Mid_Adc,100);//左右中差比和 
		      // adc_deviation =Cha_BI_He_Sqrt(Left_Adc,Right_Adc,1000)+Cha_BI_He(Left_Xie_Adc,Right_Xie_Adc,0,0);
					 adc_deviation =Cha_BI_He_Sqrt(Left_Adc,Right_Adc,900)+Cha_BI_He_Sqrt(Left_Xie_Adc,Right_Xie_Adc,100);;
		       adc_deviation = PlacePID_Control(&TurnPID,Turn_Pid[Turn_Suquence], adc_deviation, 0); //转向动态PID   
           Turn_PWM =-adc_deviation;
          Steering_Control_Out(Turn_PWM);//舵机最终输出（函数内部已限幅）
	    }
//50ms控制	 速度			 
	    if(1==Flag.T_Speed)
            {
               Flag.T_Speed=0; 
                                         
            //Speed_PWM =  PID_Realize(&SpeedPID, Speed_Pid, real_speed,aim_speed);//速度位置式PID
	          //Speed_PWM= range_protect(Speed_PWM, -1200, 1200);	// 注意正负号 
						//如果没有编码器
							//Speed_PWM = aim_speed*10;
							Speed_PWM = aim_speed*10;
            }
   }
  }
}
  




//核心关键的标志位结构体初始化
void Flag_Init()
{
	    Flag.start_go = 0;
      Flag.T_Inmost = 0;
      Flag.T_Turn=0;
      Flag.T_Speed=0; 
      Flag.STOP=0;//停车结束
	    Flag.OUT_Garage = 0;//出库标志位
	    Flag.Game = 0;
}


