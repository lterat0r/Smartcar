#include "headfile.h"
#include  "speed.h"

/***********�ٶȸ���������**************/
int16  aim_speed,Set_Speed ;         //Ŀ���ٶ� 

int16 real_speed;        //ʵʱ�ٶ�
int16 Speed_Min;

int16 right_speed;       //�����ٶ�
int16 left_speed;        //�����ٶ�
int16 last_speed;        //��һ���ٶ�
int16 Last_right_speed;//�����ϴ��ٶ�
int16 Last_left_speed;//�����ϴ��ٶ�
int32 distance=0;          //�������ߵ�·��


int16 Speed_PWM=0;         //�ٶȻ�PWM
int16 All_PWM_left=0;     //����PWM
int16 All_PWM_right=0;    //����PWM


    
/*****************��������ʼ��*****************
������void encoder_init(void)
���ܣ���������ʼ��
������  ��
˵����ctimer_count_init(CTIM0_P34);
      ������ʹ��TIM3��TIM4�����������ֻ���޸ĺ궨�弴�� 
      ������ʹ�ô�����ı�����������֧���������룩
����ֵ��
���ڣ� 2021-3-25��00:59
�޸����ڣ�2021-3-26
���ߣ�  ����    */
void encoder_init(void)
{
   ctimer_count_init(Left_Ecoder_Pin1);//��ʼ����������ⲿ��ʱ��
	 Left_Ecoder_Pin2=1; //��ʼ����������ķ�������
	 ctimer_count_init(Right_Ecoder_Pin1);//��ʼ���ұ������ⲿ��ʱ��
   Right_Ecoder_Pin2=1;//��ʼ���ұ������ķ�������
}    
/*****************�ٶȲ���*****************
������void speed_measure(void)
���ܣ�����ʵʱ�ٶȣ������ֵ�ƽ���ٶ� �������ɼ�ֵ��
������  ��
˵����  �ú������Բ���ٶȣ���ȡ�ٶȺ������ٴθ�λ���㣬�Ѱ�����
����ֵ��
���ڣ� 2021-1-12-20:30
�޸����ڣ�2021-1-12-20:30
���ߣ� ����    */
void speed_measure(void)
{ 
////////////////////////���ֲ���//////////Right//////
	                      left_speed = ctimer_count_read(Right_Ecoder_Pin1);
                     ctimer_count_clean(Right_Ecoder_Pin1); 

                  
	
//////////////////// ���ֲ���/////////Left///////////////
                       right_speed = ctimer_count_read(Left_Ecoder_Pin1);
                    ctimer_count_clean(Left_Ecoder_Pin1); 
                    
//////////////////////�����������ʹ�������ȡ����/////////////////
                    if ( 1==Left_Ecoder_Pin2)
                      left_speed = -left_speed;
                    else    left_speed = left_speed;
                    if ( 1==Right_Ecoder_Pin2)
                      right_speed = -right_speed;
                    else    right_speed = right_speed;
                      
          


     real_speed = (right_speed+left_speed)/2;        // �ٶ�ƽ��ֵ
   //  Speed_Min = last_speed;
       ///  Speed_Min = range_protect(Speed_Min, -500, 500);
   // real_speed = limit(real_speed,2000); //�޷�����   
     last_speed=real_speed; 
               
    distance +=(right_speed+left_speed)*0.01;    //�����  ��Ϊ����Ϊ��λ        	
}           

/*********************�ٶ�PI********************
�����ο���û��ʹ�õ�
**************************************/

static float SpeedErrorInteg = 0;       //ƫ����� 
float I_Calculate=0;//����I����ֵ
float P_Calculate=0;//����P����ֵ
static int8 separate=1;    //���ַ���ϵ��
float Speed_P_I_Control(float *PID, int16 speed, int16 set_speed)//��Ϊ�Ƕȱ仯С����Ҫ�Ǹ�����
{   
    float  SpeedControlOut;//���ռ�����
    int16 SpeedError;	// ��ǰ���
   
    static uint8 k;
    static float PreError[10]={0};   
    SpeedError = set_speed - speed;  //�����ٶ�ƫ��
    SpeedErrorInteg=0;          //������
    for(k=9;k>0;k--)          //20����--��++�죩
    {  
     PreError[k]=PreError[k-1];
     SpeedErrorInteg+=PreError[k];
    }
     PreError[0]=SpeedError;
     SpeedErrorInteg+=PreError[0]; //������
    
     I_Calculate = separate * PID[KI] * SpeedErrorInteg;  //����I����ֵ
  
     P_Calculate=PID[KP]*SpeedError;//����P����ֵ
		
		/////////////////////
		 if(SpeedError<0)//���٣�
		 {
		    SpeedControlOut=0;
			 /// SpeedControlOut=-5;
			 
		 }
		 else
		 {
      SpeedControlOut = P_Calculate+I_Calculate;  // ������� 
		 }
		
		///////////////////////
                          
          
     return   SpeedControlOut;
     }

