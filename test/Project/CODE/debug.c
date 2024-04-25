#include "debug.h"
//===================================================ͨ����ص�==========================================================
//======================================================================================================================
/****************���ݴ���******************
������void datasend()
������  ��
˵���� ����ͬʱ����6����  icm_gyro_x   icm_acc_x ICM_Real.gyro.y  ICM_Real.acc.z   
�����ı�����icm_acc_x  icm_gyro_y  Angle  adc_date[0] Left_Adc

����ֵ����
���ڣ�2020-7-10
���ߣ�  �Ǹ�����     */
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

//====================================================��Ļ��ص�=(��ɾ��)=============================================================
//============================================================================================================================
//sprintf(temp," date20=%d",date);
//TFTSPI_P8X8Str(0,19,temp,BLACK,WHITE);break;

//==========================================================���뿪�ؼ��������=========================================================
//====================================================================================================================================

//���뿪�����ź궨��
#define Switch_Pin_1       P33
#define Switch_Pin_2       P32
#define Switch_Pin_3       P20
#define Switch_Pin_4       P20
#define Switch_Pin_5       P20
#define Switch_Pin_6       P20
//���尴������
#define KEY1    P70      
#define KEY2    P71      
#define KEY3    P72        
#define KEY4    P73      
#define KEY5    P20       
//***************�����궨��****(������Щ�������޸ĺ궨��Ϊ��Ӧ��GPIO�⺯������)***********
#define KEY_INT(key_x)           gpio_pull_set(key_x,PULLUP)//����Ϊ��������   
#define SWITCH_INT(switch_x)     gpio_pull_set(switch_x,PULLUP)//����Ϊ��������
#define READ_GPIO(Pin_X)         Pin_X
#define TiaoCan_DelayMs(M_S)     delay_ms(M_S)   //��ʱ

unsigned char TiaoCan=0;////////////////////////���α�־λ
unsigned char TFT_SHOW=0;///////////////////////��Ļ����
unsigned char Switch1=0,Switch2=0,Switch3=0,Switch4=0,Switch5=0,Switch6=0;//����
char parameter=0;//����ѡ��

//����״̬����
uint8 key1_status = 1,key2_status = 1,key3_status = 1, key4_status = 1,key5_status = 1;
//��һ�ο���״̬����
uint8 key1_last_status, key2_last_status, key3_last_status, key4_last_status,key5_last_status;
//���ر�־λ
uint8 key1_flag=0,key2_flag=0,key3_flag=0, key4_flag=0,key5_flag=0;
/*****************���뿪�ؼ�������ʼ��*****************
������void Switch_Key_init()
���ܣ���ʼ��IO
������  ��
˵���� ��ʼ��IO��   gpio_init(D1, GPI, GPIO_HIGH, GPI_PULL_UP); GPO_PUSH_PULL
����ֵ����
���ڣ�3��23
�޸����ڣ�2021-3-24
���ߣ�   �Ǹ�����  */
void Switch_Key_init()
{

    //���뿪�س�ʼ��  �������޸ģ������޸ģ�
	    SWITCH_INT(Switch_Pin_1) ;
		  SWITCH_INT(Switch_Pin_2) ;
		  SWITCH_INT(Switch_Pin_3) ;
		  SWITCH_INT(Switch_Pin_4) ;
		  SWITCH_INT(Switch_Pin_5) ;
		  SWITCH_INT(Switch_Pin_6) ;
    
    //������ʼ�� �������޸ģ������޸ģ�
   KEY_INT(KEY1);
	 KEY_INT(KEY2);
	 KEY_INT(KEY3);
	 KEY_INT(KEY4);
	 KEY_INT(KEY5);

}


