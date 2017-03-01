#include "AllFunc.h"


//uart��ʼ������
void uart_init()
{
	  SCON = 0x50;   //���ô��ڹ�����ʽ
	  TMOD |= 0x20;  //���ö�ʱ��һ�����ڰ�λ�Զ���װ��ʽ
	  TH1 = 0xfa;    //������Ϊ4800bit/sʱ��TH1�з��õĳ�ʼֵ����ʼʱ���Զ�����TL1
	  TR1 = 1;       //�����ж�
	
	  EA = 1;         //���ж�
	  ES = 1;         //�����ж�
}

//���͵����ֽڸ�PC
void uart_sendbyte(unsigned char byte)
{
	  /*TIΪ�����жϣ��������һ�����ݣ�����һ������һ����Ҫ�ֶ���λ*/
	  SBUF = byte;//�����ݷŵ�SBUF�У�PC�����
	  while(!TI); //��TI = 0ʱ���ȴ��������
	  TI = 0;     //TI��Ҫ�ֶ�����
}

//�����ַ�����PC
void uart_sendstr(unsigned char *s)
{
	  while(*s)
		{
			  uart_sendbyte(*s);
			  s ++;
		}
}

//�жϷ�������������λ
void uart_isr() interrupt 4
{
	  static unsigned char i = 0;
	  unsigned char temp;
	  ES = 0;
	  temp = SBUF;
	  if(0x7f == temp)//ר�����ڵ·����������0x7f
		{
			  i ++;
			  if(10 == i)
				{
					  i = 0;
					  ISP_CONTR = 0xe0;
        }
		}
		else
		{
			  i = 0;
		}
		RI = 0;
		ES = 1;
}