/********************使用示例**********************************/

// /*测试ESP8266，目的:获得回显信息*/

// #include <reg52.h>
// #include "delay.h"
// #include "WifiESP8266.h"
// #include "lcd.h"
// #include "uart.h"
// #include "delay.h"
// #include <stdio.h>
// #include <string.h>

// extern bit Ok_flag;
// extern bit Str_Ready;
// extern bit Rev_Str_status;
// extern bit str_rev_flag;
// extern bit flag;
// extern unsigned char Get_str[40];
// extern int client_num;
// extern int str_len;


// void main()
// {						  	  
// 	  unsigned char temp_buf[16] = "Tmp:26 Hum:56";
// 	  int i = 0;
// 	  uart_init();
// 	
// 	  lcd_init();
// 	  lcd_clean();
// 	  delay_ms(500);  
// 	 
// 	  WifiESP8266_Init("ZTLTest","0123456789","8000");
// 	
// 	  while(1)
// 		{
// 				if(OK == SendStrToClint(temp_buf,0,strlen(temp_buf)))
// 					//发送给连接号为0的客户，hhh，三个字节长度
// 				{
// 						lcd_clean();
// 						lcd_write_str(1,1,"SEND OK");
// 				}
// 				else
// 				{
// 						lcd_clean();
// 						lcd_write_str(1,1,"ERROR");
// 				}
//         
// 				if(check_revStr() == OK)//也可以改成while在某个函数体中做
// 				{
// 						/*************写对获取的字符串的操作**************/
// 						sprintf(temp_buf,"%d %d %s\0",client_num,str_len,Get_str);
// 						lcd_write_str(0,0,temp_buf);
// 						/*****************************************/
// 						
// 					
// 						/*****************测试接收成功后，回复信息，可不加******************/
// // 						if(OK == SendStrToClint(Get_str,0,strlen(Get_str)))
// // 							//发送给连接号为0的客户，hhh，三个字节长度
// // 						{
// // 								lcd_clean();
// // 								lcd_write_str(1,1,"SEND OK");
// // 						}
// // 						else
// // 						{
// // 								lcd_clean();
// // 								lcd_write_str(1,1,"ERROR");
// // 						}
// 						/******************************************************/
// 					
// 						
// 						/**************************必须要加！********************************/
// 						clean_flag();//调用wifi模块中的清空函数
// 						/*******************************************************/
// 				}
// 				
// 				

// 		}
// }








/************************************************************/
/*
wifi模块ESP8266模块测试函数
作者：张天乐
起始时间：2016/1/19
功能：完成基本的连接等功能

修改时间：2016/1/28
功能：能够实现区分哪个客户端发来的什么数据
*/

#include "AllFunc.h"

//接收状态默认为结束
bit Rev_status      = END;//接收状态位
bit Rev_Str_status  = END;//接收字符串状态位
bit Send_flag       = NO;//发送信息到手机状态位
bit OK_flag         = NO;//成功状态位
bit Str_Ready       = NO;//数据准备好状态位
bit First_dou_flag  = NO;//是不是第一个逗号

//获取的字符串内容
unsigned char  Get_str[40];
unsigned char Get_str_loc = 0;

//客户连接号，以及字符串长度
int client_num = 0;
int wifi_str_len    = 0;
bit client_num_flag      = END;
bit str_len_flag         = END;
bit str_rev_flag         = END;



/*广告屏使用到的一些标志位*/
extern bit LastInfo_Flag;//切换为上一个内容
extern bit NextInfo_Flag;//切换为下一个内容
extern unsigned char MODE_CHANGE;//模式切换的flag

/*开始采集命令发出*/
extern bit COLLECT_TMP_FLAG;





//检测是否接收到了数据，供其他函数调用，如果接收到了数据，那么，就供其他函数提取全局变量Get_str
//在其他函数接收完以后，还要调用清空函数，清空标志位，以便下一次使用
bit check_revStr()
{
    if(Str_Ready == OK)
		{
		    return OK;
		}
		return NO;
}
	