/*****************���뿪�ز���ѡ��*****************
������void Strategy_Slect()
���ܣ�ͨ�����뿪�ص�������
������  ��
˵����  6λ���뿪�أ���������ӻ��߼��ٿɶ����޸�,�������6��Ҳ��Ҫɾ������ģ��������������Ÿĸ�û�õļ���
        ʹ���㶨��ľͺ��ˣ�����û���õ����������
����ֵ����
���ڣ�2021-3-23
�޸����ڣ�2021-3-24
���ߣ��Ǹ�����       */
void Strategy_Slect()
{
  //��ȡ���뿪��״̬
      if(!READ_GPIO(Switch_Pin_1))//��
      {
       Switch1=1;
       TFT_SHOW = 1;

      }
      if(!READ_GPIO (Switch_Pin_2))//��
      {
       Switch2=1;
       LEFT_RIGHT=0;//����������
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

    if(Switch1||Switch2||Switch3||Switch4||Switch5||Switch6)//�������뿪��
      {

      }

}

/*****************����ɨ���ȡ*****************
������void  Key_Scan_Deal ()
���ܣ���ȡ������ִ�ж�Ӧ����
������  ��
˵���� �ο�������� ��5λ��������������ӻ��߼��ٿɶ����޸�
      // 1��Ϊ���Ƽ���2��Ϊ�ϼ���3��Ϊ���Ƽ���4��Ϊ�м��̣�5��Ϊ�¼�
     //���γ���û��ʹ�õ��Σ�stc��Ƭ�����س���Ҳ�죬�����վͿ��ԣ����Ҫ�ӵĻ��Լ�����������Լ��ӾͿ���
����ֵ����
���ڣ�2021-3-23
�޸����ڣ�2021-3-23
���ߣ��Ǹ�����       */
uint8 gogo=0;
void  Key_Scan_Deal ()
{
    //ʹ�ô˷����ŵ����ڣ�����Ҫʹ��while(1) �ȴ������⴦������Դ�˷�
    //���水��״̬
    key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;
    key4_last_status = key4_status;
    key5_last_status = key5_status;
    //��ȡ��ǰ����״̬
    key1_status = READ_GPIO(KEY1);
    key2_status = READ_GPIO(KEY2);
    key3_status = READ_GPIO(KEY3);
    key4_status = READ_GPIO(KEY4);
    key5_status = READ_GPIO(KEY5);
    //��⵽��������֮��  ���ſ���λ��־λ
    if(key1_status && !key1_last_status)    key1_flag = 1;
    if(key2_status && !key2_last_status)    key2_flag = 1;
    if(key3_status && !key3_last_status)    key3_flag = 1;
    if(key4_status && !key4_last_status)    key4_flag = 1;
    if(key5_status && !key5_last_status)    key5_flag = 1;
    //��־λ��λ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�
 
           if(key1_flag&&(gogo==1||gogo==2)) //�ϼ�
           {
               key1_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
           /*  ����Ϊ�û�����  */
                      switch(parameter)
                      {
              //-----------------------�������޸�����--��ע���޸Ķ�Ӧ����ʾ��----------------------------------------------------------------
                     //��һҳ��ʾ�Ķ���
                      case 0: aim_speed+=5; break;
                      case 1:Turn_Pid[1][0]+=0.1;    break;
                      case 2:Turn_Pid[1][1]+=0.5;   break;
                      case 3:Turn_Pid[1][2]+=0.1;   break;
                      case 4: break;
                      case 5:       break;

                     /// case 6:  ; break;//������ܼ��κβ����������ˣ���ҳʹ����

                      //�ڶ�ҳ��ʾ�Ķ���
                      case 7: Speed_Pid[0]+=0.1 ; break;
                      case 8: Speed_Pid[0]+=0.1 ; break;
                      case 9: Speed_Pid[0]+=0.1 ; break;
                      case 10:  ; break;
                      case 11:  ; break;
                      case 12:  ; break;
              //--------------------�������޸�����------------------------------------------------------------------
                      }
                      /*  ����Ϊ�û�����  */
              

           }

           if(key2_flag&&(gogo==1||gogo==2))//�¼�
           {
               key2_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
                /*  ����Ϊ�û�����  */
                      switch(parameter)
                      {
                //----------------------�������޸�����--��ע���޸Ķ�Ӧ����ʾ��--------------------------------------------------------------
                          //��һҳ��ʾ�Ķ���
                         case 0: aim_speed-=5; break;
                         case 1:Turn_Pid[1][0]-=0.1;    break;
                         case 2:Turn_Pid[1][1]-=0.5;   break;
                         case 3:Turn_Pid[1][2]-=0.1;   break;
                         case 4:break;
                         case 5:  break;

                        /// case 6:  ; break;//������ܼ��κβ����������ˣ���ҳʹ����

                         //�ڶ�ҳ��ʾ�Ķ���
                         case 7:Speed_Pid[0]-=0.1; break;
                         case 8:Speed_Pid[1]-=0.1  ; break;
                         case 9:Speed_Pid[2]-=0.1  ; break;
                         case 10:  ; break;
                         case 11:  ; break;
                         case 12:  ; break;
              //--------------------�������޸�����------------------------------------------------------------------
                      }
                      /*  ����Ϊ�û�����  */
              
           }

           if(key3_flag&&(gogo==1||gogo==2))//���
           {
               key3_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
               /*  ����Ϊ�û�����  */
                      parameter--;
               /*  ����Ϊ�û�����  */

           }

           if(key4_flag&&(gogo==1||gogo==2))//�Ҽ�
           {
               key4_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
               /*  ����Ϊ�û�����  */
                      parameter++;
               /*  ����Ϊ�û�����  */

           }
            if(key5_flag) //�м�
           {
              key5_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
            /*  ����Ϊ�û�����  */
						

						/*  ����Ϊ�û�����  */

           }
     if(gogo==6)//����������ν���������������������������
			{

					TiaoCan = 1;      //���ν�����־λ

			}
											
											       //lcd_showfloat(0,5,Speed_Pid[0],2,1);//��ʾ������   ������ʾ2λ   С����ʾ��λ
           //*******************************��Ļ��ʾ��һҳ***********************

    if(parameter<6&&gogo>=1)//��ʾ����0��5��ʵ����ʾ1��6
            {
          
            }
           //*******************************��Ļ��ʾ�ڶ�ҳ**************************************************
           if(parameter>6&&parameter<13)//�����кŴ�4��9   һҳ��6������  //��ʾ����7��5��ʵ����ʾ7��12
           {
          
            }
           //*******************************��Ļ��ʾ����ҳ**************************************************
                   if(parameter>13&&parameter<20)
                   {
                  
                    }
           //###########����Ҫ����ҳ������д�Ϳ��Կ�######################����Ͳ�д�� ����
                   if(parameter==6||parameter==13||parameter==20)  //��ҳ׼��
                   {
                     
                   }//����





}


//===============================================���������=============================================
//=====================================================================================================

//���������͹� ��д�ڶ�Ӧͷ�ļ�ȥ�ˣ�ȥH�ļ��鿴

/*****************�������εε�*****************
������void BUZZ_DiDiDi()
���ܣ��������εε�
������  ��
˵����
����ֵ����
���ڣ�2021-3-23
�޸����ڣ�2021-3-23
���ߣ� �Ǹ�����      */
void BUZZ_DiDiDi(uint16 PinLV)
{
  BUZZ_ON;
  TiaoCan_DelayMs(PinLV);
  BUZZ_OFF;
}


/***************************�������**********************************************
 *  �������ƣ�Test_Servo(void)
 *  ����˵�������PWM��ʼ�������Ա궨���PWM����SD5/S3010���
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺
 *  ��    ע���ο�������ṩ�ģ�������
 ��ע�����ע�⣬һ��Ҫ�Զ����ǽ�������
 ʹ������ĸ��������̣�
 1.��ʹ�����ñ������ص�ѹ����ر�֤��ص�ѹ��7V���ϣ�������������Ӧ��
 2.Ȼ��ȷ����������ѹ��SD5�����5V���磬S3010��6-7V���磬SD012�����5V���磡����
 3.�Ѷ���Ķ���ȥ�����ö����������ת����
 4.��д�������У��ö��ת������ֵ���������û��Ӧ�ظ�1-2�������ߵ��������PWMƵ�ʼ�ռ�ձȣ����ܿ�Ϊ׼��
 5.����ܿغ�������ת�������֨֨�죬�Կ�ת������ʱ����װ�϶��̣�
 6.����K0/K1ȷ�����������ת�����ޣ�������������Ϊ�����޷���ֹ�����ת�ջ٣�
 *************************************************************************/
void Test_Servo_Hardware (void)
{
    char txt[16] = "X:";
    unsigned int  duty = Steer_Duty_Midle;

    oled_fill(0x00);  //��ʼ����
	lcd_showstr(0, 0, "Test_Servo_Hardware:");
   	pwm_init(Steer_Pin, 50, Steer_Duty_Midle);     //��ʼ�����  ���PWMƵ��200HZ����������ֵ
    pwm_duty(Steer_Pin, Steer_Duty_Midle);
    while (1)
    {
        if (!READ_GPIO(KEY1))
        {
            if (duty > 100)  		//��ֹduty��
            {
                duty += 10;     //�궨��ʱ�򣬿��԰Ѳ�����С�㣬����10
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
				oled_p6x8str(1, 2, txt); //��ʾ
				TiaoCan_DelayMs(100);        
    }
}

/****************************�������*********************************************
 *  �������ƣ�TestMotor(void)
 *  ����˵�������Ա궨���PWM���Ƶ��
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺
 *  ��    ע������2�����
 ��ע�����ע�⣬һ��Ҫ�Ե�������������
 ʹ������ĸ��������̣�
 1.��ʹ�����ñ������ص�ѹ����ر�֤��ص�ѹ��7V���ϣ�������������Ӧ��
 2.�Ӻ�ĸ�嵽��������ź��߼���Դ�ߣ�
 3.�Ӻ������嵽����ĵ��ߣ�
 4.��д�������У�ȷ�����������ת���󣬿����������Դ���أ�
 5.����K0/K1ȷ�����ת���ٶȼ�����
 6.������ַ�ת������K1�����ص���ģʽ������ֱ�ӹر��������Դ��
 *************************************************************************/
void Test_Motor_Hardware (void)
{
    short duty = 2000;
    oled_fill(0x00);  //��ʼ����
	lcd_showstr(2, 0, "Test_Motor_Hardware:");
   init_PWM(0);
    while (1)
    {
    if (!READ_GPIO(KEY1))   //����KEY0��   ���ֵ�����ת
        {
           	 go_motor (duty,0);
					 lcd_showstr(0, 4, "Left  Front");   //�ַ�����ʾ
        }
       if (!READ_GPIO(KEY2)) //����KEY2����������ͬʱ��ת
        {
          	 go_motor (-duty,-duty);
					 lcd_showstr(0, 4, "All  Black");   //�ַ�����ʾ
           	  	 
        }
      if (!READ_GPIO(KEY3))      //����KEY1��  ���ֵ�����ת
        {
           	 go_motor (0,duty);
						 lcd_showstr(0, 4, "Right Front");   //�ַ�����ʾ
        }
				if((READ_GPIO(KEY1))&&(READ_GPIO(KEY2))&&(READ_GPIO(KEY3)))
        go_motor (0,0);
      	TiaoCan_DelayMs(100);  
    }
}

/****************************�������*********************************************
 *  �������ƣ�void Test_Electric_Hardware (void)
 *  ����˵�������Ե�ŵ��Ӳ��
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2021-5-30
 *  ��    ע��
 ��ע�����
 *************************************************************************/
void Test_Electric_Hardware (void)
{
	 char txt[16];
	oled_fill(0x00);  //��ʼ����
	lcd_showstr(2, 0, "Test_Electric_Hardware:");
	ADC_Int();
	while(1)
	{
			//datasend();
		 if (!READ_GPIO(KEY2)) //����KEY2����������ͬʱ��ת
        {
					 lcd_showstr(2, 1, "Normalize_Deal....");   //�ַ�����ʾ
					AD_Date_Fitier();
					normalize_date();
					sprintf(txt,"adc0= %05f",adc_date[0]);
				lcd_showstr(1, 2, txt); //��ʾ
					sprintf(txt,"adc1= %05f",adc_date[1]);
				lcd_showstr(1, 3, txt); //��ʾ
					sprintf(txt,"adc2= %05f",adc_date[2]);
				lcd_showstr(1, 4, txt); //��ʾ
					sprintf(txt,"adc3= %05d",adc_date[3]);
				lcd_showstr(1, 5, txt); //��ʾ
					sprintf(txt,"adc4= %05d",adc_date[4]);
				lcd_showstr(1, 6, txt); //��ʾ					
           	  	 
        }
				else
				{
					
				
		 lcd_showstr(2, 1, "                   ");   //�ַ�����ʾ
     	AD_Date_Fitier();
//		  adc_date[0] = adc_mean_filter (Left_ADC_Pin,ADC_12BIT,20); 
//      adc_date[1] = adc_mean_filter  (Right_ADC_Pin,ADC_12BIT,20); 
//      adc_date[2] = adc_mean_filter  (LeftXie_ADC_Pin,ADC_12BIT,20); 
//      adc_date[3] = adc_mean_filter  (RightXie_ADC_Pin,ADC_12BIT,20); 
//      adc_date[4] = adc_mean_filter  (Mid_ADC_Pin,ADC_12BIT,2); 
		  sprintf(txt,"adc0= %05d",adc_date[0]);
		lcd_showstr(1, 2, txt); //��ʾ
			sprintf(txt,"adc1= %05d",adc_date[1]);
		lcd_showstr(1, 3, txt); //��ʾ
			sprintf(txt,"adc2= %05d",adc_date[2]);
		lcd_showstr(1, 4, txt); //��ʾ
		  sprintf(txt,"adc3= %05d",adc_date[3]);
		lcd_showstr(1, 5, txt); //��ʾ
		  sprintf(txt,"adc4= %05d",adc_date[4]);
		lcd_showstr(1, 6, txt); //��ʾ
          	}
}
	
}



