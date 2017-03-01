#include <reg52.h>
#include "delay.h"
#include "timer.h"

// //直流电机位，J45上的DC2
// sbit DC2 = P1^0;
// bit startDC2_Flag = 0;
// sbit Key1 = P2^2;
// sbit Key2 = P2^1;

// //步进电机位，J45上的abcd
// sbit a = P2^7;
// sbit b = P2^6;
// sbit c = P2^5;
// sbit d = P2^4;
// //                          0001,0011,0010,0110,0100,1100,1000,1001
// //                          1001,1000,1100,0100,0110,0010,0011,0001
// unsigned char code Back[8]={0xf1,0xf3,0xf2,0xf6,0xf4,0xfc,0xf8,0xf9}; //反转
// unsigned char code Forw[8]={0xf9,0xf8,0xfc,0xf4,0xf6,0xf2,0xf3,0xf1}; //正转

// //控制直流电机，命令发来为1，开始转动，为0，停止转动
// void control_DC2(unsigned char comd)
// {
// 	  if(comd == 1)
// 		{
// 			  startDC2_Flag = 1;
// 		}
// 		else
// 		{
// 			  startDC2_Flag = 0;
// 		}
// }

// //控制步进电机，0：正转，1：反转   需要一直摁着才能一直转动
// void control_StepMotor(unsigned char comd)
// {
// 	  unsigned char i = 0;
// 	  if(comd == 0)
// 		{
// 			  for(i = 0;i < 8;i ++)
// 			  {
// 			      a = Back[i] & 0x01;
// 					  b = Back[i] & 0x02;
// 					  c = Back[i] & 0x04;
// 					  d = Back[i] & 0x08;
// 					  delay_ms(1);
// 				}
// 		}
// 		if(comd == 1)
// 		{
// 			  for(i = 0;i < 8;i ++)
// 			  {
// 			      a = Forw[i] & 0x01;
// 					  b = Forw[i] & 0x02;
// 					  c = Forw[i] & 0x04;
// 					  d = Forw[i] & 0x08;
// 					  delay_ms(1);
// 				}
// 		}
// }

//使用参考

// void main()
// {
// 	  Timer1_Init();
// 	  while(1)
// 		{
// 			  if(Key1 == 0)
// 				{
// 					  control_StepMotor(0);
// 				}
// 				if(Key2 == 0)
// 				{
// 					  control_StepMotor(1);
// 				}
// 		}
// }



// void TImer1_Isr() interrupt 3
// {
// 	  static unsigned char timer1 = 0;
// 	  TH1 = (65536 - 50) / 256;  //50us
// 	  TL1 = (65536 - 50) % 256; 
// 	  timer1 ++;
// 	  if(timer1 == 100)
// 		{
// 			  timer1 = 0;
// 		}
// 		if(startDC2_Flag == 1)
// 		{
// 				if(timer1 < 90)
// 				{
// 						DC2 = 1;
// 				}
// 				else
// 				{
// 						DC2 = 0;
// 				}
// 		}
// }