#include  "Direction.h"
//-----前面是电磁所有相关的--------
//----最后是舵机相关的------------
uint16  adc_date[5];                //储存电感采集值 原始值   5个电感
uint16 leftleft = 0;
int16  Left_Adc=0,Right_Adc=0,Mid_Adc=0,Left_Xie_Adc=0,Right_Xie_Adc=0;//电感值

uint16  adc_max[5]={2000,2000,2200,2200,2000};               //最大值
uint16  adc_min[5]={0,0,100,100,100};               //最小值  (给定一个需要)

int16  adc_deviation;            //电感偏差
Electromagnetism  Dianci;////////////////////电磁部分结构体变量

/****************ADC初始化**************************
函数：void ADC_Int()
功能：ADC通道引脚初始化
参数：  无
说明： 	adc_init(ADC_P10,ADC_SYSclk_DIV_2);//初始化P1.0为ADC功能,ADC时钟频率：SYSclk/2
       
引脚：                                                                                                
返回值：无
日期：2021-5-28-19:30
修改时间：2021-5-28-19:30
作者：  那个混子     
*/
void ADC_Int()
{
	
	adc_init(Left_ADC_Pin,ADC_SYSclk_DIV_2);//初始化ADC功能
	adc_init(Right_ADC_Pin,ADC_SYSclk_DIV_2);//初始化ADC功能
	adc_init(LeftXie_ADC_Pin,ADC_SYSclk_DIV_2);//初始化ADC功能
	adc_init(RightXie_ADC_Pin,ADC_SYSclk_DIV_2);//初始化ADC功能
	adc_init(Mid_ADC_Pin,ADC_SYSclk_DIV_2);//初始化ADC功能

}

/****************电感采集**************************
函数：void ADC_MAX_Collect(void)
功能：
参数：  无
说明：  调用库函数（自己写的）    adc_mean_filter (ADC_P10, ADC_12BIT, 10)		
返回值：无
日期：2021-5-28-19:30
修改时间：2021-5-28-19:30
作者：  那个混子  */
void ADC_MAX_Collect(void)
  {
    int i,j;
    for(i = 600; i>0; i--)
    {
     
      adc_date[0] = adc_mean_filter (Left_ADC_Pin,ADC_12BIT,2); 
      adc_date[1] = adc_mean_filter  (Right_ADC_Pin,ADC_12BIT,2); 
      adc_date[2] = adc_mean_filter  (LeftXie_ADC_Pin,ADC_12BIT,2); 
      adc_date[3] = adc_mean_filter  (RightXie_ADC_Pin,ADC_12BIT,2); 
      adc_date[4] = adc_mean_filter  (Mid_ADC_Pin,ADC_12BIT,2); 
     for(j=0;j<3;j++)//三个横电感
	{
	    if(adc_date[j]>=adc_max[j])
	        adc_max[j]=adc_date[j];
	  //  if(adc_date[j]<=adc_min[j])
        // adc_min[j]=adc_date[j];
	      delay_ms(2);//延时采集
	}

    }
}
/****************电感采集**************************
函数：void AD_Date_Fitier()
功能：对电感采值进行冒泡排序滑动滤波
参数：  无
说明：12bit  4096   调用库函数（自己写的）    adc_mean_filter (ADC_P10, ADC_12BIT, 10)	
返回值：无
日期：2021-5-28-19:30
修改时间：2021-5-28-19:30
作者：  那个混子  */

#define FILTER_N 5 //滤波深度
 void AD_Date_Fitier()
{
   uint8 i;
  int16 filter_buf_L[FILTER_N];  //左横电感储存数组 
  int16 filter_buf_LC[FILTER_N]; //左斜电感储存数组
  int16 filter_buf_M[FILTER_N];  //中横电感储存数组 
  int16 filter_buf_R[FILTER_N];  //右横电感储存数组
  int16 filter_buf_RC[FILTER_N];  //右斜电感储存数组
  
 
//--------滑动滤波--------------
	for(i = 0; i <FILTER_N; i++)//采值 
  {
       filter_buf_L[i]  =   adc_mean_filter (Left_ADC_Pin,ADC_12BIT,10); //左横
       filter_buf_LC[i] =   adc_mean_filter  (LeftXie_ADC_Pin,ADC_12BIT,5); //左斜  
       
       filter_buf_M[i]  =   adc_mean_filter  (Mid_ADC_Pin,ADC_12BIT,5);//中间 
       
       filter_buf_RC[i] =  adc_mean_filter  (RightXie_ADC_Pin,ADC_12BIT,10);    //右斜   
       filter_buf_R[i]  =  adc_mean_filter  (Right_ADC_Pin,ADC_12BIT,5);   //右横  
   }
//--------冒泡排序去极值求平均---------
  adc_date[0]= I_Median_Average_Filter(filter_buf_L);//左      3600
  adc_date[1] =I_Median_Average_Filter(filter_buf_R);//右      3600
  adc_date[2] = I_Median_Average_Filter(filter_buf_LC);//左斜  3000
  adc_date[3] = I_Median_Average_Filter(filter_buf_RC);//右斜
  adc_date[4] = I_Median_Average_Filter(filter_buf_M);//中间   4050
	  
	 
	 
  Left_Adc = adc_date[0];//左电感最终值
  Right_Adc = adc_date[1];//右电感最终值 
  Left_Xie_Adc = adc_date[2];//左斜电感最终值
  Right_Xie_Adc = adc_date[3];//右斜电感最终值
	Mid_Adc = adc_date[4];//中间电感最终值
 

}

