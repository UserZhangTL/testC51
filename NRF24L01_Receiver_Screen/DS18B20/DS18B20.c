#include "AllFunc.h"


//һλ������λ
sbit ds = P2^0;
bit ack;
int real_tmp;
unsigned char tmpInfo_buf[8] = {0};

//��λ��ʼ��
void ds_reset()
{
	  //�����ֲ�
	  ds = 1;
	  ds = 0;  
	  delay_us(200);
	  delay_us(200);
	  ds = 1;  //��һ
	  delay_us(30);
	  if(ds == 0)  //�������Ӧ��ack��һ
		{
			  ack = 1;
		}
		else
		{
			  ack = 0;
		}
		delay_us(200);
		delay_us(200);//��ʱһ�£��Է��������ݲ�����Ӧ��״̬�������������ִ���
}


//д��һ���ֽ�����
void ds_writebyte(unsigned char byte)
{
	  unsigned char i = 0;
	  for(i = 0;i < 8;i ++)
		{
			  ds = 1;
			  ds = 0;
			  _nop_();
			  _nop_();
			  ds = byte & 0x01;
			  delay_us(40);
			  byte >>= 1;
		}
		//�����Ҫ����
		ds = 1;
}

bit ds_readbit()
{
	  bit temp;
	  ds = 1;
	  ds = 0;
	  _nop_();
	  _nop_();
	  ds = 1;//�Ȱ�ds���ߣ�Ҫ��Ϊ�͵�ƽ���ᱻ����
	  temp = ds;//��ֵ����
	  delay_us(40);
	
	  return temp;
}


//����
unsigned char ds_readbyte()
{
	 
	  unsigned char j,k;
	  unsigned char i = 0;
	  
	  for(i = 0;i < 8;i ++)
		{
			  j = ds_readbit();
			  k = (j << 7) | (k >> 1);
    }
		
		
		return k;
}

extern int tempStorageTmp;
/*�ɼ���ʪ�ȣ�Ȼ��sprintfƴװ���*/
void getTmp_and_Sprintf()
{
	  unsigned char a;
	  unsigned int tmp,b;
	  int temp_tmp = 0;//��ʱ�洢���¶�
	  
	  ds_reset();
		ds_writebyte(0xcc);
		ds_writebyte(0x44);
		delay_ms(50);
		ds_reset();
		ds_writebyte(0xcc);
		ds_writebyte(0xbe);
			  
		a = ds_readbyte();
		b = ds_readbyte();
	  tmp = (b << 8) + a;
	

	  real_tmp = (int)tmp * 0.0625;
		ModifyTmp();//����һ��
		/*������������ƴ�ӣ�������һλ�����������¶�*/
	  sprintf(tmpInfo_buf,"%03d",tempStorageTmp);
		
}

/*���ڱ����¶ȵ���ʾ����*/
void ModifyTmp()
{
	  if(real_tmp != 0)
		{
			 tempStorageTmp = real_tmp; 
		}
}

//void main()
//{
//	  while(1)
//		{
//			show_tmp();
//		}
//}