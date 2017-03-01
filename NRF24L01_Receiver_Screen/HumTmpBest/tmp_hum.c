#include "AllFunc.h"

//#include <reg52.h>
//#include <intrins.h>
//#include "delay.h"
//#include "uart.h"


///*温湿度中使用到的存储全局的变量*/
////hum_val = (hum_tmp[0] / 10)*10 + (hum_tmp[0] % 10);
////tmp_val = (hum_tmp[2] / 10)*10 + (hum_tmp[2] % 10);
//int hum_val = 0;
//int tmp_val = 0;
///*
//hum_tmp[0]中装的是湿度
//hum_tmp[2]中装的是温度
//都是十六进制
//*/
//unsigned char hum_tmp[5] = {0};
//unsigned char hum_tmp_buf[20] = {0};//组装温湿度成一个字符串

//#define OK 1
//#define ERROR 0
////请根据自己的dht11 接的IO 口来改动位定义
//sbit dht11 = P2^0;
////防止在与硬件通信时发生死循环的计数范围
//#define NUMBER 20
//#define SIZE 5
//static unsigned char status;

////存放五字节数据的数组
//unsigned char value_array[SIZE];

///*可在其他的文件引用温湿度值,实际是温度的整数的10 倍
//如dht11 读回的温度是26,则temp_value = 260, 湿度同理*/
//int temp_value, humi_value;
//unsigned char ReadValue(void);
//void Delay_1ms(unsigned int ms)
//{
//		unsigned int x, y;
//		for(x = ms; x > 0; x--)
//		{
//				for(y = 124; y > 0; y--);
//		}
//}




///*将温湿度读取出来，并且拼装成一个字符串*/
//void getTmpHum_and_sprintf()
//{
//		/*温湿度模块*/
//    if(ReadTempAndHumi() == OK)
//	  {
//        //如果成功，就转化成十进制
//			  hum_val = (value_array[0] / 10)*10 + (value_array[0] % 10);
//        tmp_val = (value_array[2] / 10)*10 + (value_array[2] % 10);  
//			  sprintf(hum_tmp_buf,"Tmp:%02d Hum:%02d",tmp_val,tmp_val);
// 		}
//		else
//		{
//			  sprintf(hum_tmp_buf,"Tmp:FF Hum:FF");
//		}
//}






//void Delay_10us(void)
//{
//		unsigned char i;
//		i--;
//		i--;
//		i--;
//		i--;
//		i--;
//		i--;
//}

///*读一个字节的数据*/
//unsigned char ReadValue(void)
//{
//		unsigned char count, value = 0, i;
//		status = OK; //设定标志为正常状态
//		for(i = 8; i > 0; i--)
//		{
//				//高位在先
//				value <<= 1;
//				count = 0;
//				//每一位数据前会有一个50us 的低电平时间.等待50us 低电平结束
//				while(dht11 == 0 && count++ < NUMBER);
//				if(count >= NUMBER)
//				{
//						status = ERROR; //设定错误标志
//						return 0; //函数执行过程发生错误就退出函数
//				}
//				//26-28us 的高电平表示该位是0,为70us 高电平表该位1
//				Delay_10us();
//				Delay_10us();
//				Delay_10us();
//				//延时30us 后检测数据线是否还是高电平
//				if(dht11 != 0)
//				{
//						//进入这里表示该位是1
//						value++;
//						//等待剩余(约40us)的高电平结束
//						while(dht11 != 0 && count++ < NUMBER)
//						{
//								dht11 = 1;
//						}
//						if(count >= NUMBER)
//						{
//								status = ERROR; //设定错误标志
//								return 0;
//						}
//				}
//		}
//		return (value);
//}
////读一次的数据,共五字节
//unsigned char ReadTempAndHumi(void)
//{
//		unsigned char i = 0, check_value = 0,count = 0;
//		EA = 0;
//		dht11 = 0; //拉低数据线大于18ms 发送开始信号
//		Delay_1ms(20); //需大于18 毫秒
//		dht11 = 1; //释放数据线,用于检测低电平的应答信号
//		//延时20-40us,等待一段时间后检测应答信号,应答信号是从机拉低数据线80us
//		Delay_10us();
//		Delay_10us();
//		Delay_10us();
//		Delay_10us();
//		if(dht11 != 0) //检测应答信号,应答信号是低电平
//		{
//				//没应答信号
//				EA = 1;
//				return ERROR;
//		}
//		else
//		{
//				//有应答信号
//				while(dht11 == 0 && count++ < NUMBER); //等待应答信号结束
//				if(count >= NUMBER) //检测计数器是否超过了设定的范围
//				{
//						dht11 = 1;
//						EA = 1;
//						return ERROR; //读数据出错,退出函数
//				}
//				count = 0;
//				dht11 = 1;//释放数据线
//				//应答信号后会有一个80us 的高电平，等待高电平结束
//				while(dht11 != 0 && count++ < NUMBER);
//				if(count >= NUMBER)
//				{
//						dht11 = 1;
//						EA = 1;
//						return ERROR; //退出函数
//				}
//				//读出湿.温度值
//				for(i = 0; i < SIZE; i++)
//				{
//						value_array[i] = ReadValue();
//						if(status == ERROR)//调用ReadValue()读数据出错会设定status 为ERROR
//						{
//								dht11 = 1;
//								EA = 1;
//								return ERROR;
//						}
//						//读出的最后一个值是校验值不需加上去
//						if(i != SIZE - 1)
//						{
//								//读出的五字节数据中的前四字节数据和等于第五字节数据表示成功
//								check_value += value_array[i];
//						}
//				}	//end for
//				//在没用发生函数调用失败时进行校验
//				if(check_value == value_array[SIZE - 1])
//				{
//						//将温湿度扩大10 倍方便分离出每一位
//						humi_value = value_array[0] * 10;
//						temp_value = value_array[2] * 10;
//						dht11 = 1;
//						EA = 1;
//						return OK; //正确的读出dht11 输出的数据
//				}
//				else
//				{
//						//校验数据出错
//						EA = 1;
//						return ERROR;
//				}
//		}
//}


//void main()
//{
//	  unsigned char i;
//	  uart_init();
//	  delay_s(1);//等待1秒，跳过模块初始的不稳定
//	  while(1)
//		{
//			  delay_s(3);
//			
//	      if(ReadTempAndHumi() == OK)
//				{
//					  for(i = 0;i < 5;i ++)
//						{
//							  uart_sendbyte(value_array[i]);//打印到串口上显示
//						}
//				}
//				 	
//				for(i = 0;i < 5;i ++)
//				{
//				   value_array[i] = 0x00;
//				}  
//		}  
//}