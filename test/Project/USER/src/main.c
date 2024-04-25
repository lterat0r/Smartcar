/*********************************************************************************************************************
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 ********************************************************************************************************************/
/*****************************第十六届智能车专科组四轮*******************************
使用芯片：STC16F40K128   （没有使用摄像头，龙邱或者逐飞程序都兼容）
说明：
创作：那个混子
联系QQ：570656605 （备明来意，有问题都可联系！）
时间：2021-5-27 
最后一次修改备注：

加油，本程序仅为大家提供一些参考帮助，助力小白快速上手智能车制作！
     →→→→→→→→→→→→→→预祝各位在比赛中取得好成绩！←←←←←←←←←←←←←←←←←←←←←←
		 →→→→→→→→→→→→→ 做智能车后悔一年，不做智能车后悔一辈子←←←←←←←←←←←←←←

**************************************************************************************/
#include "headfile.h"//总的头文件
/*
 *关于内核频率的设定，可以查看board.h文件
 *在board_init中,已经将P54引脚设置为复位
 *如果需要
 使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
 */
 
//所有的初始化函数  必须最开始调用这个
void initialize()
{
	DisableGlobalIRQ();		//关闭总中断
	board_init();		//初始化内部寄存器，勿删除此句代码。
	soft_delay(1000);//软件稍微延时一下
	int_OK=0;//初始化标志位
	lcd_init(); //LCD显示屏幕初始化
	//lcd_showstr(0,0,(unsigned char*)"第十七届智能车四轮");		//字符串显示
	lcd_showstr(0,2,"interlize...");
	delay_ms(1000);

	Flag_Init();//结构体标志位初始化
	Switch_Key_init();//按键拨码开关初始化
	Strategy_Slect();//拨码开关策略选择
	gpio_mode(BUZZ_Pin,GPO_PP);BUZZPin=0;       // 蜂鸣器GPIO初始化
	init_PWM(1);//电机初始化(内含两种驱动) 0的话是两路模式MOS驱动，1的话是一路模式DRV驱动
	encoder_init();//编码器初始化
  init_Steer_PWM();//舵机初始化
	ADC_Int();//电磁ADC初始化
	PID_Parameter_Init(&TurnPID);//转向PID结构体初始化
  PID_Parameter_Init(&SpeedPID);//速度PID结构体初始化
  uart_init(UART_User,UART_RX_Pin,UART_TX_Pin, 115200, TIM_4);	// 初始化串口 波特率115200	这里使用定时器4		
	BUZZ_DiDiDi(200);//蜂鸣器滴一声
	delay_ms(50);
  exit_init(Reed_Switch_Pin,FALLING_EDGE); //初始化 作为外部中断引脚，下降沿触发。（用于停车干簧管检测，如果引脚只是普通引脚，不能使用外部中断）
	pit_timer_ms(TIM_1,5);   //	使用定时器做周期中断，时间5ms一次。


	EnableGlobalIRQ();	//开启总中断
}
/******主函数**************************/
void main()
{
	char txt[20];   
      initialize();//总的初始化
	
	
	/****下面的测试函数只是测试用，测试结束请注释关闭，一次只允许开一个测试函数！！******/
		  //Test_Servo_Hardware();//测试舵机调值
	   // Test_Motor_Hardware();//调试电机使用
	    //Test_Electric_Hardware();//测试电磁电感采集
      Turn_Suquence = 3;//转向的PID选择
	    Set_Speed = 210;       //速度设定290
     	aim_speed = Set_Speed;//勿管，中间变量而已
   
	    lcd_showstr(0,2,"Ready OK! ");//如果程序初始化成功会显示
    	//oled_fill(0x00);  //清屏

    while(1)
	{  
		//datasend();//传输数据到匿名科创上位机
		
		/*****显示5个电感值***********/
			sprintf(txt,"Mid_Adc= %05d",adc_date[4]); 
		lcd_showstr(1, 3, txt); //显示
			sprintf(txt,"Left_Adc= %05d",adc_date[0]);
		lcd_showstr(1, 4, txt); //显示
		  sprintf(txt,"Right_Adc= %05d",adc_date[1]);
		lcd_showstr(1, 5, txt); //显示
		 sprintf(txt,"Left_Xie= %05d",adc_date[2]);
		lcd_showstr(1, 6, txt); //显示
		 sprintf(txt,"Right_Xie= %05d",adc_date[3]);
		lcd_showstr(1, 7, txt); //显示

//===========================其他处理操作==========================================================================================================

          if(Flag.Game==0)  //出库准备（这里面的执行成功了程序才会执行fuse中断里面的！）
          {
              BUZZ_ON //蜂鸣器响
             OutInGarage (0, LEFT_RIGHT); //  出库   默认右
              Flag.Game = 1;
						 lcd_showstr(0,2,"Out Garage OK!");//出库成功
              BUZZ_OFF//关闭蜂鸣器
              int_OK=1;////初始化完成  这个标志位成立中断服务函数里面的才会执行的
              Flag.start_go = 1;//启动  出库成功才执行中断里面的东西
          }
#if 1 //如果调试程序的话，不让他识别车库，请把1改为0，不让他编译这里程序即可
          else if(Flag.Game==1)
          {
          STOP_Analyse ();//停车判别
          }
#endif


   }
	
}