void clean_flag()
{
	  //清空操作
	  memset(Get_str,0,sizeof(Get_str));
	  client_num = 0;
	  wifi_str_len = 0;
	  Get_str_loc = 0;//清空一部分全局变量，以便下一次操作,Get_str要在外面函数接收完再清空
	  Str_Ready = NO;//清空接收数据准备好标志位
}


/*未完善，没有检测是否发送成功，将字符串的截取和解析加入*/
//发送字符串给给客户端连接号为num的客户端，内容为参数str
//str：发送内容，str_len：发送的字节数，client_num:客户端连接号，注意：参数均为字符串形式！
bit SendStrToClint(unsigned char *str,int client_num,int str_len)//有待加入字符串解析，用于判断是否发送成功
{
	  //用于拼接AT指令的buffer
	  unsigned char AT_temp[30] = {0};
    //拼接成： "AT+CIPSEND=client_num,str_len\r\n"  设置发送，=后面第一个参数是客户连接号即client_num,第二个是需要发送的字节数

    sprintf(AT_temp,"AT+CIPSEND=%d,%d\r\n",client_num,str_len);
    uart_sendstr(AT_temp);//将封装完的AT指令发送出去
    delay_ms(50);
		//发送需要发送的字节
    uart_sendstr(str);
    delay_ms(200);
    //经过一秒以后，查看是否有发送成功标志位，要是发送成功了，那么就置Send_flag为有效
    if(Send_flag == NO)
		{
	      return NO;
		}
		else
		{
			  //发送完成以后，还要清空标志位为无效，以便下次使用
			  Send_flag = NO;
			  return OK;
		}
}

//wifi模块的初始化函数  wifi名字和wifi密码和端口名称，IP名固定为192.168.4.1
void WifiESP8266_Init(unsigned char *name,unsigned char *password,unsigned char *port)
{
	  //用于拼接AT字符串命令
	  unsigned char AT_tempBuf[50] = {0};

	  //发送AT指令，设置wifi模式等
	  uart_sendstr("AT+RST\r\n");//重启
	  delay_s(1);

	  uart_sendstr("AT+CWMODE=2\r\n");//设置为AP模式，wifi模块当做路由器
	  delay_s(1);

    //设置wifi名称和密码
    sprintf(AT_tempBuf,"AT+CWSAP=\"%s\",\"%s\",11,4\r\n",name,password);
    uart_sendstr(AT_tempBuf);
    memset(AT_tempBuf,0,sizeof(AT_tempBuf));//用完清空
    delay_s(1);

    uart_sendstr("AT+RST\r\n");//重启
	  delay_s(1);

    uart_sendstr("AT+CIPMUX=1\r\n");//设为多路
	  delay_s(1);

    //开始拼接带有端口号的字符串
		sprintf(AT_tempBuf,"AT+CIPSERVER=1,%s\r\n",port);
	  uart_sendstr(AT_tempBuf);//打开服务，需要拼接
		memset(AT_tempBuf,0,sizeof(AT_tempBuf));//用完清空
	  delay_s(1);
}




