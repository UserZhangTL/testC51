#include <reg52.h>
#include <intrins.h>
#include "delay.h"
#include "uart.h"


unsigned char segout[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char code tab[] = (0x00,0x6c,0x92,0x82,0x44,0x28,0x10,0x00);


sbit LATCH = P1^0;
sbit SRCLK = P1^1;
sbit SER = P1^2;

sbit LATCH_B = P2^0;
sbit SRCLK_B = P2^1;
sbit SER_B = P2^2;

/*
add a new function
*/
void func()
{
	  
}


void SendByte(unsigned char dat)
{
	  unsigned char i;
	
	  for(i = 0;i < 8;i ++)
	  {
			  SRCLK = 0;
			  SER = dat&0x80;
			  dat <<= 1;
			  SRCLK = 1;
		}
}


void Send2Byte(unsigned char dat1,unsigned char dat2)
{
	  SendByte(dat1);
	  SendByte(dat2);
}

void Out595(void)
{
	  _nop_();
	  LATCH = 1;
	  LATCH = 0;
}

void SendSeg(unsigned char dat)
{
		unsigned char i;
	  for(i = 0;i < 8;i ++)
	  {
			  SRCLK_B = 0;
			  SER_B = dat & 0x80;
			  dat <<= 1;
			  SRCLK_B = 1;
		}
		LATCH_B = 1;
		_nop_();
		LATCH_B = 0;
}


void main()
{
	  unsigned char i,j;
	  uart_init();
	  while(1)
		{
				SendSeg(0xaa);
			  Send2Byte(0x00,0xff);//第一个参数是行，第二个参数是列
			  Out595();
		}
}