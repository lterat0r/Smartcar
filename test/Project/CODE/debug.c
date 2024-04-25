#include "debug.h"
//===================================================通信相关的==========================================================
//======================================================================================================================
/****************数据传输******************
函数：void datasend()
参数：  无
说明： 可以同时传输6个数  icm_gyro_x   icm_acc_x ICM_Real.gyro.y  ICM_Real.acc.z   
常看的变量：icm_acc_x  icm_gyro_y  Angle  adc_date[0] Left_Adc

返回值：无
日期：2020-7-10
作者：  那个混子     */
void datasend()
{  
   short send_data[6];                      

   send_data[0]=  adc_date[0] ; ////////ICM_Start.acc.x 
   send_data[1]=  adc_date[1] ;    //////////////////    MpuStart.gyro.x   Angle  
   send_data[2]= adc_date[2];//  ////////
   send_data[3]= adc_date[3]  ;//
   send_data[4]= Left_Adc;
   send_data[5]= Right_Adc  ;
   Data_Send(UART_User,send_data);
  // Data_Send(DEBUG_UART,send_data); 
}

//====================================================屏幕相关的=(已删除)=============================================================
//============================================================================================================================
//sprintf(temp," date20=%d",date);
//TFTSPI_P8X8Str(0,19,temp,BLACK,WHITE);break;

//==========================================================拨码开关及按键相关=========================================================
//====================================================================================================================================

//拨码开关引脚宏定义
#define Switch_Pin_1       P33
#define Switch_Pin_2       P32
#define Switch_Pin_3       P20
#define Switch_Pin_4       P20
#define Switch_Pin_5       P20
#define Switch_Pin_6       P20
//定义按键引脚
#define KEY1    P70      
#define KEY2    P71      
#define KEY3    P72        
#define KEY4    P73      
#define KEY5    P20       
//***************函数宏定义****(下面这些函数请修改宏定义为对应的GPIO库函数操作)***********
#define KEY_INT(key_x)           gpio_pull_set(key_x,PULLUP)//配置为上拉电阻   
#define SWITCH_INT(switch_x)     gpio_pull_set(switch_x,PULLUP)//配置为上拉电阻
#define READ_GPIO(Pin_X)         Pin_X
#define TiaoCan_DelayMs(M_S)     delay_ms(M_S)   //延时

unsigned char TiaoCan=0;////////////////////////调参标志位
unsigned char TFT_SHOW=0;///////////////////////屏幕开关
unsigned char Switch1=0,Switch2=0,Switch3=0,Switch4=0,Switch5=0,Switch6=0;//拨码
char parameter=0;//参数选择

//开关状态变量
uint8 key1_status = 1,key2_status = 1,key3_status = 1, key4_status = 1,key5_status = 1;
//上一次开关状态变量
uint8 key1_last_status, key2_last_status, key3_last_status, key4_last_status,key5_last_status;
//开关标志位
uint8 key1_flag=0,key2_flag=0,key3_flag=0, key4_flag=0,key5_flag=0;
/*****************拨码开关及按键初始化*****************
函数：void Switch_Key_init()
功能：初始化IO
参数：  无
说明： 初始化IO口   gpio_init(D1, GPI, GPIO_HIGH, GPI_PULL_UP); GPO_PUSH_PULL
返回值：无
日期：3月23
修改日期：2021-3-24
作者：   那个混子  */
void Switch_Key_init()
{

    //拨码开关初始化  （无需修改，请勿修改）
	    SWITCH_INT(Switch_Pin_1) ;
		  SWITCH_INT(Switch_Pin_2) ;
		  SWITCH_INT(Switch_Pin_3) ;
		  SWITCH_INT(Switch_Pin_4) ;
		  SWITCH_INT(Switch_Pin_5) ;
		  SWITCH_INT(Switch_Pin_6) ;
    
    //按键初始化 （无需修改，请勿修改）
   KEY_INT(KEY1);
	 KEY_INT(KEY2);
	 KEY_INT(KEY3);
	 KEY_INT(KEY4);
	 KEY_INT(KEY5);

}


