/*20170301*/

#include "AllFunc.h"

/*点阵屏功能模块使用到的一些全局变量*/
extern bit FifteenHundred_us_Come;//默认是没有到时间的，到了的话，会在while中判断然后清空
extern bit One_s_Come;
extern bit Four_hundred_ms_Come;//400ms到来的标志位，用来显示移位的
extern unsigned char *OutPutStr[MAX_LEN];//用于输出显示的指针数组
extern unsigned char row;//行，每次进入加一，上限十六行
//在二维数组中第二维中的字节位置，也就是在一行广告屏中，要显示的哪八个字节
extern unsigned char row_loc;
extern unsigned int char_len;
extern unsigned int temp_char_len;//显示函数中使用的字模偏移

extern unsigned char DIS[];//行数据


//bit OPEN_Flag = NO;//默认开关是关闭的，用按键九进行开启或者关闭，或者其他功能按钮开启


/*wifi模块中使用的*/
//接收到的数据和长度
extern unsigned char  Get_str[40];
extern unsigned char Get_str_loc;
bit print_flag = NO;
bit LastInfo_Flag = NO;//切换为上一个内容
bit NextInfo_Flag = NO;//切换为下一个内容
unsigned char ContentCount = 6;//用于计算显示第几个内容从0-6，一共7个内容，默认为关闭模式

extern int wifi_str_len;//引用wifi模块函数中的字符长度变量

/*模式选择*/

unsigned char MODE_CHANGE = ROLL;//模式选择

/*实时时钟使用到的变量*/
extern unsigned char Time_Buf_ASCII[100];//这个里面会存储定时采集出来的时间
bit Collect_Time_Flag_s = NO;//设置采集时钟信号的flag，定时采样，不能一直采样

/*温度采集命令到来*/
bit Collect_Tmp_Flag_4s = NO;//采集DS18B20命令
bit COLLECT_TMP_FLAG = NO;//手机发送来的采集命令，要发送信息到手机上去了
extern int real_tmp;
int tempStorageTmp;//用来存储比较正常的温度值


/*用于根据内容的编号，进行循环的显示，主要用在手机客户端的向上向下按钮上*/
void showContentBy_ContentCount()
{
	  switch(ContentCount)
		{
			//显示时间，但是这里还没有做动态的填充内容
			case 0:{showAliveTimeInfo();break;}
			//显示温湿度
			case 1:{showAliveTmpInfo();break;}
			//显示感谢信息
			case 2:{showStaitcInfo(ThankInfo,21);break;}
			//显示正在验收项目
			case 3:{showStaitcInfo(IntroductionInfo1,19);break;}
			//显示使用模块
			case 4:{showStaitcInfo(IntroductionInfo2,40);break;}
			//显示两种通信方法
			case 5:{showStaitcInfo(IntroductionInfo3,26);break;}
			//出错处理
			default:{ContentCount = 0;break;}
		}
}


/*专门为实时时间和温度采集做的检测，要实时检测，然后采集数据*/
void specialForTimeTmp_Collecting()
{
	  /*专门为实时时钟设立*/
		if(ContentCount == 0)//如果Count==0，也就是说在显示时间，那么这时候我需要动态填充，一秒一次
		{
				/*实时时钟采集部分，这里单纯做一秒采集，不包含拼装和显示内容*/
				if(Collect_Time_Flag_s == YES)
				{
						Collect_Time_Flag_s = NO;//清空标志位，开始采集
						//采集时间，采集到的时间被拼装成固定的格式在Time_Buf_ASCII中，全部是ASCII码
						P8563_gettime();
						spellRTCTime();//拼装时间
				}
		}
		/*专门为温度设立*/
		else if(ContentCount == 1)
		{
				/*温湿度信号来临*/
				if(Collect_Tmp_Flag_4s == YES)
				{
						Collect_Tmp_Flag_4s = NO;//清空标志位，开始采集
						getTmp_and_Sprintf();
					  ModifyTmp();//调整采集到的值
						spellTmpWithChinese();//拼装时间
				}
		}
}




