//#include <reg52.h>
//#include "delay.h"
//#include "uart.h"
//#include <stdio.h>
//#include <string.h>
//#include "zimo.h"//包含字库头文件即可使用

#include "AllFunc.h"
#include "zimo.h"

sbit T_STR = P2^6;       //锁存引脚，屏上标识位STR
sbit R1    = P0^4;       //数据引脚，屏上标识R1
sbit T_CLK = P2^5;       //时钟引脚，屏上标识位CLK
sbit OE    = P2^1;       //使能引脚，屏上标识EN/OE
sbit A_138     = P0^0;//ABC为138译码器选择输出组合端
sbit B_138     = P0^1;
sbit C_138     = P0^2;
sbit D_138     = P0^3;//片选两片138之一



/*
进行二维数组拼接
因为后面需要动态的填充内容
定义一个指针数组，可以放128个字，用于动态显示
*/


unsigned char *OutPutStr[MAX_LEN] = {space};

unsigned char row = 0;//行，每次进入加一，上限十六行
//在二维数组中第二维中的字节位置，也就是在一行广告屏中，要显示的哪八个字节
unsigned char row_loc = 0;
bit FifteenHundred_us_Come = NO;//默认是没有到时间的，到了的话，会在while中判断然后清空
bit One_s_Come         = NO;
bit Four_hundred_ms_Come       = NO;//400ms到来的标志位，用来显示移位的

//设置一个全局变量，用来保存需要显示的中文字符个数，可以通过衡量二维数组中的一位数组长短
unsigned int char_len = 0;

unsigned char DIS[]={15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};//行数据

/*字模中的位置偏移*/
unsigned int temp_char_len = 0;//用于循环时计算字符个数，声明为静态变量，只定义一次，局部函数可见

/*
因为在一开始使得指针数组中的所有元素都指向了space，所以就不在需要填充了
只需要改变字符串长度，为静态切换服务
参数：真是字符长度
*/
void Modify_Str_Len(unsigned int *old_char_len)
{
	  unsigned char i = 0;
	
    if(((*old_char_len) % 4) != 0)//不为四的倍数个字符数目
		{
		    *old_char_len = *old_char_len + (4 - ((*old_char_len) % 4));
		}	
}


/*初始化指针数组，保证不会出现无用的杂亮*/
void init_OutPutStr()
{
	  unsigned int i = 0;
	  for(i = 0;i < 128;i ++)
	  {
			  OutPutStr[i] = space;
		}
}

void InputByte( unsigned  char DataR1) //写一个字节
{ 
		unsigned  char i;
		for(i=8; i>0; i--)
		{
			  R1 = !(DataR1&0x80);                   
			  T_CLK = 0;
			  T_CLK = 1;
			  DataR1 = DataR1 << 1; 
		} 
}

void Init_Timer0(void)
{
		 TMOD |= 0x01;			     
		 EA=1;                      //总中断使能
	   TH0 = (65536 - TIMERVAL) / 256;//200us一次
	   TL0 = (65536 - TIMERVAL) % 256; 
	
		 ET0=1;                     //定时中断使能
		 TR0=1;                     //打开定时器开关
}

