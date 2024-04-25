#include  "Direction.h"
//-----ǰ���ǵ��������ص�--------
//----����Ƕ����ص�------------
uint16  adc_date[5];                //�����вɼ�ֵ ԭʼֵ   5�����
uint16 leftleft = 0;
int16  Left_Adc=0,Right_Adc=0,Mid_Adc=0,Left_Xie_Adc=0,Right_Xie_Adc=0;//���ֵ

uint16  adc_max[5]={2000,2000,2200,2200,2000};               //���ֵ
uint16  adc_min[5]={0,0,100,100,100};               //��Сֵ  (����һ����Ҫ)

int16  adc_deviation;            //���ƫ��
Electromagnetism  Dianci;////////////////////��Ų��ֽṹ�����

/****************ADC��ʼ��**************************
������void ADC_Int()
���ܣ�ADCͨ�����ų�ʼ��
������  ��
˵���� 	adc_init(ADC_P10,ADC_SYSclk_DIV_2);//��ʼ��P1.0ΪADC����,ADCʱ��Ƶ�ʣ�SYSclk/2
       
���ţ�                                                                                                
����ֵ����
���ڣ�2021-5-28-19:30
�޸�ʱ�䣺2021-5-28-19:30
���ߣ�  �Ǹ�����     
*/
void ADC_Int()
{
	
	adc_init(Left_ADC_Pin,ADC_SYSclk_DIV_2);//��ʼ��ADC����
	adc_init(Right_ADC_Pin,ADC_SYSclk_DIV_2);//��ʼ��ADC����
	adc_init(LeftXie_ADC_Pin,ADC_SYSclk_DIV_2);//��ʼ��ADC����
	adc_init(RightXie_ADC_Pin,ADC_SYSclk_DIV_2);//��ʼ��ADC����
	adc_init(Mid_ADC_Pin,ADC_SYSclk_DIV_2);//��ʼ��ADC����

}

/****************��вɼ�**************************
������void ADC_MAX_Collect(void)
���ܣ�
������  ��
˵����  ���ÿ⺯�����Լ�д�ģ�    adc_mean_filter (ADC_P10, ADC_12BIT, 10)		
����ֵ����
���ڣ�2021-5-28-19:30
�޸�ʱ�䣺2021-5-28-19:30
���ߣ�  �Ǹ�����  */
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
     for(j=0;j<3;j++)//��������
	{
	    if(adc_date[j]>=adc_max[j])
	        adc_max[j]=adc_date[j];
	  //  if(adc_date[j]<=adc_min[j])
        // adc_min[j]=adc_date[j];
	      delay_ms(2);//��ʱ�ɼ�
	}

    }
}
/****************��вɼ�**************************
������void AD_Date_Fitier()
���ܣ��Ե�в�ֵ����ð�����򻬶��˲�
������  ��
˵����12bit  4096   ���ÿ⺯�����Լ�д�ģ�    adc_mean_filter (ADC_P10, ADC_12BIT, 10)	
����ֵ����
���ڣ�2021-5-28-19:30
�޸�ʱ�䣺2021-5-28-19:30
���ߣ�  �Ǹ�����  */

#define FILTER_N 5 //�˲����
 void AD_Date_Fitier()
{
   uint8 i;
  int16 filter_buf_L[FILTER_N];  //����д������� 
  int16 filter_buf_LC[FILTER_N]; //��б��д�������
  int16 filter_buf_M[FILTER_N];  //�к��д������� 
  int16 filter_buf_R[FILTER_N];  //�Һ��д�������
  int16 filter_buf_RC[FILTER_N];  //��б��д�������
  
 
//--------�����˲�--------------
	for(i = 0; i <FILTER_N; i++)//��ֵ 
  {
       filter_buf_L[i]  =   adc_mean_filter (Left_ADC_Pin,ADC_12BIT,10); //���
       filter_buf_LC[i] =   adc_mean_filter  (LeftXie_ADC_Pin,ADC_12BIT,5); //��б  
       
       filter_buf_M[i]  =   adc_mean_filter  (Mid_ADC_Pin,ADC_12BIT,5);//�м� 
       
       filter_buf_RC[i] =  adc_mean_filter  (RightXie_ADC_Pin,ADC_12BIT,10);    //��б   
       filter_buf_R[i]  =  adc_mean_filter  (Right_ADC_Pin,ADC_12BIT,5);   //�Һ�  
   }
//--------ð������ȥ��ֵ��ƽ��---------
  adc_date[0]= I_Median_Average_Filter(filter_buf_L);//��      3600
  adc_date[1] =I_Median_Average_Filter(filter_buf_R);//��      3600
  adc_date[2] = I_Median_Average_Filter(filter_buf_LC);//��б  3000
  adc_date[3] = I_Median_Average_Filter(filter_buf_RC);//��б
  adc_date[4] = I_Median_Average_Filter(filter_buf_M);//�м�   4050
	  
	 
	 
  Left_Adc = adc_date[0];//��������ֵ
  Right_Adc = adc_date[1];//�ҵ������ֵ 
  Left_Xie_Adc = adc_date[2];//��б�������ֵ
  Right_Xie_Adc = adc_date[3];//��б�������ֵ
	Mid_Adc = adc_date[4];//�м�������ֵ
 

}

