#include "NewOther.h"
 
/*=========================������⴦��================================================
 inout 1Ϊ��� 0Ϊ����
 lr     1Ϊ��   0Ϊ��
 ע�⣺����������ҳ��ض�Ϊ�ҽ�������������ض�Ϊ���
 ˵�����Ѿ��޸�Ϊʱ�䷽ʽ������ˣ�����֮ǰʹ�ñ�����·�̷��������ֺܶ��˲�����������������⣬�������취��!
���ڣ�2021-5-29-9:00
�޸�ʱ�䣺2021-5-29-9:00
���ߣ�  �Ǹ�����  
 *******************************************/
uint8 LEFT_RIGHT = 0;//       LEFT_RIGHT 1Ϊ��   0Ϊ��
uint32 T_ALready = 0;//ȫ�ּ�ʱ��һֱ�ڼ�ʱ��Ŷ����
void OutInGarage (unsigned char inout, unsigned char lr)
 {
     int16 T_New = 0;//��ǰʱ���ȡ
//------------------�ұ�-�ұ�-�ұ�-�ұ�-�ұ�---------�ұ߳��⡢������----------------------------------------------------------------
     if (lr)           // 1�ҳ����
     {
         if (inout)    // 1�����
         {
             T_New = T_ALready;//��ȡ��ǰʱ��
            
//             go_motor(-4000,-4000); 
//           	 delay_ms(1200);//���ÿ��Դ�����ʱ����
//					   go_motor(3000,2000); 
					 
					 while(P32==0)
					 {	
						 	 pwm_duty(Steer_Pin, Steer_Duty_Midle);//��������
						  go_motor(-2500,-2500); 
					 }
							delay_ms(250);
					   go_motor(-6000,-3000);   //go_motor(4000,-2000);
					    pwm_duty(Steer_Pin, Steer_Duty_Min);//�����Ҵ��
             while (T_ALready < T_New + 20)
             {
                  	delay_ms(10);//���ÿ��Դ�����ʱ����
             }
						 	 pwm_duty(Steer_Pin, Steer_Duty_Midle);//��������
						 go_motor(-3000,-3000); 
						 delay_ms(100);
             DisableGlobalIRQ();		//�ر����ж� 
             while (1)
             {
                 //speed_measure();  //�����ٶ�(��Ҫ�����������û�ñ����������)
							   //go_motor(-real_speed*1000,-real_speed*1000);//ɲ������Ҫ�����������û�ñ���������򿪣�
                delay_ms(10);//���ÿ��Դ�����ʱ����
                go_motor(0,0);//ֹͣ
            }                 // �����ϣ�����ͣ��
         }
		
         else  // 0�ҳ���
         {
             T_New = T_ALready;//��ȡ��ǰʱ��
             pwm_duty(Steer_Pin, Steer_Duty_Midle);//ֱ��
             go_motor(2000,2000);       //�̶��ٶȳ�ȥ
             while (T_ALready < T_New + 15)//����ʵ�ʵ����ʱ����Ƴ���ֱ�߾���
             {
                 	delay_ms(10);//���ÿ��Դ�����ʱ����
             }
             T_New = T_ALready;//��ȡ��ǰʱ��
             pwm_duty(Steer_Pin,Steer_Duty_Min); // ������Ҵ���Ϊ������׼��
              go_motor(2000,2000);       //�̶��ٶȳ�ȥ
             while (T_ALready < T_New + 12)  //(T_ALready < T_New + 20)
             {
                  	delay_ms(10);//���ÿ��Դ�����ʱ����
             }
         }
     }

//--------------------------���-���-���-------------------��߳��⡢������----------------------------------------------------------------
     else // 0�������⣻
        {
            if (inout) // 1�����
						{
             T_New = T_ALready;//��ȡ��ǰʱ��
            
					 
					 while(P32==0)//�˻����ٴμ�⵽�ɻɹܣ�֮ǰ��������ʱ���ƺ��ˣ�������ȷ���ô˷�����������
					 {
						 	 pwm_duty(Steer_Pin, Steer_Duty_Midle);//��������
						 // go_motor(-1500,-1500); 
					 }
					 delay_ms(250);
					   go_motor(-3000,-6000); 
					    pwm_duty(Steer_Pin, Steer_Duty_Max);//�����Ҵ��
             while (T_ALready < T_New + 21)
             {
                  	delay_ms(10);//���ÿ��Դ�����ʱ����
             }
						 	 pwm_duty(Steer_Pin, Steer_Duty_Midle);//��������
						 go_motor(2000,2000);  //1500 1500
						  delay_ms(50);
             DisableGlobalIRQ();		//�ر����ж� 
             while (1)
             {
                 //speed_measure();  //�����ٶ�(��Ҫ�����������û�ñ����������)
							   //go_motor(-real_speed*1000,-real_speed*1000);//ɲ������Ҫ�����������û�ñ���������򿪣�
                delay_ms(10);//���ÿ��Դ�����ʱ����
                go_motor(0,0);//ֹͣ
            }                 // �����ϣ�����ͣ��
            }
            else  // 0�����
            {
					   T_New = T_ALready;//��ȡ��ǰʱ��
             pwm_duty(Steer_Pin, Steer_Duty_Midle);//ֱ��
             go_motor(2000,2000);       //�̶��ٶȳ�ȥ
             while (T_ALready < T_New + 12)
             {
                 	delay_ms(10);//���ÿ��Դ�����ʱ����
             }
             T_New = T_ALready;//��ȡ��ǰʱ��
             pwm_duty(Steer_Pin,Steer_Duty_Max); // ����������Ϊ������׼��
              go_motor(2000,2000);       //�̶��ٶȳ�ȥ
             while (T_ALready < T_New + 12)
             {
                  	delay_ms(10);//���ÿ��Դ�����ʱ����
             }
       
            }
        }
 }
 
 /************ͣ����⴦��*****
 ������void STOP_Analyse ()
 ���ܣ�ʶ������߽���ͣ����־λ
 ������
 ˵����
 ����ֵ��
���ڣ�2021-5-29-9:00
�޸�ʱ�䣺2021-5-29-9:00
���ߣ�  �Ǹ�����  
 *******************************************/
 uint16 STOP_T_Count = 0;
 unsigned char Only_Once = 0;
 uint8 INT_Flag=0;//�ɻɹ��ⲿ�жϱ�־λ
 void STOP_Analyse (void)
 {
 if(Flag.start_go==1 && Flag.STOP==0&&annulus_flag==0&&Dianci.SanChaKou_Flag==0) 
 {
 if(INT_Flag)
 {
	 INT_Flag = 0;//����������
	   BUZZ_ON
      if(Only_Once==0) {STOP_T_Count= T_ALready;  Only_Once = 1;}//��һ��ʶ�𵽶�ȡһ��ʱ��
     if(( T_ALready>STOP_T_Count+10))//ʶ���һ�κ��ó����ܳ�ȥһ�ξ����ٽ�����һ�ε�ʶ��
         {
             Flag.STOP=1;
         }
 }
 }
//----------------�������-----------------------------------------------
 if(Flag.STOP == 1) //���
{
 int_OK =0;   //���㲻��ִ���ж�����������
 Flag.start_go = 0;//ֹͣ�ж���ִ��
 
 BUZZ_OFF
 OutInGarage (1, LEFT_RIGHT);

}

 }

 
/*################����ר��#############################################
                                                      	DisableGlobalIRQ();		//�ر����ж�
                                                             while(1)
                                                             {
                                                                 go_motor(0,0);
                                                                 BUZZ_OFF;
                                                             }
    //################����ר��#############################################*/