/*****************拨码开关策略选择*****************
函数：void Strategy_Slect()
功能：通过拨码开关调整策略
参数：  无
说明：  6位拨码开关，如果有增加或者减少可对照修改,如果不足6个也不要删除多余的，多余的你随便引脚改个没用的即可
        使用你定义的就好了，其他没有用到的无需关心
返回值：无
日期：2021-3-23
修改日期：2021-3-24
作者：那个混子       */
void Strategy_Slect()
{
  //读取拨码开关状态
      if(!READ_GPIO(Switch_Pin_1))//用
      {
       Switch1=1;
       TFT_SHOW = 1;

      }
      if(!READ_GPIO (Switch_Pin_2))//用
      {
       Switch2=1;
       LEFT_RIGHT=0;//左出库左入库
      }
			
			
      if(!READ_GPIO (Switch_Pin_3))
      {
       Switch3=1;
      }
      if(!READ_GPIO (Switch_Pin_4))
      {
       Switch4=1;

      }
      if(!READ_GPIO (Switch_Pin_5))
      {
       Switch5=1;

      }
      if(!READ_GPIO (Switch_Pin_6))
      {
       Switch6=1;
 
      }

    if(Switch1||Switch2||Switch3||Switch4||Switch5||Switch6)//开启拨码开关
      {

      }

}

/*****************按键扫描读取*****************
函数：void  Key_Scan_Deal ()
功能：读取按键并执行对应操作
参数：  无
说明： 参考逐飞例程 ，5位按键，如果有增加或者减少可对照修改
      // 1号为左移键，2号为上键，3号为右移键，4号为中键盘，5号为下键
     //本次程序没有使用调参，stc单片机下载程序也快，改了烧就可以，如果要加的话自己根据下面的自己加就可以
返回值：无
日期：2021-3-23
修改日期：2021-3-23
作者：那个混子       */
uint8 gogo=0;
void  Key_Scan_Deal ()
{
    //使用此方法优点在于，不需要使用while(1) 等待，避免处理器资源浪费
    //保存按键状态
    key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;
    key4_last_status = key4_status;
    key5_last_status = key5_status;
    //读取当前按键状态
    key1_status = READ_GPIO(KEY1);
    key2_status = READ_GPIO(KEY2);
    key3_status = READ_GPIO(KEY3);
    key4_status = READ_GPIO(KEY4);
    key5_status = READ_GPIO(KEY5);
    //检测到按键按下之后  并放开置位标志位
    if(key1_status && !key1_last_status)    key1_flag = 1;
    if(key2_status && !key2_last_status)    key2_flag = 1;
    if(key3_status && !key3_last_status)    key3_flag = 1;
    if(key4_status && !key4_last_status)    key4_flag = 1;
    if(key5_status && !key5_last_status)    key5_flag = 1;
    //标志位置位之后，可以使用标志位执行自己想要做的事件
 
           if(key1_flag&&(gogo==1||gogo==2)) //上键
           {
               key1_flag = 0;//使用按键之后，应该清除标志位
           /*  以下为用户任务  */
                      switch(parameter)
                      {
              //-----------------------调参请修改下面--（注意修改对应的显示）----------------------------------------------------------------
                     //第一页显示的东西
                      case 0: aim_speed+=5; break;
                      case 1:Turn_Pid[1][0]+=0.1;    break;
                      case 2:Turn_Pid[1][1]+=0.5;   break;
                      case 3:Turn_Pid[1][2]+=0.1;   break;
                      case 4: break;
                      case 5:       break;

                     /// case 6:  ; break;//这个不能加任何操作在这里了，翻页使用啦

                      //第二页显示的东西
                      case 7: Speed_Pid[0]+=0.1 ; break;
                      case 8: Speed_Pid[0]+=0.1 ; break;
                      case 9: Speed_Pid[0]+=0.1 ; break;
                      case 10:  ; break;
                      case 11:  ; break;
                      case 12:  ; break;
              //--------------------调参请修改上面------------------------------------------------------------------
                      }
                      /*  以上为用户任务  */
              

           }

           if(key2_flag&&(gogo==1||gogo==2))//下键
           {
               key2_flag = 0;//使用按键之后，应该清除标志位
                /*  以下为用户任务  */
                      switch(parameter)
                      {
                //----------------------调参请修改下面--（注意修改对应的显示）--------------------------------------------------------------
                          //第一页显示的东西
                         case 0: aim_speed-=5; break;
                         case 1:Turn_Pid[1][0]-=0.1;    break;
                         case 2:Turn_Pid[1][1]-=0.5;   break;
                         case 3:Turn_Pid[1][2]-=0.1;   break;
                         case 4:break;
                         case 5:  break;

                        /// case 6:  ; break;//这个不能加任何操作在这里了，翻页使用啦

                         //第二页显示的东西
                         case 7:Speed_Pid[0]-=0.1; break;
                         case 8:Speed_Pid[1]-=0.1  ; break;
                         case 9:Speed_Pid[2]-=0.1  ; break;
                         case 10:  ; break;
                         case 11:  ; break;
                         case 12:  ; break;
              //--------------------调参请修改上面------------------------------------------------------------------
                      }
                      /*  以上为用户任务  */
              
           }

           if(key3_flag&&(gogo==1||gogo==2))//左键
           {
               key3_flag = 0;//使用按键之后，应该清除标志位
               /*  以下为用户任务  */
                      parameter--;
               /*  以上为用户任务  */

           }

           if(key4_flag&&(gogo==1||gogo==2))//右键
           {
               key4_flag = 0;//使用按键之后，应该清除标志位
               /*  以下为用户任务  */
                      parameter++;
               /*  以上为用户任务  */

           }
            if(key5_flag) //中键
           {
              key5_flag = 0;//使用按键之后，应该清除标志位
            /*  以下为用户任务  */
						

						/*  以上为用户任务  */

           }
     if(gogo==6)//告辞啦，调参结束！！！！！！！！！！！！
			{

					TiaoCan = 1;      //调参结束标志位

			}
											
											       //lcd_showfloat(0,5,Speed_Pid[0],2,1);//显示浮点数   整数显示2位   小数显示三位
           //*******************************屏幕显示第一页***********************

    if(parameter<6&&gogo>=1)//显示参数0到5，实际显示1到6
            {
          
            }
           //*******************************屏幕显示第二页**************************************************
           if(parameter>6&&parameter<13)//这里行号从4到9   一页调6个参数  //显示参数7到5，实际显示7到12
           {
          
            }
           //*******************************屏幕显示第三页**************************************************
                   if(parameter>13&&parameter<20)
                   {
                  
                    }
           //###########还需要更多页仿照着写就可以咯######################这里就不写了 结束
                   if(parameter==6||parameter==13||parameter==20)  //翻页准备
                   {
                     
                   }//清屏





}


