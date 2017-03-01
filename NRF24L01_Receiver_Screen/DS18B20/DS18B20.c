#include "AllFunc.h"


//一位的数据位
sbit ds = P2^0;
bit ack;
int real_tmp;
unsigned char tmpInfo_buf[8] = {0};

//复位初始化
void ds_reset()
{
	  //根据手册
	  ds = 1;
	  ds = 0;  
	  delay_us(200);
	  delay_us(200);
	  ds = 1;  //置一
	  delay_us(30);
	  if(ds == 0)  //如果有相应，ack置一
		{
			  ack = 1;
		}
		else
		{
			  ack = 0;
		}
		delay_us(200);
		delay_us(200);//延时一下，以防后面数据操作在应答状态下作操作，出现错误
}


//写入一个字节数据
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
		//最后需要拉高
		ds = 1;
}

bit ds_readbit()
{
	  bit temp;
	  ds = 1;
	  ds = 0;
	  _nop_();
	  _nop_();
	  ds = 1;//先把ds拉高，要是为低电平，会被拉低
	  temp = ds;//赋值返回
	  delay_us(40);
	
	  return temp;
}


//读出
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
/*采集温湿度，然后sprintf拼装完成*/
void getTmp_and_Sprintf()
{
	  unsigned char a;
	  unsigned int tmp,b;
	  int temp_tmp = 0;//临时存储的温度
	  
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
		ModifyTmp();//调整一次
		/*正负数都可以拼接，多留出一位用来放零下温度*/
	  sprintf(tmpInfo_buf,"%03d",tempStorageTmp);
		
}

/*用于避免温度的显示错误*/
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