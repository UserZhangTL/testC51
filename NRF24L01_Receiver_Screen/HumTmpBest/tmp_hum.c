#include "AllFunc.h"

//#include <reg52.h>
//#include <intrins.h>
//#include "delay.h"
//#include "uart.h"


///*��ʪ����ʹ�õ��Ĵ洢ȫ�ֵı���*/
////hum_val = (hum_tmp[0] / 10)*10 + (hum_tmp[0] % 10);
////tmp_val = (hum_tmp[2] / 10)*10 + (hum_tmp[2] % 10);
//int hum_val = 0;
//int tmp_val = 0;
///*
//hum_tmp[0]��װ����ʪ��
//hum_tmp[2]��װ�����¶�
//����ʮ������
//*/
//unsigned char hum_tmp[5] = {0};
//unsigned char hum_tmp_buf[20] = {0};//��װ��ʪ�ȳ�һ���ַ���

//#define OK 1
//#define ERROR 0
////������Լ���dht11 �ӵ�IO �����Ķ�λ����
//sbit dht11 = P2^0;
////��ֹ����Ӳ��ͨ��ʱ������ѭ���ļ�����Χ
//#define NUMBER 20
//#define SIZE 5
//static unsigned char status;

////������ֽ����ݵ�����
//unsigned char value_array[SIZE];

///*�����������ļ�������ʪ��ֵ,ʵ�����¶ȵ�������10 ��
//��dht11 ���ص��¶���26,��temp_value = 260, ʪ��ͬ��*/
//int temp_value, humi_value;
//unsigned char ReadValue(void);
//void Delay_1ms(unsigned int ms)
//{
//		unsigned int x, y;
//		for(x = ms; x > 0; x--)
//		{
//				for(y = 124; y > 0; y--);
//		}
//}




///*����ʪ�ȶ�ȡ����������ƴװ��һ���ַ���*/
//void getTmpHum_and_sprintf()
//{
//		/*��ʪ��ģ��*/
//    if(ReadTempAndHumi() == OK)
//	  {
//        //����ɹ�����ת����ʮ����
//			  hum_val = (value_array[0] / 10)*10 + (value_array[0] % 10);
//        tmp_val = (value_array[2] / 10)*10 + (value_array[2] % 10);  
//			  sprintf(hum_tmp_buf,"Tmp:%02d Hum:%02d",tmp_val,tmp_val);
// 		}
//		else
//		{
//			  sprintf(hum_tmp_buf,"Tmp:FF Hum:FF");
//		}
//}






//void Delay_10us(void)
//{
//		unsigned char i;
//		i--;
//		i--;
//		i--;
//		i--;
//		i--;
//		i--;
//}

///*��һ���ֽڵ�����*/
//unsigned char ReadValue(void)
//{
//		unsigned char count, value = 0, i;
//		status = OK; //�趨��־Ϊ����״̬
//		for(i = 8; i > 0; i--)
//		{
//				//��λ����
//				value <<= 1;
//				count = 0;
//				//ÿһλ����ǰ����һ��50us �ĵ͵�ƽʱ��.�ȴ�50us �͵�ƽ����
//				while(dht11 == 0 && count++ < NUMBER);
//				if(count >= NUMBER)
//				{
//						status = ERROR; //�趨�����־
//						return 0; //����ִ�й��̷���������˳�����
//				}
//				//26-28us �ĸߵ�ƽ��ʾ��λ��0,Ϊ70us �ߵ�ƽ���λ1
//				Delay_10us();
//				Delay_10us();
//				Delay_10us();
//				//��ʱ30us �����������Ƿ��Ǹߵ�ƽ
//				if(dht11 != 0)
//				{
//						//���������ʾ��λ��1
//						value++;
//						//�ȴ�ʣ��(Լ40us)�ĸߵ�ƽ����
//						while(dht11 != 0 && count++ < NUMBER)
//						{
//								dht11 = 1;
//						}
//						if(count >= NUMBER)
//						{
//								status = ERROR; //�趨�����־
//								return 0;
//						}
//				}
//		}
//		return (value);
//}
////��һ�ε�����,�����ֽ�
//unsigned char ReadTempAndHumi(void)
//{
//		unsigned char i = 0, check_value = 0,count = 0;
//		EA = 0;
//		dht11 = 0; //���������ߴ���18ms ���Ϳ�ʼ�ź�
//		Delay_1ms(20); //�����18 ����
//		dht11 = 1; //�ͷ�������,���ڼ��͵�ƽ��Ӧ���ź�
//		//��ʱ20-40us,�ȴ�һ��ʱ�����Ӧ���ź�,Ӧ���ź��Ǵӻ�����������80us
//		Delay_10us();
//		Delay_10us();
//		Delay_10us();
//		Delay_10us();
//		if(dht11 != 0) //���Ӧ���ź�,Ӧ���ź��ǵ͵�ƽ
//		{
//				//ûӦ���ź�
//				EA = 1;
//				return ERROR;
//		}
//		else
//		{
//				//��Ӧ���ź�
//				while(dht11 == 0 && count++ < NUMBER); //�ȴ�Ӧ���źŽ���
//				if(count >= NUMBER) //���������Ƿ񳬹����趨�ķ�Χ
//				{
//						dht11 = 1;
//						EA = 1;
//						return ERROR; //�����ݳ���,�˳�����
//				}
//				count = 0;
//				dht11 = 1;//�ͷ�������
//				//Ӧ���źź����һ��80us �ĸߵ�ƽ���ȴ��ߵ�ƽ����
//				while(dht11 != 0 && count++ < NUMBER);
//				if(count >= NUMBER)
//				{
//						dht11 = 1;
//						EA = 1;
//						return ERROR; //�˳�����
//				}
//				//����ʪ.�¶�ֵ
//				for(i = 0; i < SIZE; i++)
//				{
//						value_array[i] = ReadValue();
//						if(status == ERROR)//����ReadValue()�����ݳ�����趨status ΪERROR
//						{
//								dht11 = 1;
//								EA = 1;
//								return ERROR;
//						}
//						//���������һ��ֵ��У��ֵ�������ȥ
//						if(i != SIZE - 1)
//						{
//								//���������ֽ������е�ǰ���ֽ����ݺ͵��ڵ����ֽ����ݱ�ʾ�ɹ�
//								check_value += value_array[i];
//						}
//				}	//end for
//				//��û�÷�����������ʧ��ʱ����У��
//				if(check_value == value_array[SIZE - 1])
//				{
//						//����ʪ������10 ����������ÿһλ
//						humi_value = value_array[0] * 10;
//						temp_value = value_array[2] * 10;
//						dht11 = 1;
//						EA = 1;
//						return OK; //��ȷ�Ķ���dht11 ���������
//				}
//				else
//				{
//						//У�����ݳ���
//						EA = 1;
//						return ERROR;
//				}
//		}
//}


//void main()
//{
//	  unsigned char i;
//	  uart_init();
//	  delay_s(1);//�ȴ�1�룬����ģ���ʼ�Ĳ��ȶ�
//	  while(1)
//		{
//			  delay_s(3);
//			
//	      if(ReadTempAndHumi() == OK)
//				{
//					  for(i = 0;i < 5;i ++)
//						{
//							  uart_sendbyte(value_array[i]);//��ӡ����������ʾ
//						}
//				}
//				 	
//				for(i = 0;i < 5;i ++)
//				{
//				   value_array[i] = 0x00;
//				}  
//		}  
//}