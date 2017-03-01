#include "AllFunc.h"


extern uchar TX_buf[10];
extern void uart_test(unsigned char length);
sbit SDA=P1^6;
sbit SCL=P1^7;
unsigned char Time_Buf_ASCII[100] = {0};//用于输出纯ASCII码的字符串
unsigned char TX_buf[10];


uchar g8563_Store[6]; /*时间交换区,全局变量声明*/
uchar code c8563_Store[6]={0x16,0x05,0x25,0x16,0x23,0x00}; /*写入时间初值：星期一 07:59:00*/
//uchar code c8563_Store[6]={0x01,0x02,0x03,0x04,0x05,0x05};
/********************************************
内部函数，延时1
********************************************/
void Delay()
{
		_nop_();_nop_(); _nop_();_nop_();_nop_();_nop_();/*根据晶振频率制定延时时间*/
}
/********************************************
内部函数，I2C开始
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
内部函数，I2C结束
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
内部函数，输出ACK ,每个字节传输完成，输出ack=0,结束读书据，ack=1;
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
内部函数，等待ACK
********************************************/
void WaitACK()
{  
		uchar errtime=20;
   		SDA=1;
   		Delay(); /*读ACK*/
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
内部函数.输出数据字节
入口:B=数据
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
   		WaitACK();     //I2C器件或通讯出错，将会退出I2C通讯
}
/********************************************
内部函数.输入数据
出口:B
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
输出数据->pcf8563
********************************************/
void writeData(uchar address,uchar mdata)
{
		Start();
   		writebyte(0xa2); /*写命令*/
		writebyte(address); /*写地址*/
	    writebyte(mdata); /*写数据*/
		Stop();
}
/********************************************
输入数据<-pcf8563
********************************************/
/*uchar ReadData(uchar address) //单字节
{  
		uchar rdata;
   		Start();
	    writebyte(0xa2); //写命令
	    writebyte(address); //写地址
	    Start();
	    writebyte(0xa3); //读命令
	    rdata=Readbyte();
	    WriteACK(1);
	    Stop();
	    return(rdata);
}	*/
void ReadData1(uchar address,uchar count,uchar * buff) /*多字节*/
{  
		uchar i;
   		Start();
   		writebyte(0xa2); /*写命令*/
	    writebyte(address); /*写地址*/
	    Start();
	    writebyte(0xa3); /*读命令*/
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
内部函数,读入时间到内部缓冲区
********************************************/
void P8563_Read()
{   
		uchar time[7];
		int real_int_time[7] = 0;
	  
	  int len = 0;
	
    ReadData1(0x02,0x07,time);
		/*读出原始的BCD码表示的时间信息*/
	  g8563_Store[0]=time[0]&0x7f; /*秒 */
	  g8563_Store[1]=time[1]&0x7f; /*分 */
	  g8563_Store[2]=time[2]&0x3f; /*小时 */
		g8563_Store[3]=time[3]&0x3f; /*日 */
	  g8563_Store[4]=time[5]&0x1f; /*月 */
		g8563_Store[5]=time[6]; /*年  */
    /*转存到buf中*/
	  TX_buf[0] = g8563_Store[5];
		TX_buf[1] = g8563_Store[4];
		TX_buf[2] = g8563_Store[3];
	  TX_buf[3] = g8563_Store[2];	
		TX_buf[4] = g8563_Store[1];	
		TX_buf[5] = g8563_Store[0];	
		/*将BCD码形式的十六进制数据转化为十进制数据*/
		real_int_time[0] = (TX_buf[0] % 16) + ((TX_buf[0] / 16)*10);
		real_int_time[1] = (TX_buf[1] % 16) + ((TX_buf[1] / 16)*10);
		real_int_time[2] = (TX_buf[2] % 16) + ((TX_buf[2] / 16)*10);
		real_int_time[3] = (TX_buf[3] % 16) + ((TX_buf[3] / 16)*10);
		real_int_time[4] = (TX_buf[4] % 16) + ((TX_buf[4] / 16)*10);
		real_int_time[5] = (TX_buf[5] % 16) + ((TX_buf[5] / 16)*10);
//        uart_test(6);  	
    /*拼接成字符串，待用*/
    sprintf(Time_Buf_ASCII,"20%02d\\%02d\\%02d %02d:%02d:%02d",
		real_int_time[0] ,real_int_time[1],real_int_time[2],real_int_time[3],real_int_time[4],real_int_time[5] ,strlen(Time_Buf_ASCII)    );
		/*测试打印*/
		//uart_sendstr(Time_Buf_ASCII);
		
		//清空操作要在用完之后，其实也不需要，反正长度都一样
		//memset(Time_Buf_ASCII,0,sizeof(Time_Buf_ASCII));
	
}
/********************************************
读入时间到内部缓冲区----外部调用 
********************************************/
void P8563_gettime()
{
    	P8563_Read();
    	if(g8563_Store[0]==0)
      			P8563_Read(); /*如果为秒=0，为防止时间变化，再读一次*/
}	
/********************************************
写时间修改值
********************************************/
void P8563_settime()
{
	    //uchar i;
	  writeData(8,g8563_Store[0]); //年 
	 	writeData(7,g8563_Store[1]); //月 
		writeData(5,g8563_Store[2]); //日 
		writeData(4,g8563_Store[3]); //时 
	  writeData(3,g8563_Store[4]); //分  
		writeData(2,g8563_Store[5]); //秒 
}
/********************************************
P8563的初始化-----外部调用
********************************************/
void P8563_init()
{
    uchar i;
	  
    for(i=0;i<=5;i++) 
	      g8563_Store[i]=c8563_Store[i]; /*初始化时间*/
    P8563_settime();  
}

