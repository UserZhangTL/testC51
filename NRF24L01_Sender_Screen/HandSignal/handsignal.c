#include "AllFunc.h"


sbit Left_Avoid = P3^1;//��ߵı���ģ��
sbit Right_Avoid = P1^6;//�ұߵı���ģ��

unsigned char Direction_flag = NO;//
bit One_s_Come = NO;//10ms����״̬λ


void getHandSignal()//ʹ��ȫ�ֱ�����־λDirection_flag��ʾ
{
	  if((Left_Avoid == 0) || (Right_Avoid == 0))
		{
				if((Left_Avoid == 0) && (Right_Avoid == 1))//�Ұ�
				{
						One_s_Come = NO;//��ʼ�����ˣ������һ�£���֤���������Ч
						while((One_s_Come == NO) && (Right_Avoid == 1));//����֮һ����
						if(One_s_Come == YES)//�޸ĵ�0�ˣ�˵��û����ȷ�������˳�
						{
								Direction_flag = NO;
						}
						else if(Right_Avoid == 0)
						{ 
							  delay_ms(5);
                if((Right_Avoid == 0) && (Left_Avoid == 1))	
                {									
								    Direction_flag = RIGHT;
								}
						}
				}
				
		}
	
	
	

}


