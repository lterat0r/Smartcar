#ifndef __HEADFILE_H_
#define __HEADFILE_H_




#include "isr.h"
#include <string.h>
#include <stdio.h>
#include "intrins.h"
//------STC16F SDK��
#include "STC16Fxx.h"
#include "board.h"
#include "common.h"

//------��ɿƼ���Ƭ����������ͷ�ļ�
#include "zf_uart.h"
#include "zf_gpio.h"
#include "zf_iic.h"
#include "zf_adc.h"
#include "zf_spi.h"
#include "zf_tim.h"
#include "zf_pwm.h"
#include "zf_nvic.h"
#include "zf_exti.h"
#include "zf_delay.h"

//------��ɿƼ���Ʒ����ͷ�ļ�
#include "SEEKFREE_FONT.h"
#include "SEEKFREE_18TFT.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_ICM20602.h"
#include "SEEKFREE_TSL1401.h"
#include "SEEKFREE_IPS114_SPI.h"
#include "SEEKFREE_MPU6050.h"
#include "SEEKFREE_OLED.h"
#include "SEEKFREE_ABSOLUTE_ENCODER.h"
#include "SEEKFREE_WIRELESS.h"
#include "SEEKFREE_PRINTF.h"


//*****�Լ���ӵ�ͷ�ļ�*****************
#include  "speed.h"//�ٶȿ���
#include "fuse.h" //�ж��ܵ��ںϿ���
#include "Function.h" //���ܺ���
#include  "DirectioN.h"//���ת���Լ��������
#include "NewOther.h" //��Ҫ����������
#include "debug.h"//������صģ������ڡ����������뿪�صȣ�

/*****************ʹ�õ�Ӳ�����Ŷ���*********
Ϊ����ǿ����ļ����ԣ��󲿷ֳ�������ʹ�ú궨�壬���Ӳ���в���޸�����궨�弴�ɣ�
ע���޸�ʱ����go to������Ӧģ�����ճ�����ƣ���Ҫ�Լ���Ŷ��
*********************************************/


//������� 
  //MOTOR_MODE  0�Ļ�����·ģʽMOS������1�Ļ���һ·ģʽDRV����
//=====һ�������Ҫ��·PWMģʽ=====MOS����==
#define Left_Z_Pin     PWMA_CH1P_P60 //PWMB_CH2_P75   //PWMA_CH1P_P60 
#define Left_F_Pin     PWMA_CH2P_P62//PWMB_CH1_P74 //PWMA_CH2P_P62
#define Right_Z_Pin    PWMA_CH3P_P64//PWMB_CH3_P76  //PWMA_CH3P_P64
#define Right_F_Pin    PWMA_CH4P_P66//PWMB_CH4_P77  //PWMA_CH4P_P66
//=====һ�����ֻҪһ·PWMģʽ������һ�����ſ��Ʒ���=====DRV����==
#define Left_PWM_Pin   PWMA_CH4P_P66//PWMA_CH4P_P66            PWMA_CH1P_P60
#define Left_DIR_Pin   P6_4//P64                      P62          
#define Right_PWM_Pin  PWMA_CH2P_P62//PWMA_CH2P_P62            PWMA_CH3P_P64
#define Right_DIR_Pin  P6_0//P60                      P66

//�������   
#define Steer_Pin    PWMB_CH1_P74 //PWMA_CH4N_P17   //PWMB_CH1_P74 

//����������  ռ���˶�ʱ��TIM0��TIM3��TIM4  
#define Left_Ecoder_Pin1     CTIM0_P34  //CTIM3_P04   CTIM0_P34    //LSB����
#define Left_Ecoder_Pin2     P35        //P05         P30           //Dir��������
#define Right_Ecoder_Pin1    CTIM3_P04   //CTIM4_P06  CTIM3_P04     //LSB����
#define Right_Ecoder_Pin2    P53           //Dir��������

//��������     ռ�ö�ʱ��TIM4
#define UART_User    UART_4          //
#define UART_TX_Pin  UART4_TX_P53   //UART4_TX_P03   //
#define UART_RX_Pin  UART4_RX_P52  //UART4_RX_P02 

//����������
#define BUZZ_Pin    P7_7  
#define BUZZPin     P77

//ADC����ź�����5����У�ʵ���϶����ԣ���У����������޸ļ��ɣ�
#define LeftXie_ADC_Pin   ADC_P01    //ADC_P00
#define Left_ADC_Pin      ADC_P00   
#define Right_ADC_Pin     ADC_P05
#define RightXie_ADC_Pin  ADC_P06

#define Mid_ADC_Pin       ADC_P17   //ADC_P05

//�ɻɹ����� (��������ⲿ�жϴ������붨��ΪINT�������ţ��������ͨ������ֱ��д���ź�) 
#define Reed_Switch_Pin   INT0_P32   //P37  



// MPU6050����������  �뵽 #include "SEEKFREE_IIC.h"�ļ��޸�

#endif