//===============================================蜂鸣器相关=============================================
//=====================================================================================================

//蜂鸣器开和关 被写在对应头文件去了，去H文件查看

/*****************蜂鸣器滴滴滴*****************
函数：void BUZZ_DiDiDi()
功能：蜂鸣器滴滴滴
参数：  无
说明：
返回值：无
日期：2021-3-23
修改日期：2021-3-23
作者： 那个混子      */
void BUZZ_DiDiDi(uint16 PinLV)
{
  BUZZ_ON;
  TiaoCan_DelayMs(PinLV);
  BUZZ_OFF;
}


/***************************测试完毕**********************************************
 *  函数名称：Test_Servo(void)
 *  功能说明：舵机PWM初始化，测试标定输出PWM控制SD5/S3010舵机
 *  参数说明：无
 *  函数返回：无
 *  修改时间：
 *  备    注：参考龙邱库提供的！！！！
 【注意事项】注意，一定要对舵机打角进行限制
 使用龙邱母板测试流程：
 1.先使用万用表测量电池电压，务必保证电池电压在7V以上，否则无力不反应！
 2.然后确定舵机供电电压，SD5舵机用5V供电，S3010用6-7V供电，SD012舵机用5V供电！！！
 3.把舵机的舵盘去掉，让舵机可以自由转动；
 4.烧写程序并运行，让舵机转动到中值附近；如果没反应重复1-2步，或者调整舵机的PWM频率计占空比，能受控为准；
 5.舵机受控后用手轻转，舵机会吱吱响，对抗转动，此时可以装上舵盘；
 6.按键K0/K1确定舵机的左右转动极限，并记下来，作为后续限幅防止舵机堵转烧毁！
 *************************************************************************/
void Test_Servo_Hardware (void)
{
    char txt[16] = "X:";
    unsigned int  duty = Steer_Duty_Midle;

    oled_fill(0x00);  //初始清屏
	lcd_showstr(0, 0, "Test_Servo_Hardware:");
   	pwm_init(Steer_Pin, 50, Steer_Duty_Midle);     //初始化舵机  输出PWM频率200HZ，并设置中值
    pwm_duty(Steer_Pin, Steer_Duty_Midle);
    while (1)
    {
        if (!READ_GPIO(KEY1))
        {
            if (duty > 100)  		//防止duty超
            {
                duty += 10;     //标定的时候，可以把步长改小点，比如10
            }
        }
        if (! READ_GPIO(KEY2))
        {
            duty = Steer_Duty_Midle;
        }
        if (! READ_GPIO(KEY4))
        {
            duty -= 10;
        }
			  pwm_duty(Steer_Pin, duty);
				lcd_showint8(0, 5, duty);
				sprintf(txt, "Servo:%05d ", duty);
				oled_p6x8str(1, 2, txt); //显示
				TiaoCan_DelayMs(100);        
    }
}

