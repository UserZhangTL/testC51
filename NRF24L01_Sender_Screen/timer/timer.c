#include <reg52.h>
#include "timer.h"

//��ʱ��0��ʼ��
void Timer0_Init()
{
		EA = 1;   
	  TMOD |= 0x01; 
	  TH0 = (65536 - 2000) / 256;
	  TL0 = (65536 - 2000) % 256;  
	  ET0 = 1;
	  TR0 = 1; 
}


//��ʱ��1��ʼ��
void Timer1_Init()
{
		EA = 1;
	  TMOD |= 0x10;
	  TH1 = (65536 - 2000) / 256;
	  TL1 = (65536 - 2000) % 256; 
	  ET1 = 1; 
	  TR1 = 1; 
}

