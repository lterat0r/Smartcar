#include "Standing.h"
//ת����icm_gyro_z
//ֱ����icm_gyro_y  icm_acc_x
//icm_acc_x -460----1550
//ֱ��PD
 uint16  K_P=300;
 uint16 K_D=20;
 
#define   CONSTANT      2     //����ϵ��  ��СҪ�ʵ�����D
#define   DT          0.0166 //����ϵ��      �����˸����ٶ�
#define   R_Gyro      0.04  //���ٶ�ϵ��0.4
#define   R_Acc       0.025  //�Ƕ�ϵ��
float Angle;
float  AngleIntegral=30.0;
int16  Angle_pwm=0;
 int16 aim_angle=0;//Ŀ��Ƕ�
/******************�Ƕȸ����ں�*************
���ƣ��ɼ��ĽǶȵĴ���
˵�����ú��������˶ԽǶȺͼ��ٶȽ��й��㴦����һ�������Ƕ��ں�
˼·�� �ɼ�ֵ������������λ����������һ�����������ں� ������������ �����������սǶ�
                                          �K    �J
                                            ������ͨ��ʵʱ���ٶ�����ֵĽǶ���һ�����ٻ�����
*/
void Complementary_Filter()
{ 	
    float  fDeltaValue = 0;                              //������
  ICM_Real.gyro.y = (ICMOffset.gyro.y - icm_gyro_y)*R_Gyro;//���ٶ�
  ICM_Real.acc.x =  (icm_acc_x-  ICMOffset.acc.x )*R_Acc;//�Ƕ�
	ICM_Real.gyro.z = (icm_gyro_y - ICMOffset.gyro.z );

  Angle = AngleIntegral;                                 //�����ںϽǶ�
  fDeltaValue = ( ICM_Real.acc.x - Angle) / CONSTANT;        //ʱ��ϵ��������������
  AngleIntegral += (ICM_Real.gyro.y + fDeltaValue) * DT; //���ٶȻ����ںϺ�ĽǶ� 
}

/****************���ݴ���******************
������void datasend()
������  ��
˵���� ����ͬʱ����6����  icm_gyro_x   icm_acc_x ICM_Real.gyro.y  ICM_Real.acc.z 
�����ı�����icm_acc_x  icm_gyro_y  Angle

����ֵ����
���ڣ�2020-7-10
���ߣ�  �Ǹ�����     */
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



