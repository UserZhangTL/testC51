#include "AllFunc.h"


//uart初始化函数
void uart_init()
{
	  SCON = 0x50;   //设置串口工作方式
	  TMOD |= 0x20;  //设置定时器一工作在八位自动重装方式
	  TH1 = 0xfa;    //比特率为4800bit/s时的TH1中放置的初始值，开始时，自动赋给TL1
	  TR1 = 1;       //允许中断
	
	  EA = 1;         //总中断
	  ES = 1;         //串口中断
}

//发送单个字节给PC
void uart_sendbyte(unsigned char byte)
{
	  /*TI为发送中断，如果发送一个数据，则被置一，被置一后需要手动复位*/
	  SBUF = byte;//将内容放到SBUF中，PC会接收
	  while(!TI); //当TI = 0时，等待发送完成
	  TI = 0;     //TI需要手动置零
}

//发送字符串到PC
void uart_sendstr(unsigned char *s)
{
	  while(*s)
		{
			  uart_sendbyte(*s);
			  s ++;
		}
}

//中断服务函数，用于软复位
void uart_isr() interrupt 4
{
	  static unsigned char i = 0;
	  unsigned char temp;
	  ES = 0;
	  temp = SBUF;
	  if(0x7f == temp)//专门用于德飞莱开发板的0x7f
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