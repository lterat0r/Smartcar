#ifndef _debug_h
#define _debug_h
#include "headfile.h"

#define BUZZ_ON            BUZZPin = 1;          //��������   
#define BUZZ_OFF           BUZZPin = 0;         //�ط�����

//�ⲿ��������
void datasend(void);
void display( int hang,int date);
void Switch_Key_init(void);          //���뿪�س�ʼ��
void Strategy_Slect(void) ;          //���뿪�ز���ѡ��
void  Key_Scan_Deal (void );          //����������
void BUZZ_DiDiDi(uint16 PinLV);              //�������εε�
void Test_Servo_Hardware(void);
void Test_Motor_Hardware (void);
void Test_Electric_Hardware (void);

//�ⲿ��������
extern unsigned char TiaoCan;            //���α�־λ
extern unsigned char TFT_SHOW;           //��Ļ����
extern unsigned char Switch1,Switch2,Switch3,Switch4,Switch5,Switch6;//����


#endif  
