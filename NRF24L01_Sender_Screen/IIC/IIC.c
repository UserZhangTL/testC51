#include <reg52.h>
#include "delay.h"
#include "lcd.h"
#include "IIC.h"

#define ERR 1
#define SCC 0

sbit SCK = P2^1;
sbit SDA = P2^2;

bit ack = 0;


void IIC_start()
{
	  SDA = 1;
	  SCK = 1;
	  delay_us(1);
	  SDA = 0;
	  delay_us(1);
	
	  SCK = 0;
}

void IIC_stop()
{
	  SDA = 0;
	  SCK = 1;
	  delay_us(1);
	  SDA = 1;
	  delay_us(1);
	
	  SCK = 0;
}

void IIC_SendByte(unsigned char temp)
{
	  unsigned char i = 0;
	  for(i = 0;i < 8;i ++)
	  {
			  SDA = temp & 0x80;
			  SCK = 1;
			  delay_us(1);
			  SCK = 0;
			  delay_us(1);
			
			  temp <<= 1;
    }
		
		SDA = 1;
		SCK = 1;
		delay_us(7);
		if(SDA == 0)
		{
			  ack = 1;
		}
		else
		{
			  ack = 0;
    }
		
		SCK = 0;
}


void IIC_ACK()
{
	  SDA = 0;
	  SCK = 1;
	  delay_us(1);
	  SCK = 0;
}

void IIC_NOACK()
{
	  SDA = 1;
	  SCK = 1;
	  delay_us(1);
	  SCK = 0;
}

unsigned char IIC_RcvByte()
{
	  unsigned char i = 0,temp;
	  SDA = 1;
	  for(i = 0;i < 8;i ++)
	  {
			  SCK = 0;
			  delay_us(1);
			  SCK = 1;
			  temp <<= 1;
			  if(1 == SDA)
				{
					  temp += 1;
				}
    }
		
		SCK = 0;
		return temp;
}

void check_ack()
{
	  if(0 == ack)
		{
			  while(1)
				{
					  P0 = 0xf0;
				}
    }

}

//д��
unsigned char AT24C02_SendStr(unsigned char deviceaddr,unsigned char romaddr,unsigned char *s,unsigned char num)
{
	  unsigned char i;
	  IIC_start();
	  IIC_SendByte(deviceaddr);
	  if(0 == ack)
			return ERR;
		IIC_SendByte(romaddr);
		if(0 == ack)
			return ERR;
		for(i = 0;i < num;i ++)
		{
			  IIC_SendByte(*s);
			  if(0 == ack)
			     return ERR;
				s ++;
		}
		IIC_stop();
		return SCC;
}

//����
unsigned char AT24C02_RcvStr(unsigned char deviceaddr,unsigned char romaddr,unsigned char *s,unsigned char num)
{
	  unsigned char i;
	  IIC_start();
	  IIC_SendByte(deviceaddr);
	  if(0 == ack)
			return ERR;
		IIC_SendByte(romaddr);
		 if(0 == ack)
			return ERR;
		IIC_start();
		 IIC_SendByte(deviceaddr + 1);
		 if(0 == ack)
			return ERR;
		 for(i = 0;i < num - 1; i++)
		 {
			   *s = IIC_RcvByte();
			   IIC_ACK();
			   s ++;
		 }
		 *s = IIC_RcvByte();
		 IIC_NOACK();
		   IIC_stop();
		 
		 return SCC;
}


// void main()
// {
//     unsigned char sor[] = {'1','2','3','4','5','6','0'};
//     unsigned char mu[7] = {0};

//     AT24C02_SendStr(0xae,0,sor,7);
// delay_ms(200);
//     AT24C02_RcvStr(0xae,0,mu,7);

//     delay_ms(200);

//     lcd_init();
//     lcd_write_str(1,1,mu);
//     

// 		while(1);
// }
