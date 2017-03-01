#include "keyboard_v.h"
#include "delay.h"
#include <reg52.h>

#define KEYPORT P0


unsigned char keyvalue()
{
	 unsigned char keyvalue;
	 KEYPORT = 0xf0;
	 if(0xf0 != KEYPORT  ) //说明有按键按下了
	 {
		   delay_ms(10);
		   if(0xf0 != KEYPORT) //确实有按键按下了
			 {
           KEYPORT = 0xfe;
           if(0xfe != KEYPORT)				 
					 {
						     keyvalue = KEYPORT & 0xf0;
                 keyvalue += 0x0e;    //去除干扰
// 						  /*电子琴不需要后沿消抖*/
// 								 while(0xfe != KEYPORT);
// 								 delay_ms(10);
// 								 while(0xfe != KEYPORT); //后沿去抖动
                 return keyvalue;						 
						}
						
					 KEYPORT = 0xfd;
           if(0xfd != KEYPORT)				 
					 {
						     keyvalue = KEYPORT & 0xf0;
                 keyvalue += 0x0d;    //去除干扰
// 						 /*电子琴不需要后沿消抖*/
// 								 while(0xfd != KEYPORT);
// 								 delay_ms(10);
// 								 while(0xfd != KEYPORT); //后沿去抖动
                 return keyvalue;						 
						}	

					 KEYPORT = 0xfb;
           if(0xfb != KEYPORT)				 
					 {
						     keyvalue = KEYPORT & 0xf0;
                 keyvalue += 0x0b;    //去除干扰
// 						 /*电子琴不需要后沿消抖*/
// 								 while(0xfb != KEYPORT);
// 								 delay_ms(10);
// 								 while(0xfb != KEYPORT); //后沿去抖动
                 return keyvalue;						 
						}		

					 KEYPORT = 0xf7;
           if(0xf7 != KEYPORT)				 
					 {
						     keyvalue = KEYPORT & 0xf0;
                 keyvalue += 0x07;    //去除干扰
// 						 /*电子琴不需要后沿消抖*/
// 								 while(0xf7 != KEYPORT);
// 								 delay_ms(10);
// 								 while(0xf7 != KEYPORT); //后沿去抖动
                 return keyvalue;						 
						}								
						
			 }
	 }
	 
	 return 0xff;
}

/*！！！！！普中开发板专用键盘扫描键值返回函数！！！！！！！*/
unsigned char key_por()
{
	  unsigned char num;
	  num = keyvalue();
	  switch(num)
	  {
			  case 0x7e:return 4;break;
			  case 0xbe:return 8;break;
			  case 0xde:return 12;break;
			  case 0xee:return 16;break;
			
			  case 0x7d:return 3;break;
			  case 0xbd:return 7;break;
			  case 0xdd:return 11;break;
			  case 0xed:return 15;break;
			
			  case 0x7b:return 2;break;
			  case 0xbb:return 6;break;
			  case 0xdb:return 10;break;
			  case 0xeb:return 14;break;
			
			  case 0x77:return 1;break;
			  case 0xb7:return 5;break;
			  case 0xd7:return 9;break;
			  case 0xe7:return 13;break;
			
			  default:return 0xff;break;
    }
    return 0xff;
}