/****************************测试完毕*********************************************
 *  函数名称：TestMotor(void)
 *  功能说明：测试标定输出PWM控制电机
 *  参数说明：无
 *  函数返回：无
 *  修改时间：
 *  备    注：驱动2个电机
 【注意事项】注意，一定要对电机输出进行限制
 使用龙邱母板测试流程：
 1.先使用万用表测量电池电压，务必保证电池电压在7V以上，否则无力不反应！
 2.接好母板到驱动板的信号线及电源线；
 3.接好驱动板到电机的导线；
 4.烧写程序并运行，确定电机能正常转动后，开启驱动板电源开关；
 5.按键K0/K1确定电机转动速度及方向；
 6.如果出现疯转，按下K1键返回低速模式，或者直接关闭驱动板电源！
 *************************************************************************/
void Test_Motor_Hardware (void)
{
    short duty = 2000;
    oled_fill(0x00);  //初始清屏
	lcd_showstr(2, 0, "Test_Motor_Hardware:");
   init_PWM(0);
    while (1)
    {
    if (!READ_GPIO(KEY1))   //按下KEY0键   左轮单独正转
        {
           	 go_motor (duty,0);
					 lcd_showstr(0, 4, "Left  Front");   //字符串显示
        }
       if (!READ_GPIO(KEY2)) //按下KEY2键，左右轮同时反转
        {
          	 go_motor (-duty,-duty);
					 lcd_showstr(0, 4, "All  Black");   //字符串显示
           	  	 
        }
      if (!READ_GPIO(KEY3))      //按下KEY1键  右轮单独正转
        {
           	 go_motor (0,duty);
						 lcd_showstr(0, 4, "Right Front");   //字符串显示
        }
				if((READ_GPIO(KEY1))&&(READ_GPIO(KEY2))&&(READ_GPIO(KEY3)))
        go_motor (0,0);
      	TiaoCan_DelayMs(100);  
    }
}

/****************************测试完毕*********************************************
 *  函数名称：void Test_Electric_Hardware (void)
 *  功能说明：测试电磁电感硬件
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2021-5-30
 *  备    注：
 【注意事项】
 *************************************************************************/
void Test_Electric_Hardware (void)
{
	 char txt[16];
	oled_fill(0x00);  //初始清屏
	lcd_showstr(2, 0, "Test_Electric_Hardware:");
	ADC_Int();
	while(1)
	{
			//datasend();
		 if (!READ_GPIO(KEY2)) //按下KEY2键，左右轮同时反转
        {
					 lcd_showstr(2, 1, "Normalize_Deal....");   //字符串显示
					AD_Date_Fitier();
					normalize_date();
					sprintf(txt,"adc0= %05f",adc_date[0]);
				lcd_showstr(1, 2, txt); //显示
					sprintf(txt,"adc1= %05f",adc_date[1]);
				lcd_showstr(1, 3, txt); //显示
					sprintf(txt,"adc2= %05f",adc_date[2]);
				lcd_showstr(1, 4, txt); //显示
					sprintf(txt,"adc3= %05d",adc_date[3]);
				lcd_showstr(1, 5, txt); //显示
					sprintf(txt,"adc4= %05d",adc_date[4]);
				lcd_showstr(1, 6, txt); //显示					
           	  	 
        }
				else
				{
					
				
		 lcd_showstr(2, 1, "                   ");   //字符串显示
     	AD_Date_Fitier();
//		  adc_date[0] = adc_mean_filter (Left_ADC_Pin,ADC_12BIT,20); 
//      adc_date[1] = adc_mean_filter  (Right_ADC_Pin,ADC_12BIT,20); 
//      adc_date[2] = adc_mean_filter  (LeftXie_ADC_Pin,ADC_12BIT,20); 
//      adc_date[3] = adc_mean_filter  (RightXie_ADC_Pin,ADC_12BIT,20); 
//      adc_date[4] = adc_mean_filter  (Mid_ADC_Pin,ADC_12BIT,2); 
		  sprintf(txt,"adc0= %05d",adc_date[0]);
		lcd_showstr(1, 2, txt); //显示
			sprintf(txt,"adc1= %05d",adc_date[1]);
		lcd_showstr(1, 3, txt); //显示
			sprintf(txt,"adc2= %05d",adc_date[2]);
		lcd_showstr(1, 4, txt); //显示
		  sprintf(txt,"adc3= %05d",adc_date[3]);
		lcd_showstr(1, 5, txt); //显示
		  sprintf(txt,"adc4= %05d",adc_date[4]);
		lcd_showstr(1, 6, txt); //显示
          	}
}
	
}