/******************************************主函数***************************************************************/
void main()
{
    unsigned char RecData[5] = {0};//由于传输的数据宽度是5个字节，这里也使用五个字节作为一次数据
    unsigned char i = 0;
		
		/*用于拼装显示到手机上的信息*/
    unsigned char sendToClientBuf[50] = {0};
		
		int str_loc = 0;
		delay_s(2);
		uart_init();
		
		//采集一次温度
		getTmp_and_Sprintf();
		if(real_tmp != 0)//首次采集就放入到保存的变量中
		{
			  tempStorageTmp = real_tmp;
		}
		else
    {
			  tempStorageTmp = 25;//要是失败了，那么先放入比较适宜的温度值，等待下一次更新
		}
		
    Init_Timer0();
	  NRF24L01_Init();
    uart_init();
	  delay_s(1);
		
		//要是要重新设置当前实时时钟，才要调用这个，然后在数组中填充日期的十六进制的BCD码，一般只要写一次就可以
		//P8563_init();		
		
		WifiESP8266_Init("Screen","0123456789","8080");
		
		ContentCount = 6;//默认开始为关闭模式
		//首先显示提示信息
		 showStaitcInfo(InitInfo,4);
		
		//如果按下按键，则说明要设置为发送模式，发送数据
	  //想要测试到STATUS = 0x2e，需要下载一遍，然后立马开串口，会出现2E,说明发送成功
    SetNRF_ReceiveDataMode();
    while(1)
		{ 
        /*发送提示信息到手机，包括温度和采集错误的提示信息*/
        if(COLLECT_TMP_FLAG == YES)
				{
   					COLLECT_TMP_FLAG = NO;//清空操作
					  getTmp_and_Sprintf();//更新一次数据
					  ModifyTmp();
					  sprintf(sendToClientBuf,"Tmp:%03d",tempStorageTmp);
					  SendStrToClint(sendToClientBuf,0,strlen(sendToClientBuf));
					  memset(sendToClientBuf,0,sizeof(sendToClientBuf));//清空为下一次操作准备
				}					
			
			
			
				/*模式切换部分*/
				if(MODE_CHANGE == ROLL)
				{
						specialForTimeTmp_Collecting();
						showLeftRollString(OutPutStr,char_len);
				}
				else if(MODE_CHANGE == STATIC)
				{
						specialForTimeTmp_Collecting();
						showStaticString(OutPutStr,char_len);
				}
				
				/*通过wifi发来的命令辨析，切换内容*/
				if(NextInfo_Flag == YES)
				{
						NextInfo_Flag = NO;
						if(ContentCount >= 5)
						{
								ContentCount = 0;
						}
						else
						{
								ContentCount ++;
						}
						//以上修改完成，就开始显示固定的内容
						showContentBy_ContentCount();
				}
				else if(LastInfo_Flag == YES)
				{
						LastInfo_Flag = NO;
						if(ContentCount == 0)
						{
								ContentCount = 5;
						}
						else
						{
								ContentCount --;
						}
						showContentBy_ContentCount();
				}
			

				/*如果有了wifi发送信息过来，那么要进入做一些处理*/
							/*用于打印收到的数据*/
				if(check_revStr() == OK)//也可以改成while在某个函数体中做
				{
						/*************写对获取的字符串的操作**************/
						//大于1的字符串，可以准备打印
						if(wifi_str_len > 1)
						{
								//遍历字符串，如果包含命令符，不打印
								for(str_loc = 0;str_loc < wifi_str_len; str_loc ++)
								{
										if((Get_str[str_loc] == '$') || (Get_str[str_loc] == '#')|| (Get_str[str_loc] == '^')
											|| (Get_str[str_loc] == '&')|| (Get_str[str_loc] == '*')|| (Get_str[str_loc] == '['))
										{
												print_flag = NO;
										}
								}
								//如果能够打印，打印出来，不能打印要还原标志位能够打印的状态
								if(print_flag == OK)
								{
										/*调用纯ASCII码接口，拼装字符串*/
										/*点阵屏部分，注意要传入真实正确的字符个数，只能是ASCII码*/
										transfer_String_to_OutPutStr(Get_str,Get_str_loc);
									  //这里做这一步，是为了让显示实时时间和温度的时候，不掩盖手机发送来的数据
									  ContentCount = 6;
								}
								else//还原默认的能够打印标志位
								{
										print_flag = OK;
								}

						}
						clean_flag();//调用wifi模块中的清空函数
				}
	

			  
				
			  /*如果接收到了命令数据*/
			  if(NRF_RecData(RecData))
				{
					  //开始解析第第三个字节，该字节作为命令字
					  switch(RecData[2]) 
						{
							case 0x01:
							{
								  ContentCount = 0;//内容计数标志，0表示这时候是显示时间模式 
								  showAliveTimeInfo();//显示时间，但是这里还没有做动态的填充内容
								
								  break;
							}
							case 0x02:
							{
								  ContentCount = 1;//内容计数，1表示温湿度显示模式
								  showAliveTmpInfo();/*动态显示温湿度*/
								  break;
							}
							//显示感谢信息
							case 0x03:
							{ContentCount = 2;showStaitcInfo(ThankInfo,21);break;}
							//显示正在验收项目
							case 0x04:{ContentCount = 3;showStaitcInfo(IntroductionInfo1,19);break;}
							//显示使用模块
							case 0x05:{ContentCount = 4;showStaitcInfo(IntroductionInfo2,40);break;}
							//显示两种通信方法
							case 0x06:
							{ContentCount = 5;showStaitcInfo(IntroductionInfo3,26);break;}
							case 0x07:{MODE_CHANGE = ROLL;break;}
							case 0x08:{MODE_CHANGE = STATIC;break;}
							//显示初始化内容
							case 0x09:{ContentCount = 6;showStaitcInfo(InitInfo,4);break;}
							//向右手势命令，切换下一个内容
							case 0x0a:{NextInfo_Flag = YES;break;}
							//没有命令时的操作
							default:{break;}
						}
					
				}
			
				memset(RecData,0,4*sizeof(unsigned char));//每次接收完清空
    }
}

