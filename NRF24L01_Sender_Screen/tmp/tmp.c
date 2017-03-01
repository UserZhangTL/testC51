#include <reg52.h>
#include <intrins.h>
#include "lcd.h"
#include <stdio.h>
#include "delay.h"
#include "timer.h"

//一位的数据位
sbit ds = P1^0;
bit ack;
sbit BEEP = P3^7;


unsigned char tmp_flag = 0;


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

void show_tmp()
{
	  unsigned char a;
	  unsigned int tmp,b;
	  float c;
	  unsigned char disbuf[13] = {0};
			
	  BEEP = 0;
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
		c = (float)tmp * 0.0625;
	  sprintf(disbuf,"tmp is %5.3f",c);
	  delay_ms(50);
	  lcd_write_str(1,0,disbuf);
}