/*
封装一层函数，作为接口调用
参数一：传入二维数组指针
参数二：这个二维数组的一维数组长度，也就是有多少个字要显示
*/
void showLeftRollString(unsigned char *outChar[128],unsigned char char_len)
{
	  //static unsigned int temp_char_len = 0;//用于循环时计算字符个数，声明为静态变量，只定义一次，局部函数可见
	
	  unsigned int loc_count = 0;//用于循环的变量，用于动态找到四个字符中的某一个
	
		//200us到来进入刷新
		if(FifteenHundred_us_Come == YES)
		{
				FifteenHundred_us_Come = NO;//一进入就清空标志位
				
				T_STR=0;       //锁存释放
				//打出一行的字节在点阵屏的某一行上
				for(loc_count = temp_char_len;loc_count < (    temp_char_len + 4      );loc_count ++) //四个字，字符长度在char_len中
				{
						InputByte(outChar[loc_count][row_loc]);//显示二维数组中的某两个连续的字节，点亮一块屏的16个灯
						InputByte(outChar[loc_count][row_loc + 1]);		 
				}
				OE=1;  //暂时关闭屏幕，防止出现鬼影，如果没有使用该引脚控制则会出现拖影现象。
				A_138 = DIS[row] & 0x01;
				B_138 = DIS[row] & 0x02;
				C_138 = DIS[row] & 0x04;
				D_138 = DIS[row] & 0x08;
				T_STR=1;      //锁存有效，此时一行的数据显示到屏上
				OE=0;//使能
			
				
				row ++;//逐行扫描，扫描十六行结束，也就是说一次显示四个汉字已经结束
				if(row == 16)
				{
						row = 0;
						//使得过了400ms才进行下一次的移动，视觉上比较清晰
						if(Four_hundred_ms_Come == YES)
						{
								Four_hundred_ms_Come = NO;//清空一下  
							
								temp_char_len ++;//顺序的往后显示一个字符
								//要是在上面加加的基础上，加上4就大于了最大字节数，那么就不需要再继续显示了，从头开始
								/*这里存在一点点逻辑问题，就是字符的顺序移位，何时该从头开始继续问题，留由读者辨析*/
								if((temp_char_len + 4) > char_len)
								{
										temp_char_len = 0;
								}
								
						}
				}
			 
			 //一个一维数组中包含了32个字节
				row_loc += 2;
				if(row_loc == 32)
				{
						row_loc = 0;   
				}
		}
		
	
}




/*静态显示，四字四字的变换循环刷新*/
void showStaticString(unsigned char *outChar[128],unsigned char real_char_len)
{
	  //static unsigned int temp_char_len = 0;//用于循环时计算字符个数，声明为静态变量，只定义一次，局部函数可见
		
	  unsigned int loc_count;//计数变量

	  
	  if(FifteenHundred_us_Come == YES)
		{
				FifteenHundred_us_Come = NO;//一进入就清空标志位
			
				T_STR=0;       //锁存释放
				//打出一行的字节在点阵屏的某一行上，需要判断是否是四的倍数，要不是，就要考虑最后的显示问题
			  for(loc_count = temp_char_len;loc_count < ( ((temp_char_len + 4) > real_char_len) ? real_char_len : (temp_char_len + 4)  );loc_count ++) //四个字，字符长度在char_len中
				{
						InputByte(outChar[loc_count][row_loc]);//显示二维数组中的某两个连续的字节，点亮一块屏的16个灯
						InputByte(outChar[loc_count][row_loc + 1]);		 
				}
				OE=1;  //暂时关闭屏幕，防止出现鬼影，如果没有使用该引脚控制则会出现拖影现象。
				A_138 = DIS[row] & 0x01;
				B_138 = DIS[row] & 0x02;
				C_138 = DIS[row] & 0x04;
				D_138 = DIS[row] & 0x08;
				T_STR=1;      //锁存有效，此时一行的数据显示到屏上
				OE=0;//使能
			
				row ++;//逐行扫描
				if(row == 16)
				{
						row = 0;
					
					  if(One_s_Come == YES)//一秒切换一次
						{
							  One_s_Come = NO;
							  
                if((temp_char_len + 4) < real_char_len)
								{
									  temp_char_len += 4;
								}
								else
								{
									  temp_char_len = 0;
								}
 							
						}
				}
			 
			 //一个一维数组中包含了32个字节
				row_loc += 2;
				if(row_loc == 32)
				{
						row_loc = 0;   
				}
		}
}


