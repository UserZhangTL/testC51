#include <reg52.h>
#include "lcd.h"
#include "delay.h"
#include "tmp.h"

//三个实时时钟的端口
sbit SCLK = P2^2;
sbit IO = P2^1;
sbit RST = P2^0;

//需要输入的时间,时分秒，年月日，星期几
unsigned char timebuf[7] = {15,42,0,15,11,16,1};
//被读出来的时间
unsigned char tempbuf[7];


//ds1302中，时分秒等的寄存器地址
#define ds1302_sec_add			0x80		
#define ds1302_min_add			0x82		
#define ds1302_hr_add			0x84		
#define ds1302_date_add			0x86		
#define ds1302_month_add		0x88		
#define ds1302_day_add			0x8a		
#define ds1302_year_add			0x8c		
#define ds1302_control_add		0x8e		
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe



//在addr地址指向的内容中写入byte
void ds1302_writebyte(unsigned char addr,unsigned char byte)
{
	unsigned char i = 0;
	RST = 0;
	SCLK = 0;
	RST = 1;
	addr = addr & 0xfe;  //取消写保护
	for(i = 0;i < 8;i++)
	{
		IO = addr & 0x01;//取出每一位，从最低位开始
		SCLK = 0;
		SCLK = 1;//上升沿，写入
		addr = addr >> 1; //循环左移
	}
	//在地址指向的寄存器中写入内容
	for(i = 0; i < 8; i++)
	{
		IO = byte & 0x01;
		SCLK = 0;
		SCLK = 1;
		byte = byte >> 1;
	}
	//最后都置零，参考手册
	RST = 0;
	SCLK = 0;
}


//从add地址指向的内容中读出数据
unsigned char ds1302_readbyte(unsigned char addr)
{
	  unsigned char i = 0,temp;
		
		RST = 0;
		SCLK = 0;
		RST = 1;
	  //设置写保护
	  addr = addr | 0x01;
	  //写入地址
		for(i = 0;i < 8;i++)
		{
			IO = addr & 0x01;
			SCLK = 0;
			SCLK = 1;
			addr = addr >> 1;
		}
		//读出数据
		for(i = 0; i < 8; i++)
		{
			 
			 SCLK = 1;
			 SCLK = 0;//下降沿以后数据准备好，可读
			 temp = temp >> 1;
			 if(IO) temp = temp + 0x80;//要是IO是1才加在最高位，从最低位开始读入
		}
		
		RST = 0;
		return temp;
}

void ds1302_writetime()
{
	 unsigned char i;
	 unsigned char temp;
	 for(i = 0; i < 7; i++)
	 {
		   //取出十位
		   temp = timebuf[i] / 10;
		   //变为个位
		   timebuf[i] = timebuf[i] % 10;
		   //将十位左移，到高四位，然后把个位放在低四位上
		   timebuf[i] = (temp << 4) + timebuf[i];
	 }
	 ds1302_writebyte(ds1302_control_add,0x00); //关掉写保护
	 ds1302_writebyte(ds1302_hr_add,timebuf[0]);//时
	 ds1302_writebyte(ds1302_min_add,timebuf[1]);//分
	 ds1302_writebyte(ds1302_sec_add,timebuf[2]);//秒
	 ds1302_writebyte(ds1302_year_add,timebuf[3]);//年
	 ds1302_writebyte(ds1302_month_add,timebuf[4]);//月
	 ds1302_writebyte(ds1302_date_add,timebuf[5]);//日
	 ds1302_writebyte(ds1302_day_add,timebuf[6]);//星期几
	 
	 ds1302_writebyte(ds1302_control_add,0x80); //打开写保护
}

void ds1302_readtime()
{
	  unsigned char i;
	  unsigned char temp;
	  //读出数据
	  tempbuf[0] = ds1302_readbyte(ds1302_hr_add);
	  tempbuf[1] = ds1302_readbyte(ds1302_min_add);
	  tempbuf[2] = ds1302_readbyte(ds1302_sec_add);
	  tempbuf[3] = ds1302_readbyte(ds1302_year_add);
	  tempbuf[4] = ds1302_readbyte(ds1302_month_add);
	  tempbuf[5] = ds1302_readbyte(ds1302_date_add);
  	tempbuf[6] = ds1302_readbyte(ds1302_day_add);
	  
	  //对读出的数据做处理，使其变为十进制显示
	  for(i = 0; i < 7; i++)
	  {
			
			  //将高四位取出乘十加上低四位，变为可以显示的数字
			  temp = tempbuf[i] >> 4;  //十位
			  tempbuf[i] = tempbuf[i] & 0x0f;
			  tempbuf[i] = (temp * 10) + tempbuf[i];
		}
}

void distime()
{
	  unsigned char disbuf[9] = {0};
    unsigned char disdata[15] = {0};
    disbuf[0] = tempbuf[0] / 10 + 0x30;
    disbuf[1] = tempbuf[0] % 10 + 0x30;
    disbuf[2] = ':';
    disbuf[3] = tempbuf[1] / 10 + 0x30;
    disbuf[4] = tempbuf[1] % 10 + 0x30;
    disbuf[5] = ':';
    disbuf[6] = tempbuf[2] / 10 + 0x30;
    disbuf[7] = tempbuf[2] % 10 + 0x30;

    lcd_write_str(0,0,disbuf);
		show_tmp();
		

    disdata[0] = '2';
    disdata[1] = '0';
    disdata[2] = tempbuf[3] / 10 % 10 + 0x30;
    disdata[3] = tempbuf[3] % 10 + 0x30;
    disdata[4] = '/';

    disdata[5] = tempbuf[4] / 10 + 0x30;
    disdata[6] = tempbuf[4] % 10 + 0x30;
    disdata[7] = '/';

    disdata[8] = tempbuf[5] / 10 + 0x30;
    disdata[9] = tempbuf[5] % 10 + 0x30;
		
    disdata[10] = ' ';		
		
		switch(tempbuf[6])
		{
			  case 1:
				{
						disdata[11] = 'M';
					  disdata[12] = 'o';
					  disdata[13] = 'n';
					  break;
				}
				case 2:
				{
					  disdata[11] = 'T';
					  disdata[12] = 'u';
					  disdata[13] = 's';
					  break;
				}
				case 3:
				{
					  disdata[11] = 'W';
					  disdata[12] = 'e';
					  disdata[13] = 'n';
					  break;
				}
				case 4:
				{
					  disdata[11] = 'T';
					  disdata[12] = 'h';
					  disdata[13] = 'r';
					  break;
				}
				case 5:
				{
					  disdata[11] = 'F';
					  disdata[12] = 'r';
					  disdata[13] = 'i';
					  break;
				}
				case 6:
				{
					  disdata[11] = 'S';
					  disdata[12] = 'a';
					  disdata[13] = 't';
					  break;
				}
				case 7:
				{
					  disdata[11] = 'S';
					  disdata[12] = 'u';
					  disdata[13] = 'n';
					  break;
				}
				default:
					  disdata[11] = '*';
					  disdata[12] = '*';
					  disdata[13] = '*';
				    break;
		}

		lcd_write_str(1,1,disdata);
}



// void main()
// {
// 	  lcd_init();
// 	  lcd_clean();
// 	  //ds1302_writetime();//写过一次了就不需要再写了
// 	  while(1)
// 		{
// 			  ds1302_readtime(); 
// 			  distime();
// 			  delay_ms(50);
// 		}
// }