/******************** �����ʼ��***************
������void init_PWM(void)
������  ��
˵����  ��ĸ10000
		    pwm_init(PWMA_CH1P_P60, 10000, 0);     //��ʼ��PWM5  ʹ������P2.5  ���PWMƵ��10000HZ   ռ�ձ�Ϊ�ٷ�֮ pwm_duty / PWM_DUTY_MAX * 100
����ֵ����
���ڣ�2021-5-27-��23:00
�޸����ڣ�2021-5-27-��23:00
���ߣ� ����    */
unsigned char MOTOR_MODE=0;//�����޸�ɾ��
void init_PWM(unsigned char Motor_Set)
{
	MOTOR_MODE = Motor_Set;
	if (MOTOR_MODE==0) 
	{
//-----MOS����-----------
  pwm_init(Left_Z_Pin, 17000,0);//���ֳ�ʼ��
	pwm_init(Left_F_Pin, 17000,0);
	pwm_init(Right_Z_Pin, 17000,0);//���ֳ�ʼ��
	pwm_init(Right_F_Pin, 17000,0);
	}
		else
	{
//------DRV����-------------
	pwm_init(Left_PWM_Pin, 17000,0);//���ֳ�ʼ��
  gpio_mode(Left_DIR_Pin,GPO_PP);       // ����DRV��������ΪΪ�������
	pwm_init(Right_PWM_Pin, 17000,0);//���ֳ�ʼ��
	gpio_mode(Right_DIR_Pin,GPO_PP);       // ����DRV��������ΪΪ�������

	}
 
}
/****************************������**********************
������void go_motor (int16 left_PWM,int16 right_PWM)
������  int16 left_PWM,int16 right_PWM
˵����pwm_duty(PWMA_CH1P_P60, duty);
      ��ؽ����ӵĵ���߼��������һ������һ�����������������ڶ������������ҵ��
      ����Ĳ������Ϊ�����������ת����ֵ��ת����������
����ֵ����
���ڣ�2021-5-27-��23:00
�޸����ڣ�2021-5-27-��23:00
���ߣ� ����    */
#define Duty_Max  7000   //�޷����ֵ9500

void go_motor (int16 left_PWM,int16 right_PWM)
{
if  (MOTOR_MODE==0)
{	
//-------MOS����--------	
    if (left_PWM>0)           //����
  {
		left_PWM = left_PWM<=Duty_Max ? left_PWM : Duty_Max;
     pwm_duty(Left_Z_Pin,left_PWM);
     pwm_duty(Left_F_Pin,0);			 //��ת
   } 	
else 
 {
    left_PWM = left_PWM>=-Duty_Max ? (-left_PWM) : Duty_Max;  
     pwm_duty(Left_Z_Pin,0);	
     pwm_duty(Left_F_Pin,left_PWM);			 //��ת
  }

    if (right_PWM>0)           //����
  {
   right_PWM = right_PWM<=Duty_Max ? right_PWM : Duty_Max; 
	 pwm_duty(Right_Z_Pin,right_PWM);	
	 pwm_duty(Right_F_Pin,0);			 //��ת
   } 
else 
 {
    right_PWM = right_PWM>=-Duty_Max ? (-right_PWM) : Duty_Max;  
	  pwm_duty(Right_Z_Pin,0);	
	  pwm_duty(Right_F_Pin,right_PWM); //��ת
  }
}
else
{
//-------DRV����-----------
   if (left_PWM>0)           //����
  {
		left_PWM = left_PWM<=Duty_Max ? left_PWM : Duty_Max;
		 P64=0;			 
     pwm_duty(Left_PWM_Pin,left_PWM);//��ת
   } 	
else 
 {
    left_PWM = left_PWM>=-Duty_Max ? (-left_PWM) : Duty_Max;  
     P64=1;	
     pwm_duty(Left_PWM_Pin,left_PWM);//��ת
  }

    if (right_PWM>0)           //����
  {
   right_PWM = right_PWM<=Duty_Max ? right_PWM : Duty_Max;
   P60=0;			 
	 pwm_duty(Right_PWM_Pin,right_PWM);	//��ת		
	
   } 
else 
 {
    right_PWM = right_PWM>=-Duty_Max ? (-right_PWM) : Duty_Max;  
	  P60=1;
	  pwm_duty(Right_PWM_Pin,right_PWM); //��ת
  }
}

}

