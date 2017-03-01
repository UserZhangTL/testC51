/*
���ƣ����߹�������Ͷ�
���ߣ�������
����ʱ�䣺2016/5/21
�������ݣ�ɾ������ģ�����ݣ��������õ�NRF���Ͳ���


*/


#include "AllFunc.h"

extern unsigned char Direction_flag;
extern bit One_s_Come;


unsigned char Hand_Switch = NO;//�����Ʊ�־λ��Ĭ���ǹرյ�

void Iner0_Init()//����11���������ƵĹ�
{
	  EA = 1; //���ж�
	  EX0 = 1;//�����ⲿ�ж�0
	  IT0 = 0;//����Ϊ�͵�ƽ����
}

void Iner1_Init()//����12���������ƵĿ�
{
	  EA = 1;
	  EX1 = 1;
	  IT1 = 0;
}


//��ʱ��0��ʼ��
void Timer0_Init()
{
		EA = 1;   
	  TMOD |= 0x01; 
	  TH0 = (65536 - 1000) / 256;
	  TL0 = (65536 - 1000) % 256;  
	  ET0 = 1;
	  TR0 = 1; 
}

void main()
{	
	  unsigned char Tx[4] = {0xaa,0x55,0x04,0xff};//�ڷ��Ͷˣ����͵��ֽڳ���Ϊ4
    
    unsigned char keynum = 0xff;

	  NRF24L01_Init();
    uart_init();
		Iner0_Init();
		Iner1_Init();


		//������°�������˵��Ҫ����Ϊ����ģʽ����������
	  //��Ҫ���Ե�STATUS = 0x2e����Ҫ����һ�飬Ȼ���������ڣ������2E,˵�����ͳɹ�
    //�����Ĺ����У�����Ҫ������������֤һ��ʱ�����ȷֵ
    
		//һ��ʼ���ǽ���ģʽ
    SetNRF_ReceiveDataMode();
    while(1)
	  {
			  /*ע�⣡ֻ�еڶ����ĸ��ֽڵ������ǿ����õģ�����
			  ����ֻ��ȡ�������ֽڵ����ݣ���Ϊ��Ч����
			  ע�⣡��Ϊ����û�н���ظ��������һ�����ݵ����⣬
			  ���ԣ�������ػ񵽵����ݶ��ǵ�һ��==�ڶ���==������==���ĸ��ģ�������Ч����
			  ��������(�������²��ԣ����ֲ��ٳ����ظ��������һ���ֽڵ����⣬ֻ�ǻ��һ����Ч���ֽڣ����ĸ��ֽڵ���Ч���ݱ��������ֽ�)
			  */
			 
				//ɨ�����
			  keynum = Key_Scan();
	      if(keynum == 1)
	      {
					  Tx[2] = 0x01;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//Ҫ�Ƿ��ͳɹ�
		        {
							  //���ͳɹ�����ô���ǿ���
			      }
	      }
				else if(keynum == 2)
	      {
					  Tx[2] = 0x02;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//Ҫ�Ƿ��ͳɹ�
		        {
				         //���ͳɹ�����ô���ǹ���
							  
			      }
	      }
				else if(keynum == 3)
	      {
					  Tx[2] = 0x03;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//Ҫ�Ƿ��ͳɹ�
		        {
			      }
	      }
				else if(keynum == 4)
	      {
					  Tx[2] = 0x04;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//Ҫ�Ƿ��ͳɹ�
		        {
			      }
	      }
				else if(keynum == 5)
	      {
					  Tx[2] = 0x05;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//Ҫ�Ƿ��ͳɹ�
		        {
				        
			      }
	      }
				else if(keynum == 6)
	      {
					  Tx[2] = 0x06;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//Ҫ�Ƿ��ͳɹ�
		        {
				        
			      }
	      }
				else if(keynum == 7)
	      {
					  Tx[2] = 0x07;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//Ҫ�Ƿ��ͳɹ�
		        {
				        
			      }
	      }
				else if(keynum == 8)
	      {
					  Tx[2] = 0x08;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//Ҫ�Ƿ��ͳɹ�
		        {
				        
			      }
	      }
				else if(keynum == 9)
	      {
					  Tx[2] = 0x09;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//Ҫ�Ƿ��ͳɹ�
		        {
				        
			      }
						
//						//�ָ�������ģʽ��
//						SetNRF_ReceiveDataMode();
	      }
				else 
				{}		
					
				//Ĭ�����Ʊ�־λΪ�رյģ�Ҫ�ǿ����˲��д˹���
        if(Hand_Switch == YES)
				{					
						getHandSignal();
					
					  //���Ʊ�ʾ��һ��		
						if(Direction_flag == RIGHT)//
						{
								Direction_flag = NO;
								Tx[2] = 0x0a;
								SetNRF_SendDataMode(Tx);
								if(!CheckAck())//Ҫ�Ƿ��ͳɹ�
								{
										
								} 
						}
			  }
			  
    }
}


void Timer0_Isr() interrupt 1
{
	  static unsigned int i = 0;
	
	  TH0 = (65536 - 1000) / 256;
	  TL0 = (65536 - 1000) % 256;
	
	  i ++;
	  if(i == 500)
		{
			  i = 0;
			  One_s_Come = YES;
		}
}

void Int0_Isr() interrupt 0
{
	  EX0 = 0;
	  Hand_Switch = NO;
	  EX0 = 1;
}

void Int1_Isr() interrupt 2
{
	  EX1 = 0;
	  Hand_Switch = YES;
	  EX1 = 1;
}