/*****************��һ������***********************
������void  normalize_date()
���ܣ��ɼ����ֵ�����й�һ����С��
������  ��
˵����  �ڸú����е��õ�вɼ���������ʹ��������������ֵ�����͸ú�����
����ֵ����
���ڣ�2021-5-28-19:30
�޸�ʱ�䣺2021-5-28-19:30
���ߣ�  �Ǹ�����  */
/*****************��һ������***********************
������void  normalize_date()
���ܣ��ɼ����ֵ�����й�һ����С��
������  ��
˵����  �ڸú����е��õ�вɼ���������ʹ��������������ֵ�����͸ú�����
        ��һ����Ҫ�ɲ�Ҫ
        û���õ���������������
����ֵ����
���ڣ� 2021-1-13    ��23:40
�޸����ڣ� 2021-3-22    ��22:00
���ߣ�  ����    */
void  normalize_date(void)
{
 int16 JSADC_DATE[5];//����������ͱ������з��ŵ�  ���ڼ��㴢��
	unsigned char  i;
  for(i=0;i<5;i++)             //��һ������
  {
		if(adc_date[i]<adc_min[i]) adc_min[i]=adc_date[i];//ˢ����Сֵ
		else if(adc_date[i]>adc_max[i]) adc_max[i]=adc_date[i];//ˢ�����ֵ
   JSADC_DATE[i]= (adc_date[i]-adc_min[i])*100/(adc_max[i]-adc_min[i]);         //��һ������
   if(JSADC_DATE[i]<=0)
     JSADC_DATE[i]=0;
   else if(JSADC_DATE[i]>=100)
     JSADC_DATE[i]=100;
  }
  Left_Adc = JSADC_DATE[0];//��������ֵ
  Right_Adc = JSADC_DATE[1];//�ҵ������ֵ 
  Left_Xie_Adc = JSADC_DATE[2];//��б�������ֵ
  Right_Xie_Adc = JSADC_DATE[3];//��б�������ֵ
	Mid_Adc = JSADC_DATE[4];//�м�������ֵ
  
}
/*****************Բ����������***********************
������void  annulus_analysis()
���ܣ��б𻷵�����
������  ��
˵����
����ֵ��
���ڣ�
ԭ���ǳ��򵥣���Բ�����жϵ����ֵ���������趨һ���̶����ٶȵ��٣�
      ͨ��������ʱ�ó�������ǰ��һ�����루��ֹ��·�磩��Ȼ��̶����ֱ�ӿ��ƶ����ǣ�
      Ȼ��Ҳ��ͨ��������ʱ����ֻҪ�ѳ���ս�ȥ�ˣ����Ǿͻָ�����ѭ��������Ҫ����������⣬����������ͨ�������ǻ��߱�����������ʱ�ѱ�־λ�����
      Ȼ���ֹ���Ӷ��������ǰ�ת��pid������С�ܶ࣬���ٶȻָ��������ɡ�
      �ڵ���ʱ����ֻ������Ǹ���ǵ���ʱ�ͼ������ߵ���ʱ�����������ù�

������룬�еı�����������൱��û�����ã��ɲο�
�����ο�������������������

�޸����ڣ�2021-5-30
���ߣ��Ǹ�����    */
uint16 Out_Time = 0;//���������־λ���������
uint16  T_annulus=0;//�뻷ʱ�� �������
uint16  Corner_Limit=1000;//���ٶȻ����޶� һ�㲻��Ҫ�޸ģ�δʹ��������������ģ�
uint16 Annulus_Distance_Limit = 360;//·���޶�
uint16   T_Clear=200;//�������־λ��ʱ�䣬����Ū̫С���Է����־λ̫�����ʶ��  ��������Բ���ļ���޸ĵ��� //T_Clear=200

