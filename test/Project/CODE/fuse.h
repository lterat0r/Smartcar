#ifndef __FUSE_H__
#define __FUSE_H__
#include "headfile.h"

extern unsigned char   int_OK;//��ʼ���ɹ���־λ
extern int16   Turn_PWM ;   //����ת��PWM
//���Ĺؼ��ı�־λ�ṹ�嶨��
typedef struct 
{
      unsigned char start_go;           //��ʼ������
      
      unsigned char T_Inmost;           //���ڻ���������
      unsigned char T_Turn ;           //ת��������� 
      unsigned char T_Speed ;            //�ٶȿ�������
      unsigned char  STOP ;            //ͣ������
	    unsigned char OUT_Garage;     //�����־λ
	unsigned char Game;
    
}Body;
extern Body Flag;

/*********��������**************/
void Fuse_result(void)  ;
//���Ĺؼ��ı�־λ�ṹ���ʼ��
void Flag_Init();



#endif
