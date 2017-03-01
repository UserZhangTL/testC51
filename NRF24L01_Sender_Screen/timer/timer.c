#include <reg52.h>
#include "timer.h"

//定时器0初始化
void Timer0_Init()
{
		EA = 1;   
	  TMOD |= 0x01; 
	  TH0 = (65536 - 2000) / 256;
	  TL0 = (65536 - 2000) % 256;  
	  ET0 = 1;
	  TR0 = 1; 
}


//定时器1初始化
void Timer1_Init()
{
		EA = 1;
	  TMOD |= 0x10;
	  TH1 = (65536 - 2000) / 256;
	  TL1 = (65536 - 2000) % 256; 
	  ET1 = 1; 
	  TR1 = 1; 
}