/*-----------------------------------------------
             定时器执行动态扫描
------------------------------------------------*/
void Timer0_isr(void) interrupt 1
{
	  static unsigned int counter_s  = 0;
    static unsigned int counter_us = 0;
	  static unsigned int counter_ms = 0;
	  
	  static unsigned int counter_s_PCF8563 = 0;//专门为实时时钟定义一个计数变量
	  static unsigned int counter_3s_Tmp = 0;
	
    TH0 = (65536 - TIMERVAL) / 256;
	  TL0 = (65536 - TIMERVAL) % 256;  
	
	  counter_s  ++;
	  counter_us ++;
	  counter_ms ++;
	
	  counter_s_PCF8563 ++;
	  counter_3s_Tmp ++;
	
	  if(counter_us == US_COUNT_MAX)//进来一次就置为有效，但如果while中没有清空，就一直有效
		{
	      FifteenHundred_us_Come = YES;
			  counter_us = 0;//清空
		}
		if(counter_ms == MS_COUNT_MAX)//左移速度大约4-5ms移位一次
		{
			  Four_hundred_ms_Come = YES;
			  counter_ms = 0;
		}
	  if(counter_s == S_COUNT_MAX)//大约1s切换静态内容显示
		{
			  counter_s = 0;//清空
			  One_s_Come = YES;//标志位有效
		}
		
		
		if(counter_s_PCF8563 == S_COUNT_MAX_PCF8563)
		{
			  counter_s_PCF8563 = 0;//清空计数
			  Collect_Time_Flag_s = YES;
		}
		
		if(counter_3s_Tmp == S_COUNT_MAX_TMP)
		{
			  counter_3s_Tmp = 0;
			  Collect_Tmp_Flag_4s = YES;
		}

}
