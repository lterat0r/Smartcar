#include "fuse.h"
Body Flag;                  //�ṹ�嶨�����ؼ���־λ
int16 Turn_PWM = 0;   //����ת��PWM
/*******************PIT��ʱ�ж�******************
������void Fuse_result(void)      
���ܣ��ٶȻ���ת�򻷿���
������  ��
˵����
����ֵ����
���ڣ� 2021-5-30
���ߣ�  �Ǹ�����    */
unsigned char int_OK=0;//��ʼ���ɹ���־λ
void Fuse_result(void)      
{
  if(int_OK)                //��ʼ���ɹ�
  {
 			 // P62=0;// ����� ����  
        if(!Flag.start_go) // ������������  
       {
       go_motor(0,0);
       }
///////////////////////���¾��ǿ��Ƶ�ִ�д���//////////////////////
        else
        {
//5ms���� 
           if(1==Flag.T_Inmost)   
          {  
           Flag.T_Inmost = 0;
					//All_PWM_left = Speed_PWM-Turn_PWM*0.5;
					//All_PWM_right = Speed_PWM+Turn_PWM*0.5;
					All_PWM_left = Speed_PWM;
					All_PWM_right = Speed_PWM;
				if( !Dianci.SanChaKou_Flag)//�������·��
          go_motor(All_PWM_left,All_PWM_right);          //�������

          } 
      
//10ms���� ����
         if(1==Flag.T_Turn)
         {
            Flag.T_Turn=0;
         
				    Electromagnetism_Control();//��Ųɼ�����
					   // adc_deviation =Cha_BI_He(Left_Adc,Right_Adc,Mid_Adc,100);//�����в�Ⱥ� 
		      // adc_deviation =Cha_BI_He_Sqrt(Left_Adc,Right_Adc,1000)+Cha_BI_He(Left_Xie_Adc,Right_Xie_Adc,0,0);
					 adc_deviation =Cha_BI_He_Sqrt(Left_Adc,Right_Adc,900)+Cha_BI_He_Sqrt(Left_Xie_Adc,Right_Xie_Adc,100);;
		       adc_deviation = PlacePID_Control(&TurnPID,Turn_Pid[Turn_Suquence], adc_deviation, 0); //ת��̬PID   
           Turn_PWM =-adc_deviation;
          Steering_Control_Out(Turn_PWM);//�����������������ڲ����޷���
	    }
//50ms����	 �ٶ�			 
	    if(1==Flag.T_Speed)
            {
               Flag.T_Speed=0; 
                                         
            //Speed_PWM =  PID_Realize(&SpeedPID, Speed_Pid, real_speed,aim_speed);//�ٶ�λ��ʽPID
	          //Speed_PWM= range_protect(Speed_PWM, -1200, 1200);	// ע�������� 
						//���û�б�����
							//Speed_PWM = aim_speed*10;
							Speed_PWM = aim_speed*10;
            }
   }
  }
}
  




//���Ĺؼ��ı�־λ�ṹ���ʼ��
void Flag_Init()
{
	    Flag.start_go = 0;
      Flag.T_Inmost = 0;
      Flag.T_Turn=0;
      Flag.T_Speed=0; 
      Flag.STOP=0;//ͣ������
	    Flag.OUT_Garage = 0;//�����־λ
	    Flag.Game = 0;
}