uint8 annulus_flag=0;           //������־λ
uint8 Left_Annulus=0;           //�󻷱�־λ
uint8 Direction_Annulus=0;          //�һ���־λ
uint8 annulus_success=0;        //�����ɹ�
int32 corner=0 ;              //���ӵĹսǣ�ͨ�������ǻ��֣���δʹ��������������ģ�
uint8 Round_Start =0;  ////�սǻ��֣����������ֱ�־λ
int32 Annulus_Distance=0;//Բ��·��

void  Annulus_Analysis(void)
{

 if(annulus_success<2) //����������ý�Բ���Ĵ���
{
//Ԥ��Բ��------------------------------------------1--------------------------//�������
  if((Left_Adc>3700&&Right_Adc>3700&&annulus_flag==0&&Left_Xie_Adc>3700&&Right_Xie_Adc>3000&&Mid_Adc>2000&&T_ALready>175&&T_ALready<250) || 
		(Left_Adc>3700&&Right_Adc>3700&&annulus_flag==0&&Left_Xie_Adc>3700&&Right_Xie_Adc>3000&&Mid_Adc>2000&&T_ALready>500&&T_ALready<800) ) // if(Left_Adc>99&&Right_Adc>99&&Mid_Adc>99&&annulus_flag==0&&Switch3==0) //800
  {
		aim_speed = 210;//Բ���̶��ٶ�
		if(Left_Xie_Adc>Right_Xie_Adc+100 && Right_Adc<3200)//�������һ��� ��
		{//��
			Direction_Annulus = 1;

		}
  else //�һ�
	{		Direction_Annulus = 0;	

	}		
    BUZZ_ON;
	 go_motor(-6000,-6000);
    annulus_flag=1; //Բ����־λ
		
  }

//----------Բ����---------------------------------2---------------------//
 if( annulus_flag==1)    //10msһ��
{
  Round_Start =1;   //���ٶȻ��ֻ��߱��������ֱ�־λ
  T_annulus++;  //��ȥʱ�����
	
                if(T_annulus>20&&T_annulus<150) //���ƹ̶���ǵ�ʱ��50---100   �ǳ���Ҫ������������������������ //if(T_annulus>20&&T_annulus<150)
                {
									BUZZ_OFF
	                     Turn_Suquence = 3;//Բ�������PID����
                            if(Direction_Annulus==1)  //��
                            {
                              // Steering_Control_Out(-290)  ;  // -250 ������  ������ ������ǿ�ƿ���ת��ģ�����������������������
                                Right_Adc=Right_Xie_Adc-1000;
														
                                Left_Adc=Left_Xie_Adc+1000;
															
                           
														}
                        else if(!Direction_Annulus) //�һ�
                            {
                              //Steering_Control_Out(160)  ;//   180  ������  ������   ������ǿ�ƿ���ת��ģ�����������������������
                                Right_Adc=Right_Xie_Adc+500;
                                Left_Adc=Left_Xie_Adc-1000;  //-1100
														}

                }
                else if(T_annulus>150)//�ս�ȥ������ѭ��
                {
                         annulus_flag = 2; //�ָ����� ��������ѭ��
                         Turn_Suquence = 3;  //Բ���ض���ת��pid
                         Right_Adc=Right_Adc;
                         Left_Adc=Left_Adc;
	                      T_annulus=0;  //ʱ�����㣬�Ա�����ʹ��
									      

                }
								

}

//---------��������-----------------------------------3-------------------//
if(annulus_flag==2)
{
  BUZZ_ON 
	T_annulus++;  //��ȥʱ�����
	Turn_Suquence = 3;//Բ����PID���� 
	if(T_annulus>100&&T_annulus<200)             //if(T_annulus>100&&T_annulus<150)
	{
		if(Direction_Annulus==1)  //��
    {
      Right_Adc=Right_Adc;
      Left_Adc=Left_Adc+200;
			//Steering_Control_Out(-290)  ;  // -250 ������  ������ ������ǿ�ƿ���ת��ģ�����������������������
    }
   else if(Direction_Annulus==0) //�һ�
    {
      Right_Adc=Right_Adc + 1000;//Right_Adc+200;
      Left_Adc=Left_Adc;
    }
	}
                else if(T_annulus>200)   //�ս�ȥ������ѭ��
                {
									 annulus_flag =3;//������־λ����
									  Round_Start =2;//�Ƴ�����
									BUZZ_OFF
									  Turn_Suquence = 3;


                }

}

/********�������̶Ա����������ǵȲ���***************************/
if(Round_Start ==1)
{
	//������Ϊû�ñ�������û�������ǣ����ﲻ���ˣ��ұ�ʾ���Ѹ㣬���������֮һ��򵥺ܶ࣡������������

/*   //��������   ���ַ���ʹ��������   ���ﲻʹ����
//corner +=abs(MpuStart.gyro.z*0.01); //���ٶȻ���
    if(corner>Corner_Limit) //����һȦ                      &&(adc_date[0]+adc_date[1]>1800)
    {
      Round_Start =2;//�Ƴ�����
     annulus_flag =3;//������־λ����
   }
*/
 /*     //��������  ���ַ���ʹ�ñ�����
    Annulus_Distance +=real_speed*0.005;    //�����  ��Ϊ����Ϊ��λ
    if(Annulus_Distance>800) //����һȦAnnulus_Distance_Limit  800
    {
       Round_Start =2;//�Ƴ�����
        annulus_flag =3;//������־λ����

   }
	*/
	
	
}


if(Round_Start ==2)
 { Out_Time++;}
//--------------Բ������---------------------------4--------------------------//
   if(Out_Time>T_Clear)    //���б�־λ����  if(Out_Time>T_Clear)
   {


        //BUZZ_ON ;
        annulus_success++;
        annulus_flag=0;
        Round_Start =0;
        T_annulus=0;
        Out_Time=0;
        Annulus_Distance = 0;
        Turn_Suquence = 3;
      	aim_speed = Set_Speed;//Բ�������ָ��ٶ�
        Direction_Annulus =0;//
   }

}
}
/*****************************����ڷ����ж�**************************
������void Sanc_Cha_Kou_Analysis()
���ܣ�����ڷ����ж�
������  ��
˵����
����ֵ����
���ڣ�
�޸����ڣ�
���ߣ�  ����
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
        Dianci.SanChaKou_Flag=1;//��־λ����
        SanChaKou_Number ++;//ʶ������ڵĴ���
       BUZZ_ON;
       SanChaKou_Once = 1;
			  aim_speed = 210;

       if( SanChaKou_Once==1 ){SanChakou_Length= T_ALready;  SanChaKou_Once = 2;}//��һ��ʶ�𵽶�ȡһ��·��
    }
		
		
//		// ����Ϊǿ�ز���
//    if(T_ALready>(SanChakou_Length+200))//�ܳ�ȥһ�ξ�������־λ׼��ʶ����һ��200  �µ�ǰ�ͼ�
//    {
//        //SanChaKou_Once = 0;
//			
//        aim_speed = 250 ;
//			
//    }
//		
//		if(T_ALready>(SanChakou_Length+80))//�ܳ�ȥһ�ξ�������־λ׼��ʶ����һ��200  �µ�ǰ�ͼ�
//    {
//        //SanChaKou_Once = 0;
//			BUZZ_ON;
//        aim_speed = Set_Speed + 80;
//			
//    }
//		
//		if(T_ALready>(SanChakou_Length+330))//�ܳ�ȥһ�ξ�������־λ׼��ʶ����һ��200
//    {
//        //SanChaKou_Once = 0;
//			BUZZ_OFF;
//        aim_speed = Set_Speed;
//			
//    }
//ǿ���������µ�
//		if(T_ALready>520)//�ܳ�ȥһ�ξ�������־λ׼��ʶ����һ��200  �µ�ǰ�ͼ�
//    {
//        //SanChaKou_Once = 0;
//			BUZZ_ON;
//        aim_speed = Set_Speed + 150;
//			
//    }
//		
//		if(T_ALready>550)//�ܳ�ȥһ�ξ�������־λ׼��ʶ����һ��200
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

 //---------������ת------
 if(SanChaKou_Number ==1)
 {
    if( Dianci.SanChaKou_Flag ==1)//10msִ��һ��
    {
        SanChaKou_T++;
         go_motor(-3000,-3000);
          Right_Adc=3600;
          Left_Adc=300;
        if(SanChaKou_T>10)//    �ٶȿ��˰Ѵ�ֵ�Ӵ�
        {
            SanChaKou_T = 0;
            Dianci.SanChaKou_Flag = 0;//��־λ����


        BUZZ_OFF;
        }  
    }
		
 }
//-------��������������ת-------------
 else if (SanChaKou_Number==2)
 {
     if( Dianci.SanChaKou_Flag ==1)//10msִ��һ��
       {
           SanChaKou_T++;
          //go_motor(-3000,-3000);
          Right_Adc=300;
          Left_Adc=3600;
           if(SanChaKou_T>30)// �ٶȿ��˰Ѵ�ֵ�Ӵ�100
           {
               SanChaKou_T = 0;
               Dianci.SanChaKou_Flag = 0;//��־λ
              BUZZ_OFF;
           }
       }
			 BUZZ_ON;
        aim_speed = Set_Speed + 160;   //140 - 150ǿ����
			if(T_ALready>570)
			{
				BUZZ_OFF;
        aim_speed = Set_Speed;
			}
 }

}

  /****************����**************
������void  Protect_Anticollision()
���ܣ�
������  ��
˵����  
����ֵ��
���ڣ�2019-5-6
���ߣ� ����  */
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
  
