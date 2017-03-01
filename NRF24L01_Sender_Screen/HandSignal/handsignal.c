#include "AllFunc.h"


sbit Left_Avoid = P3^1;//左边的壁障模块
sbit Right_Avoid = P1^6;//右边的壁障模块

unsigned char Direction_flag = NO;//
bit One_s_Come = NO;//10ms来临状态位


void getHandSignal()//使用全局变量标志位Direction_flag表示
{
	  if((Left_Avoid == 0) || (Right_Avoid == 0))
		{
				if((Left_Avoid == 0) && (Right_Avoid == 1))//右摆
				{
						One_s_Come = NO;//开始计数了，先清空一下，保证下面计数有效
						while((One_s_Come == NO) && (Right_Avoid == 1));//两者之一跳出
						if(One_s_Come == YES)//修改到0了，说明没有正确操作，退出
						{
								Direction_flag = NO;
						}
						else if(Right_Avoid == 0)
						{ 
							  delay_ms(5);
                if((Right_Avoid == 0) && (Left_Avoid == 1))	
                {									
								    Direction_flag = RIGHT;
								}
						}
				}
				
		}
	
	
	

}