/*****************归一化处理***********************
函数：void  normalize_date()
功能：采集电感值，进行归一化缩小，
参数：  无
说明：  在该函数中调用电感采集函数，在使用中需调用求最大值函数和该函数。
返回值：无
日期：2021-5-28-19:30
修改时间：2021-5-28-19:30
作者：  那个混子  */
/*****************归一化处理***********************
函数：void  normalize_date()
功能：采集电感值，进行归一化缩小，
参数：  无
说明：  在该函数中调用电感采集函数，在使用中需调用求最大值函数和该函数。
        归一化可要可不要
        没有用到！！！！！！！
返回值：无
日期： 2021-1-13    晚23:40
修改日期： 2021-3-22    晚22:00
作者：  混子    */
void  normalize_date(void)
{
 int16 JSADC_DATE[5];//这个数据类型必须是有符号的  用于计算储存
	unsigned char  i;
  for(i=0;i<5;i++)             //归一化处理
  {
		if(adc_date[i]<adc_min[i]) adc_min[i]=adc_date[i];//刷新最小值
		else if(adc_date[i]>adc_max[i]) adc_max[i]=adc_date[i];//刷新最大值
   JSADC_DATE[i]= (adc_date[i]-adc_min[i])*100/(adc_max[i]-adc_min[i]);         //归一化处理
   if(JSADC_DATE[i]<=0)
     JSADC_DATE[i]=0;
   else if(JSADC_DATE[i]>=100)
     JSADC_DATE[i]=100;
  }
  Left_Adc = JSADC_DATE[0];//左电感最终值
  Right_Adc = JSADC_DATE[1];//右电感最终值 
  Left_Xie_Adc = JSADC_DATE[2];//左斜电感最终值
  Right_Xie_Adc = JSADC_DATE[3];//右斜电感最终值
	Mid_Adc = JSADC_DATE[4];//中间电感最终值
  
}
/*****************圆环分析处理***********************
函数：void  annulus_analysis()
功能：判别环岛处理
参数：  无
说明：
返回值：
日期：
原理：非常简单，到圆环，判断到电感值增大，我们设定一个固定的速度低速，
      通过计数延时让车子再往前跑一点点距离（防止擦路肩），然后固定打角直接控制舵机打角，
      然后也是通过计数延时，车只要把车身拐进去了，我们就恢复正常循迹，不需要处理出环问题，出环后我们通过陀螺仪或者编码器或者延时把标志位清除，
      然后防止车子抖动，我们把转向pid参数调小很多，把速度恢复正常即可。
      在调试时我们只需调节那个打角的延时和继续行走的延时，其他都不用管

下面代码，有的变量或者语句相当于没有作用，可参考
仅供参考！！！！！！！！！

修改日期：2021-5-30
作者：那个混子    */
uint16 Out_Time = 0;//出环后清标志位，无需关心
uint16  T_annulus=0;//入环时间 无需关心
uint16  Corner_Limit=1000;//角速度积分限定 一般不需要修改（未使用陀螺仪无需关心）
uint16 Annulus_Distance_Limit = 360;//路程限定
uint16   T_Clear=200;//出环清标志位的时间，不能弄太小，以防清标志位太早二次识别  根据两个圆环的间距修改调整 //T_Clear=200