//封装一层函数，传入字符串，传入的可以为字符数组，本函数动态寻找字库，对OutPutStr进行拼接
/*只对常用ASCII码有效*/
/*使用了OutPutStr和char_len两个全局变量*/
/*注意：传入的字符串长度，要是为字符数组，不能使用strlen，因为没\0
  可以使用sizeof
*/
void transfer_String_to_OutPutStr(unsigned char *strbuf,unsigned int str_len)
{
	  unsigned int i = 0;
	  /*清空所有标志位等*/
    clear_used_extern();
	  /*初始化指针数组*/
    init_OutPutStr();
	  for(i = 0;i < str_len;i ++)
	  {
			  OutPutStr[i] = ASCII[(strbuf[i] - ' ')];//ASCII表从空格开始，所以下标为相对于空格的偏移
		}
		char_len = str_len;
		Modify_Str_Len(&char_len);//修改完成，同时补充空格完成，现在是专门给静态切换服务的
}

/*每次都要清空一下各个标志位和全局变量，保证下一次的从头开始正常显示*/
void clear_used_extern()
{
	  row = 0;
	  row_loc = 0;
	  temp_char_len = 0;
}

/*通过字模填充需要显示的静态数组内容*/
void showStaitcInfo(unsigned char Needed_Info[][32],unsigned int temp_len)
{
	  unsigned int i = 0;
	  /*清空所有标志位等*/
    clear_used_extern();
	  init_OutPutStr();
	
	
    for(i = 0;i < temp_len;i ++)
	  {
			  OutPutStr[i] = Needed_Info[i];
		}
		
		char_len = temp_len;
		Modify_Str_Len(&char_len);
}


//用于显示时间的数组
/*
时间长度较为固定！比较差的函数，不能灵活的修改，需要考虑的因素比较多
前面提示时间长度，后面真正时间长度两者都需要手动计算长度

*/
extern unsigned char Time_Buf_ASCII[100];

//拼装一下实时时间，显示部分另外再说
void spellRTCTime()
{
	  unsigned int i = 0;
	
	  //前面提示的字符长度为4个空格加后面五个字符
    for(i = 0;i < 9;i ++)
	  {
			  OutPutStr[i] = TimeInfo[i];
		}
		for(i = 9;i < 28;i ++)
		{
			  OutPutStr[i] = ASCII[ (Time_Buf_ASCII[i-9] - ' ')];//填充完毕
		}
		
}

/*显示动态内容，显示内容全部来自全局变量*/
void showAliveTimeInfo()
{
	  unsigned int i = 0;
	  /*清空所有标志位等*/
    clear_used_extern();
	  init_OutPutStr();
	
    spellRTCTime();//拼装时间
		
		char_len = 28;//固定输出长度就是28
		Modify_Str_Len(&char_len);
}



/*************************温湿度显示部分*********************/

///*温湿度拼装显示部分*/
extern unsigned char tmpInfo_buf[8];
void spellTmpWithChinese()
{
	  unsigned int i = 0;
	   //前面提示的字符长度为4个空格加后面五个字符
    for(i = 0;i < 9;i ++)
	  {
			  OutPutStr[i] = TmpInfo[i];
		}
		for(i = 9;i < 12;i ++)
		{
			  OutPutStr[i] = ASCII[ (tmpInfo_buf[i-9] - ' ')];//填充完毕
		}
		OutPutStr[12] = TmpInfo[7];//在最后再拼接一个度字
}

/*显示动态内容，显示内容全部来自全局变量*/
void showAliveTmpInfo()
{
	  unsigned int i = 0;
	  /*清空所有标志位等*/
    clear_used_extern();
	  init_OutPutStr();
	
    spellTmpWithChinese();//拼装时间
		
		char_len = 13;//固定输出长度就是12
		Modify_Str_Len(&char_len);
}





/*单独使用点阵屏时范例*/
//void main()
//{
//	  unsigned char i = 0;
//	  unsigned char buf[10] = "abcdefghij";
//	  
//	  uart_init();
//	  delay_ms(100);
//	  //WifiESP8266_Init("Car","01234567","8080");  
//	
//    Init_Timer0(); //初始化定时器
//    
//	  
//    transfer_String_to_OutPutStr(buf,sizeof(buf));	
//	
//	
//	  while(1)
//		{
//				//showStaticString(OutPutStr,char_len);
//			  showLeftRollString(OutPutStr,char_len);
//			  
//			
//		}
//		
//}







/***********************德飞莱**************************/


