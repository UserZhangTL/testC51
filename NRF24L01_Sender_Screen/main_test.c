/*
名称：无线广告屏发送端
作者：张天乐
更新时间：2016/5/21
更新内容：删除无用模块内容，留下有用的NRF发送部分


*/


#include "AllFunc.h"

extern unsigned char Direction_flag;
extern bit One_s_Come;


unsigned char Hand_Switch = NO;//打开手势标志位，默认是关闭的

void Iner0_Init()//按键11，控制手势的关
{
	  EA = 1; //总中断
	  EX0 = 1;//允许外部中断0
	  IT0 = 0;//设置为低电平触发
}

void Iner1_Init()//按键12，控制手势的开
{
	  EA = 1;
	  EX1 = 1;
	  IT1 = 0;
}


//定时器0初始化
void Timer0_Init()
{
		EA = 1;   
	  TMOD |= 0x01; 
	  TH0 = (65536 - 1000) / 256;
	  TL0 = (65536 - 1000) % 256;  
	  ET0 = 1;
	  TR0 = 1; 
}

void main()
{	
	  unsigned char Tx[4] = {0xaa,0x55,0x04,0xff};//在发送端，发送的字节长度为4
    
    unsigned char keynum = 0xff;

	  NRF24L01_Init();
    uart_init();
		Iner0_Init();
		Iner1_Init();


		//如果按下按键，则说明要设置为发送模式，发送数据
	  //想要测试到STATUS = 0x2e，需要下载一遍，然后立马开串口，会出现2E,说明发送成功
    //按键的过程中，不需要后沿消抖，保证一定时间的正确值
    
		//一开始就是接收模式
    SetNRF_ReceiveDataMode();
    while(1)
	  {
			  /*注意！只有第二三四个字节的数据是可以用的！！！
			  所以只索取后三个字节的数据，作为有效数据
			  注意！因为还是没有解决重复接收最后一个数据的问题，
			  所以，当后面截获到的数据都是第一个==第二个==第三个==第四个的，都是无效数据
			  不做接收(近期重新测试，发现不再出现重复出现最后一个字节的问题，只是会多一个无效的字节，把四个字节的有效数据变成了五个字节)
			  */
			 
				//扫描键盘
			  keynum = Key_Scan();
	      if(keynum == 1)
	      {
					  Tx[2] = 0x01;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//要是发送成功
		        {
							  //发送成功，那么就是开了
			      }
	      }
				else if(keynum == 2)
	      {
					  Tx[2] = 0x02;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//要是发送成功
		        {
				         //发送成功，那么就是关了
							  
			      }
	      }
				else if(keynum == 3)
	      {
					  Tx[2] = 0x03;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//要是发送成功
		        {
			      }
	      }
				else if(keynum == 4)
	      {
					  Tx[2] = 0x04;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//要是发送成功
		        {
			      }
	      }
				else if(keynum == 5)
	      {
					  Tx[2] = 0x05;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//要是发送成功
		        {
				        
			      }
	      }
				else if(keynum == 6)
	      {
					  Tx[2] = 0x06;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//要是发送成功
		        {
				        
			      }
	      }
				else if(keynum == 7)
	      {
					  Tx[2] = 0x07;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//要是发送成功
		        {
				        
			      }
	      }
				else if(keynum == 8)
	      {
					  Tx[2] = 0x08;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//要是发送成功
		        {
				        
			      }
	      }
				else if(keynum == 9)
	      {
					  Tx[2] = 0x09;
			      SetNRF_SendDataMode(Tx);
	          if(!CheckAck())//要是发送成功
		        {
				        
			      }
						
//						//恢复到接收模式，
//						SetNRF_ReceiveDataMode();
	      }
				else 
				{}		
					
				//默认手势标志位为关闭的，要是开启了才有此功能
        if(Hand_Switch == YES)
				{					
						getHandSignal();
					
					  //左移表示上一个		
						if(Direction_flag == RIGHT)//
						{
								Direction_flag = NO;
								Tx[2] = 0x0a;
								SetNRF_SendDataMode(Tx);
								if(!CheckAck())//要是发送成功
								{
										
								} 
						}
			  }
			  
    }
}


void Timer0_Isr() interrupt 1
{
	  static unsigned int i = 0;
	
	  TH0 = (65536 - 1000) / 256;
	  TL0 = (65536 - 1000) % 256;
	
	  i ++;
	  if(i == 500)
		{
			  i = 0;
			  One_s_Come = YES;
		}
}

void Int0_Isr() interrupt 0
{
	  EX0 = 0;
	  Hand_Switch = NO;
	  EX0 = 1;
}

void Int1_Isr() interrupt 2
{
	  EX1 = 0;
	  Hand_Switch = YES;
	  EX1 = 1;
}