uint8 annulus_flag=0;           //环岛标志位
uint8 Left_Annulus=0;           //左环标志位
uint8 Direction_Annulus=0;          //右环标志位
uint8 annulus_success=0;        //进环成功
int32 corner=0 ;              //车子的拐角（通过陀螺仪积分）（未使用陀螺仪无需关心）
uint8 Round_Start =0;  ////拐角积分，编码器积分标志位
int32 Annulus_Distance=0;//圆环路程

void  Annulus_Analysis(void)
{

 if(annulus_success<2) //这里可以设置进圆环的次数
{
//预测圆环------------------------------------------1--------------------------//测试完毕
  if((Left_Adc>3700&&Right_Adc>3700&&annulus_flag==0&&Left_Xie_Adc>3700&&Right_Xie_Adc>3000&&Mid_Adc>2000&&T_ALready>175&&T_ALready<250) || 
		(Left_Adc>3700&&Right_Adc>3700&&annulus_flag==0&&Left_Xie_Adc>3700&&Right_Xie_Adc>3000&&Mid_Adc>2000&&T_ALready>500&&T_ALready<800) ) // if(Left_Adc>99&&Right_Adc>99&&Mid_Adc>99&&annulus_flag==0&&Switch3==0) //800
  {
		aim_speed = 210;//圆环固定速度
		if(Left_Xie_Adc>Right_Xie_Adc+100 && Right_Adc<3200)//区分左右环的 左环
		{//左环
			Direction_Annulus = 1;

		}
  else //右环
	{		Direction_Annulus = 0;	

	}		
    BUZZ_ON;
	 go_motor(-6000,-6000);
    annulus_flag=1; //圆环标志位
		
  }

//----------圆环入---------------------------------2---------------------//
 if( annulus_flag==1)    //10ms一次
{
  Round_Start =1;   //角速度积分或者编码器积分标志位
  T_annulus++;  //进去时间控制
	
                if(T_annulus>20&&T_annulus<150) //控制固定打角的时间50---100   非常重要！！！！！！！！！！！！ //if(T_annulus>20&&T_annulus<150)
                {
									BUZZ_OFF
	                     Turn_Suquence = 3;//圆环拐入的PID参数
                            if(Direction_Annulus==1)  //左环
                            {
                              // Steering_Control_Out(-290)  ;  // -250 负数左  正数右 这里是强制控制转向的！！！！！！！！！！！！
                                Right_Adc=Right_Xie_Adc-1000;
														
                                Left_Adc=Left_Xie_Adc+1000;
															
                           
														}
                        else if(!Direction_Annulus) //右环
                            {
                              //Steering_Control_Out(160)  ;//   180  负数左  正数右   这里是强制控制转向的！！！！！！！！！！！！
                                Right_Adc=Right_Xie_Adc+500;
                                Left_Adc=Left_Xie_Adc-1000;  //-1100
														}

                }
                else if(T_annulus>150)//拐进去后正常循迹
                {
                         annulus_flag = 2; //恢复正常 环中正常循迹
                         Turn_Suquence = 3;  //圆环特定的转向pid
                         Right_Adc=Right_Adc;
                         Left_Adc=Left_Adc;
	                      T_annulus=0;  //时间清零，以备后面使用
									      

                }
								

}

//---------出环处理-----------------------------------3-------------------//
if(annulus_flag==2)
{
  BUZZ_ON 
	T_annulus++;  //进去时间控制
	Turn_Suquence = 3;//圆环的PID参数 
	if(T_annulus>100&&T_annulus<200)             //if(T_annulus>100&&T_annulus<150)
	{
		if(Direction_Annulus==1)  //左环
    {
      Right_Adc=Right_Adc;
      Left_Adc=Left_Adc+200;
			//Steering_Control_Out(-290)  ;  // -250 负数左  正数右 这里是强制控制转向的！！！！！！！！！！！！
    }
   else if(Direction_Annulus==0) //右环
    {
      Right_Adc=Right_Adc + 1000;//Right_Adc+200;
      Left_Adc=Left_Adc;
    }
	}
                else if(T_annulus>200)   //拐进去后正常循迹
                {
									 annulus_flag =3;//出环标志位成立
									  Round_Start =2;//绕出来了
									BUZZ_OFF
									  Turn_Suquence = 3;


                }

}

/********进环过程对编码器陀螺仪等测量***************************/
if(Round_Start ==1)
{
	//本次因为没得编码器，没得陀螺仪，这里不用了，我表示很难搞，如果有其中之一会简单很多！！！！！！！

/*   //出环处理   该种方法使用陀螺仪   这里不使用了
//corner +=abs(MpuStart.gyro.z*0.01); //角速度积分
    if(corner>Corner_Limit) //绕了一圈                      &&(adc_date[0]+adc_date[1]>1800)
    {
      Round_Start =2;//绕出来了
     annulus_flag =3;//出环标志位成立
   }
*/
 /*     //出环处理  该种方法使用编码器
    Annulus_Distance +=real_speed*0.005;    //测距离  化为厘米为单位
    if(Annulus_Distance>800) //绕了一圈Annulus_Distance_Limit  800
    {
       Round_Start =2;//绕出来了
        annulus_flag =3;//出环标志位成立

   }
	*/
	
	
}


if(Round_Start ==2)
 { Out_Time++;}
//--------------圆环结束---------------------------4--------------------------//
   if(Out_Time>T_Clear)    //所有标志位清零  if(Out_Time>T_Clear)
   {


        //BUZZ_ON ;
        annulus_success++;
        annulus_flag=0;
        Round_Start =0;
        T_annulus=0;
        Out_Time=0;
        Annulus_Distance = 0;
        Turn_Suquence = 3;
      	aim_speed = Set_Speed;//圆环结束恢复速度
        Direction_Annulus =0;//
   }

}
}
/*****************************三岔口分析判断**************************
函数：void Sanc_Cha_Kou_Analysis()
功能：三岔口分析判断
参数：  无
说明：
返回值：无
日期：
修改日期：
作者：  混子
 *************************************************************************/
