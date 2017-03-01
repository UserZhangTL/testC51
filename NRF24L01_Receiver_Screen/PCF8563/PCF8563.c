#include "AllFunc.h"


extern uchar TX_buf[10];
extern void uart_test(unsigned char length);
sbit SDA=P1^6;
sbit SCL=P1^7;
unsigned char Time_Buf_ASCII[100] = {0};//���������ASCII����ַ���
unsigned char TX_buf[10];


uchar g8563_Store[6]; /*ʱ�佻����,ȫ�ֱ�������*/
uchar code c8563_Store[6]={0x16,0x05,0x25,0x16,0x23,0x00}; /*д��ʱ���ֵ������һ 07:59:00*/
//uchar code c8563_Store[6]={0x01,0x02,0x03,0x04,0x05,0x05};
/********************************************
�ڲ���������ʱ1
********************************************/
void Delay()
{
		_nop_();_nop_(); _nop_();_nop_();_nop_();_nop_();/*���ݾ���Ƶ���ƶ���ʱʱ��*/
}
/********************************************
�ڲ�������I2C��ʼ
********************************************/
void Start()
{ 
		SDA=1;
   		SCL=1;
   		Delay();
   		SDA=0;
   		Delay();
   		SCL=0;
	

}
/********************************************
�ڲ�������I2C����
********************************************/
void Stop()
{
			SDA=0;
   		SCL=0;
	    Delay();
   		SCL=1;
   		Delay();
   		SDA=1;
   		Delay();
}
/********************************************
�ڲ����������ACK ,ÿ���ֽڴ�����ɣ����ack=0,��������ݣ�ack=1;
********************************************/
void WriteACK(uchar ack)
{
		SDA=ack;
   		Delay();
   		SCL=1;
   		Delay();
   		SCL=0;
}
/********************************************
�ڲ��������ȴ�ACK
********************************************/
void WaitACK()
{  
		uchar errtime=20;
   		SDA=1;
   		Delay(); /*��ACK*/
   		SCL=1;
   		Delay();
   		while(SDA)
   		{  
				errtime--;
      			if(!errtime) 
						Stop();
   		}
   		SCL=0;
   		Delay();
}
/********************************************
�ڲ�����.��������ֽ�
���:B=����
********************************************/
void writebyte(uchar wdata)
{
		uchar i;
   		for(i=0;i<8;i++)
   		{
        		if(wdata&0x80) 
						SDA=1;
       			else 
						SDA=0;
       			wdata<<=1;
       			SCL=1;
       			Delay();
       			SCL=0;
   		}
   		WaitACK();     //I2C������ͨѶ���������˳�I2CͨѶ
}
/********************************************
�ڲ�����.��������
����:B
********************************************/
uchar Readbyte()
{
		uchar i,bytedata;
   		SDA=1;
   		for(i=0;i<8;i++)
   		{
      			SCL=1; 
      			bytedata<<=1;
      			bytedata|=SDA;
      			SCL=0;
      			Delay();
   		}
   		return(bytedata);
}
/********************************************
�������->pcf8563
********************************************/
void writeData(uchar address,uchar mdata)
{
		Start();
   		writebyte(0xa2); /*д����*/
		writebyte(address); /*д��ַ*/
	    writebyte(mdata); /*д����*/
		Stop();
}
/********************************************
��������<-pcf8563
********************************************/
/*uchar ReadData(uchar address) //���ֽ�
{  
		uchar rdata;
   		Start();
	    writebyte(0xa2); //д����
	    writebyte(address); //д��ַ
	    Start();
	    writebyte(0xa3); //������
	    rdata=Readbyte();
	    WriteACK(1);
	    Stop();
	    return(rdata);
}	*/
void ReadData1(uchar address,uchar count,uchar * buff) /*���ֽ�*/
{  
		uchar i;
   		Start();
   		writebyte(0xa2); /*д����*/
	    writebyte(address); /*д��ַ*/
	    Start();
	    writebyte(0xa3); /*������*/
	    for(i=0;i<count;i++)
	    {
	    		buff[i]=Readbyte();
	       		if(i<count-1) 
						WriteACK(0);
	    }
	    WriteACK(1);
        Stop();
}  
/********************************************
�ڲ�����,����ʱ�䵽�ڲ�������
********************************************/
void P8563_Read()
{   
		uchar time[7];
		int real_int_time[7] = 0;
	  
	  int len = 0;
	
    ReadData1(0x02,0x07,time);
		/*����ԭʼ��BCD���ʾ��ʱ����Ϣ*/
	  g8563_Store[0]=time[0]&0x7f; /*�� */
	  g8563_Store[1]=time[1]&0x7f; /*�� */
	  g8563_Store[2]=time[2]&0x3f; /*Сʱ */
		g8563_Store[3]=time[3]&0x3f; /*�� */
	  g8563_Store[4]=time[5]&0x1f; /*�� */
		g8563_Store[5]=time[6]; /*��  */
    /*ת�浽buf��*/
	  TX_buf[0] = g8563_Store[5];
		TX_buf[1] = g8563_Store[4];
		TX_buf[2] = g8563_Store[3];
	  TX_buf[3] = g8563_Store[2];	
		TX_buf[4] = g8563_Store[1];	
		TX_buf[5] = g8563_Store[0];	
		/*��BCD����ʽ��ʮ����������ת��Ϊʮ��������*/
		real_int_time[0] = (TX_buf[0] % 16) + ((TX_buf[0] / 16)*10);
		real_int_time[1] = (TX_buf[1] % 16) + ((TX_buf[1] / 16)*10);
		real_int_time[2] = (TX_buf[2] % 16) + ((TX_buf[2] / 16)*10);
		real_int_time[3] = (TX_buf[3] % 16) + ((TX_buf[3] / 16)*10);
		real_int_time[4] = (TX_buf[4] % 16) + ((TX_buf[4] / 16)*10);
		real_int_time[5] = (TX_buf[5] % 16) + ((TX_buf[5] / 16)*10);
//        uart_test(6);  	
    /*ƴ�ӳ��ַ���������*/
    sprintf(Time_Buf_ASCII,"20%02d\\%02d\\%02d %02d:%02d:%02d",
		real_int_time[0] ,real_int_time[1],real_int_time[2],real_int_time[3],real_int_time[4],real_int_time[5] ,strlen(Time_Buf_ASCII)    );
		/*���Դ�ӡ*/
		//uart_sendstr(Time_Buf_ASCII);
		
		//��ղ���Ҫ������֮����ʵҲ����Ҫ���������ȶ�һ��
		//memset(Time_Buf_ASCII,0,sizeof(Time_Buf_ASCII));
	
}
/********************************************
����ʱ�䵽�ڲ�������----�ⲿ���� 
********************************************/
void P8563_gettime()
{
    	P8563_Read();
    	if(g8563_Store[0]==0)
      			P8563_Read(); /*���Ϊ��=0��Ϊ��ֹʱ��仯���ٶ�һ��*/
}	
/********************************************
дʱ���޸�ֵ
********************************************/
void P8563_settime()
{
	    //uchar i;
	  writeData(8,g8563_Store[0]); //�� 
	 	writeData(7,g8563_Store[1]); //�� 
		writeData(5,g8563_Store[2]); //�� 
		writeData(4,g8563_Store[3]); //ʱ 
	  writeData(3,g8563_Store[4]); //��  
		writeData(2,g8563_Store[5]); //�� 
}
/********************************************
P8563�ĳ�ʼ��-----�ⲿ����
********************************************/
void P8563_init()
{
    uchar i;
	  
    for(i=0;i<=5;i++) 
	      g8563_Store[i]=c8563_Store[i]; /*��ʼ��ʱ��*/
    P8563_settime();  
}

