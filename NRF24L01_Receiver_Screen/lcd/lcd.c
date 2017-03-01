#include "lcd.h"
#include "delay.h"
#include <intrins.h>
#include <reg52.h>


sbit RS = P2^4;
sbit RW = P2^5;
sbit E = P2^6;

#define DataPort P0

// void lcd_clean()
// {
// 	  lcd_write_cmd(0x01);
// 		delay_ms(15);
// }


void lcd_write_cmd(unsigned char byte)
{
	  //命令
	  RS = 0;
	  //写有效
	  RW = 0;
	  //使能有效
	  E = 1;
	  //命令内容
	  DataPort = byte;
	  //延时，等待命令输入成功
	  _nop_();
	  _nop_();
	  //关闭使能
	  E = 0;
}

void lcd_write_data(unsigned char byte)
{
	  //数据
	  RS = 1;
	  //写
	  RW = 0;
	  E = 1;
	  DataPort = byte;
	  _nop_();
	  _nop_();
	  delay_us(10);//芯片要求延时时间长一点
	  E = 0;
}

void lcd_init()
{
	  /*具体见使用手册，长沙太阳那个*/
	  delay_ms(15);
	  /*做三遍，为了确保成功*/
	  lcd_write_cmd(0x38);//设置显示模式
	  delay_ms(5);
	  lcd_write_cmd(0x38);
	  delay_ms(5);
	  lcd_write_cmd(0x38);
	  delay_ms(5);
	
	  
	  lcd_write_cmd(0x08);//显示关闭
	  delay_ms(2);
	  lcd_write_cmd(0x01);//清屏
	  delay_ms(2);
	  lcd_write_cmd(0x06);//光标关
	  delay_ms(2);
	  lcd_write_cmd(0x0c);//显示开
	  
}

//写入一个字符，在（x，Y）位置，写入byte
void lcd_write_char(unsigned char x,unsigned char y,unsigned char byte)
{
	  //第一行
	  if(0 == y)
		{
			  lcd_write_cmd(0x80 + x);
    }
		//第二行
		else if(1 == y)
		{
			  lcd_write_cmd(0x80 + 0x40 + x);
    }
		
		lcd_write_data(byte);
}


//打印一个字符串，第一个字符在x，y处
void lcd_write_str(unsigned char x,unsigned char y,unsigned char *s)
{
	  if(0 == y)
		{
			  lcd_write_cmd(0x80 + x);
    }
		else if(1 == y)
		{
			  lcd_write_cmd(0x80 + 0x40 + x);
    }
		
		//当还未到'\0'时，循环打印，光标会自动往后移动，所以不需要手动
		while(*s)
		{
			  lcd_write_data(*s);
			  s ++;
    }
}