uint16 SanChaKou_T = 0;
uint8  SanChaKou_Once = 0;
int32 SanChakou_Length = 0;
uint8 SanChaKou_Number = 0;
void Sanc_Cha_Kou_Analysis(void)
{
  
//	if(Left_Adc<3850 && Right_Adc<1800&&Left_Adc>2400 && Right_Adc>800
//				&&Mid_Adc<3000&&Mid_Adc>1000&&SanChaKou_Once==0&&SanChaKou_Number<2&&annulus_flag==0&&T_ALready>200)
//	if(Left_Adc<3900 && Right_Adc<1850&&Left_Adc>2400 && Right_Adc>800
//				&&Mid_Adc<2950&&Mid_Adc>1000&&SanChaKou_Once==0&&SanChaKou_Number<2&&annulus_flag==0&&T_ALready>200)
      if(Left_Adc<3900 && Right_Adc<3500&&Left_Adc>3500 && Right_Adc>1900
				&&Mid_Adc<2000&&Mid_Adc>1000&&SanChaKou_Once==0&&SanChaKou_Number<2&&annulus_flag==0&&Left_Xie_Adc>1000&&Right_Xie_Adc>1000&& T_ALready>50 )
	{
        Dianci.SanChaKou_Flag=1;//标志位成立
        SanChaKou_Number ++;//识别到三岔口的次数
       BUZZ_ON;
       SanChaKou_Once = 1;
			  aim_speed = 210;

       if( SanChaKou_Once==1 ){SanChakou_Length= T_ALready;  SanChaKou_Once = 2;}//第一次识别到读取一次路程
    }
		
		
//		// 以下为强控部分
//    if(T_ALready>(SanChakou_Length+200))//跑出去一段距离后清标志位准备识别下一次200  坡道前猛加
//    {
//        //SanChaKou_Once = 0;
//			
//        aim_speed = 250 ;
//			
//    }
//		
//		if(T_ALready>(SanChakou_Length+80))//跑出去一段距离后清标志位准备识别下一次200  坡道前猛加
//    {
//        //SanChaKou_Once = 0;
//			BUZZ_ON;
//        aim_speed = Set_Speed + 80;
//			
//    }
//		
//		if(T_ALready>(SanChakou_Length+330))//跑出去一段距离后清标志位准备识别下一次200
//    {
//        //SanChaKou_Once = 0;
//			BUZZ_OFF;
//        aim_speed = Set_Speed;
//			
//    }
//强控三岔内坡道
//		if(T_ALready>520)//跑出去一段距离后清标志位准备识别下一次200  坡道前猛加
//    {
//        //SanChaKou_Once = 0;
//			BUZZ_ON;
//        aim_speed = Set_Speed + 150;
//			
//    }
//		
//		if(T_ALready>550)//跑出去一段距离后清标志位准备识别下一次200
//    {
//        //SanChaKou_Once = 0;
//			BUZZ_OFF;
//        aim_speed = Set_Speed;
//			
//    }
		  if(T_ALready>(SanChakou_Length+370)) 
			{
				SanChaKou_Once = 0;
				
			}

 //---------》》右转------
 if(SanChaKou_Number ==1)
 {
    if( Dianci.SanChaKou_Flag ==1)//10ms执行一次
    {
        SanChaKou_T++;
         go_motor(-3000,-3000);
          Right_Adc=3600;
          Left_Adc=300;
        if(SanChaKou_T>10)//    速度快了把此值加大
        {
            SanChaKou_T = 0;
            Dianci.SanChaKou_Flag = 0;//标志位清零


        BUZZ_OFF;
        }  
    }
		
 }
//-------》》》》》》左转-------------
 else if (SanChaKou_Number==2)
 {
     if( Dianci.SanChaKou_Flag ==1)//10ms执行一次
       {
           SanChaKou_T++;
          //go_motor(-3000,-3000);
          Right_Adc=300;
          Left_Adc=3600;
           if(SanChaKou_T>30)// 速度快了把此值加大100
           {
               SanChaKou_T = 0;
               Dianci.SanChaKou_Flag = 0;//标志位
              BUZZ_OFF;
           }
       }
			 BUZZ_ON;
        aim_speed = Set_Speed + 160;   //140 - 150强控速
			if(T_ALready>570)
			{
				BUZZ_OFF;
        aim_speed = Set_Speed;
			}
 }

}

  /****************保护**************
函数：void  Protect_Anticollision()
功能：
参数：  无
说明：  
返回值：
日期：2019-5-6
作者： 混子  */
void  Protect_Anticollision()
{
  if(Left_Adc<10&&Right_Adc<10)
   Dianci.AllInductance_Lose =1;
  if(Dianci.AllInductance_Lose) 
  {
    Flag.start_go = 0;
    while(1)
		{
        go_motor(0,0);
		}
    }
 
}  
  
