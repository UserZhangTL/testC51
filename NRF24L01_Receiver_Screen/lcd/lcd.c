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
	  //����
	  RS = 0;
	  //д��Ч
	  RW = 0;
	  //ʹ����Ч
	  E = 1;
	  //��������
	  DataPort = byte;
	  //��ʱ���ȴ���������ɹ�
	  _nop_();
	  _nop_();
	  //�ر�ʹ��
	  E = 0;
}

void lcd_write_data(unsigned char byte)
{
	  //����
	  RS = 1;
	  //д
	  RW = 0;
	  E = 1;
	  DataPort = byte;
	  _nop_();
	  _nop_();
	  delay_us(10);//оƬҪ����ʱʱ�䳤һ��
	  E = 0;
}

void lcd_init()
{
	  /*�����ʹ���ֲᣬ��ɳ̫���Ǹ�*/
	  delay_ms(15);
	  /*�����飬Ϊ��ȷ���ɹ�*/
	  lcd_write_cmd(0x38);//������ʾģʽ
	  delay_ms(5);
	  lcd_write_cmd(0x38);
	  delay_ms(5);
	  lcd_write_cmd(0x38);
	  delay_ms(5);
	
	  
	  lcd_write_cmd(0x08);//��ʾ�ر�
	  delay_ms(2);
	  lcd_write_cmd(0x01);//����
	  delay_ms(2);
	  lcd_write_cmd(0x06);//����
	  delay_ms(2);
	  lcd_write_cmd(0x0c);//��ʾ��
	  
}

//д��һ���ַ����ڣ�x��Y��λ�ã�д��byte
void lcd_write_char(unsigned char x,unsigned char y,unsigned char byte)
{
	  //��һ��
	  if(0 == y)
		{
			  lcd_write_cmd(0x80 + x);
    }
		//�ڶ���
		else if(1 == y)
		{
			  lcd_write_cmd(0x80 + 0x40 + x);
    }
		
		lcd_write_data(byte);
}


//��ӡһ���ַ�������һ���ַ���x��y��
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
		
		//����δ��'\0'ʱ��ѭ����ӡ�������Զ������ƶ������Բ���Ҫ�ֶ�
		while(*s)
		{
			  lcd_write_data(*s);
			  s ++;
    }
}