/****************��ŵĵ������PWM���**************
������void Electromagnetism_Control()
���ܣ�
������  ��
˵����  
����ֵ��PWM
���ڣ�2021-5-30
���ߣ� �Ǹ�����  */

void Electromagnetism_Control()
{
   AD_Date_Fitier();                  //��вɼ����� ð�����򻬶��˲�
	 Annulus_Analysis();//Բ��ʶ����
	Sanc_Cha_Kou_Analysis();//�����ʶ����
  /// normalize_date();                     //�ɼ���й�һ��  0--100 �����ã�δ�ҵ�ԭ�򣬲�Ҫ��һ����
  // Protect_Anticollision();///����
     
}
/*******************�����ʼ��***************
������void init_PWM(void)
������  ��
˵����  ��ĸ10000��ʹ�ã������޸������޸Ķ�Ӧ�궨�弴��
             	pwm_init(PWM0_P00, 100, 5000);     //��ʼ��PWM0  ʹ������P0.0  ���PWMƵ��100HZ   ռ�ձ�Ϊ�ٷ�֮ 5000/PWM_DUTY_MAX*100
//							PWM_DUTY_MAX��zf_pwm.h�ļ��� Ĭ��Ϊ10000
*
*ע�⣬�ȵ��ڶ����������ΪSD05����Ƶ��Ϊ200hz ,������ΪS3010,Ƶ����Ϊ50hz
*Ƶ��ȷ�����Ȱ�ռ�ձȷ�ĸ����PWM_DUTY_MAXȷ����һ�������޸���
*Ȼ��Ϳ�ʼ���ڶ���ˣ���ռ�ձȵķ��ӣ������õĺ���������Ǹ����������ݾ�����һ�£������1/20��ռ�ձȣ�Ȼ����������������
*���㹫ʽ����ֵռ�ձȴ����7.5% ����Ƶ�ʾ��ȶ��й�ϵ�� 20ms(1.5ms�ߵ�ƽ)
����ֵ����
���ڣ�2021-5-28 ���� 10:00
�޸�ʱ�䣺2021-5-28 ���� 10:00
���ߣ� ����    */

void init_Steer_PWM(void)
{
  	pwm_init(Steer_Pin, 50, Steer_Duty_Midle);     //��ʼ�����  ���PWMƵ��50HZ����������ֵ
}
/****************���ת��������**************
������void Steering_Control_Out(int16 duty)
���ܣ�
������  ��
˵����   ���ת�����    ע����ö����ֵ�����Ҽ���Ҳ��������Ҫ�޸�����ĺ궨��
����ֵ��PWM
���ڣ�2021-5-28 ���� 10:00
�޸�ʱ�䣺2021-5-28 ���� 10:00
���ߣ� ����    */
void Steering_Control_Out(int16 duty)
{
     duty = Steer_Duty_Midle + duty ;//�ڶ����ֵ�Ļ����Ͻ���ƫ��
   if (duty >= Steer_Duty_Max) duty = Steer_Duty_Max;
   else if(duty <= Steer_Duty_Min) duty = Steer_Duty_Min;
     pwm_duty(Steer_Pin, duty);
}


/*################����ר��#############################################
                                                      	DisableGlobalIRQ();		//�ر����ж�
                                                             while(1)
                                                             {
                                                                 go_motor(0,0);
                                                                 BUZZ_OFF;
                                                             }
    //################����ר��#############################################*/