//中断服务函数，用于软复位
void uart_isr() interrupt 4
{
	  //loc用来对ret_msg全局变量给偏移，用来组装一个字符串
	  static unsigned char i = 0;
	  unsigned char temp;
	  ES = 0;
	  temp = SBUF;//temp不能被改变，因为软复位需要用到
	
	  switch(temp)
		{
			//上一个内容
			case '$':
			{
				  LastInfo_Flag = YES;
				  break;
			}
			case '#':
			{
				  NextInfo_Flag = YES;
				  break;
			}
			//左键，切换为滚动模式
			case '&':
			{
				  MODE_CHANGE = ROLL;
				  break;
			}
			//右键，切换为静态切换模式
			case '*':
			{
				  MODE_CHANGE = STATIC;
				  break;
			}
			//采集命令
			case '[':
			{
				  COLLECT_TMP_FLAG = YES;
				  break;
			}
			
			default:break;
		}			
	
	
	
	
	  /*ESP8266截取字符串部分*/
    if(temp == '\n')//开始符
		{
			  Rev_status = BEGIN;//设置开始接收
		}
		else if(temp == '\r')//结束符
		{
			  Rev_status = END;//设置结束接收
		}
		else//出来\r\n以外的字符
		{
			
				/************************************************/
				//专门用来接收IPD和CIFSR，结束接收时要把Rev_Str_status置为无效，再次进入下面的循环，检测第一个字符
				if(Rev_Str_status == BEGIN)
				{
					  if((str_rev_flag == END) &&(temp != ':') && (str_len_flag == BEGIN))//开始接收字符串长度
						{
							  wifi_str_len = wifi_str_len * 10 + (temp - '0');
						}
						//为了充分保证，只有一种情况，并且进入一次，采用多个flag
					  if((str_rev_flag == END) &&(First_dou_flag == OK) && (temp == ',') && (client_num_flag == BEGIN) && (str_len_flag == END))//再一次接收到了逗号，开始接收字符串长度
						{
							  str_len_flag = BEGIN;
							  client_num_flag = END;
						}
						//第一个逗号来临，进入，以后不再进入
					  if((str_rev_flag == END) &&(First_dou_flag == NO) && (temp == ',') && (client_num_flag == END))//逗号来临
						{
							  client_num_flag = BEGIN;
							  First_dou_flag = OK;//是第一个逗号
						}
						//开始接收用户连接号
						if((str_rev_flag == END) && (temp != ',') && (client_num_flag == BEGIN))
						{
							  client_num = client_num * 10 + (temp - '0');
						}
						
						//开始接收字符串
						if(str_rev_flag == BEGIN)
						{
							  //保存字符串到全局变量中，以便后面输出
							  Get_str[Get_str_loc] = temp;
							  Get_str_loc ++;
							  //如果字符串的长度和刚刚接收到的指明字符串长度相同，则不再接收，做收尾工作
							  if(Get_str_loc == wifi_str_len)
								{
									  Get_str[Get_str_loc] = '\0';
									  Str_Ready       = OK;//设置标志位，说明我已经接收到了一个整的字符串了，可以进行操作了。
									  Rev_Str_status  = END;//清空接收字符串标志位，使得可以再次进入下面switch循环
									  client_num_flag = END;//清空接收client_num标志位
										str_len_flag    = END;//清空接收str_len字符串长度标志位
										str_rev_flag    = END;//清空接收真正字符串标志位
									  First_dou_flag  = NO;//清空区分第几个逗号的标志位
								}
						}
						
						if(temp == ':')//要是开始了：，那么后面开始接收字符长度为str_len的字符串长度
						{
							  str_rev_flag = BEGIN;
						}
				}
				/************************************************/
			
			 /************************************************/
			  //要在接收字符串的标志位无效，并且接收状态位有效的时候才做
			  if((Rev_Str_status == END) && (Rev_status == BEGIN))
				{
					  //检测到第一个以后，立刻置接收标志位无效暂时不接收
					  switch(temp)
					  {
								case '+'://要么接收到+IPD 要么接收到 +CIFSR
								{
									  Rev_Str_status = BEGIN;//开始接收，暂时不进入这个switch循环
										Rev_status = END;
									  break;
								}
								case 'E'://发送失败，回显ERROR
								{
									  Send_flag = NO;
									  Rev_status = END;
									  break;
								}
								case 'S'://发送成功，回显SEND OK
								{
									  Send_flag = OK;
									  Rev_status = END;
									  break;
								}
								default:
								{
									  Rev_status = END;
									  break;
								}
						}
				}

		}
	  
	  
	

		//软复位时使用
	  if(0x7f == temp)//special for Doflye
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



