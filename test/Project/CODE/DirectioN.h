#ifndef __DirectioN_H__
#define __DirectioN_H__
#include "headfile.h"


//������ֵ
#define Steer_Duty_Max  810//
#define Steer_Duty_Midle 730  //750 //��ֵ700
#define Steer_Duty_Min  650//
//--ȫ�ֱ�������---

//---------��������-----------
extern uint8 Caiji_Flag;///��Ųɼ���־λ
extern uint16  adc_date[5];                //�����вɼ�ֵ
extern uint16  adc_max[5];               //���ֵ
extern int16  ADC_Mid[3] ;                //����������е�����
extern int16 adc_deviation;
extern int32 corner;              //���ӵĹսǣ�ͨ�������ǻ��֣�
extern uint8 annulus_flag;
extern uint8 annulus_success;        //�����ɹ�
extern  int16  Left_Adc,Right_Adc,Mid_Adc,Left_Xie_Adc,Right_Xie_Adc;//���ֵ

//��Ų��ֽṹ�����
typedef struct   
{
  uint8 LeftInductance_Lost; //����ǿ�ȶ�ʧ
  uint8 RightInductance_Lost; //�ҵ��ǿ�ȶ�ʧ
  uint8 AllInductance_Lose;//���е�Ŷ�ʧ//////��ײ������־λ
  int16 Distance;         //���Բ�����
	uint8 SanChaKou_Flag;
	
}Electromagnetism;
extern Electromagnetism  Dianci;



//-----��������---
void init_Steer_PWM(void);
void Steering_Control_Out(int16 duty);

void ADC_Int();    //���ADCͨ����ʼ��
void ADC_MAX_Collect(void);//��ֵ�ɼ�
void AD_Date_Fitier();/////////��л����˲�
void  normalize_date(); //��й�һ��
 void STOP_Analyse (void);
void  Protect_Anticollision();//����
void Electromagnetism_Control();




#endif