/****************电磁的电机控制PWM输出**************
函数：void Electromagnetism_Control()
功能：
参数：  无
说明：  
返回值：PWM
日期：2021-5-30
作者： 那个混子  */

void Electromagnetism_Control()
{
   AD_Date_Fitier();                  //电感采集处理 冒泡排序滑动滤波
	 Annulus_Analysis();//圆环识别处理
	Sanc_Cha_Kou_Analysis();//三岔口识别处理
  /// normalize_date();                     //采集电感归一化  0--100 不能用，未找到原因，不要归一化啦
  // Protect_Anticollision();///保护
     
}
/*******************舵机初始化***************
函数：void init_PWM(void)
参数：  无
说明：  分母10000，使用，如需修改引脚修改对应宏定义即可
             	pwm_init(PWM0_P00, 100, 5000);     //初始化PWM0  使用引脚P0.0  输出PWM频率100HZ   占空比为百分之 5000/PWM_DUTY_MAX*100
//							PWM_DUTY_MAX在zf_pwm.h文件中 默认为10000
*
*注意，先调节舵机，如果舵机为SD05，则频率为200hz ,如果舵机为S3010,频率则为50hz
*频率确定后，先把占空比分母，即PWM_DUTY_MAX确定，一般无需修改了
*然后就开始调节舵机了，调占空比的分子，即调用的函数的最后那个参数，根据经验算一下，大概是1/20的占空比，然后往左往右慢慢试
*计算公式：中值占空比大概是7.5% （和频率精度都有关系） 20ms(1.5ms高电平)
返回值：无
日期：2021-5-28 上午 10:00
修改时间：2021-5-28 上午 10:00
作者： 混子    */

void init_Steer_PWM(void)
{
  	pwm_init(Steer_Pin, 50, Steer_Duty_Midle);     //初始化舵机  输出PWM频率50HZ，并设置中值
}
/****************舵机转向控制输出**************
函数：void Steering_Control_Out(int16 duty)
功能：
参数：  无
说明：   舵机转向控制    注意调好舵机中值后，左右极限也调出来，要修改上面的宏定义
返回值：PWM
日期：2021-5-28 上午 10:00
修改时间：2021-5-28 上午 10:00
作者： 混子    */
void Steering_Control_Out(int16 duty)
{
     duty = Steer_Duty_Midle + duty ;//在舵机中值的基础上进行偏移
   if (duty >= Steer_Duty_Max) duty = Steer_Duty_Max;
   else if(duty <= Steer_Duty_Min) duty = Steer_Duty_Min;
     pwm_duty(Steer_Pin, duty);
}


/*################调试专用#############################################
                                                      	DisableGlobalIRQ();		//关闭总中断
                                                             while(1)
                                                             {
                                                                 go_motor(0,0);
                                                                 BUZZ_OFF;
                                                             }
    //################调试专用#############################################*/
