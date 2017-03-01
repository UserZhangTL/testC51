/*
时间：2015/11/29
功能：实现发送数据功能,主机上
作者：张天乐
*/
#include "AllFunc.h"
#include "NRF24L01_Receiver.h"


sbit CE   = P1^0;  //RX/TX模式选择端
sbit SCK  = P1^1;	//SPI时钟端
sbit MISO = P1^2;	//SPI主机输出从机输出端
sbit CSN  = P1^3; //SPI片选端//就是SS，低电平有效
sbit MOSI = P1^4;	//SPI主机输出从机输入端
sbit IRQ  = P1^5; //可屏蔽中断端，低电平使能


unsigned char  code TxAddr[]={0x34,0x43,0x10,0x10,0x01};//发送地址
unsigned char  code RxAddr[]={0x34,0x43,0x10,0x10,0x01};//接收地址，保持和发送地址相同

/*状态寄存器的4 5 6位，详见中文手册P17*/
unsigned char bdata sta;   //状态标志，bdata是允许对一个八位的数据，如sta的每一位进行操作，相当如IO口
sbit RX_DR=sta^6;//接收数据中断，当接收到有效数据后置一，写1清除中断
sbit TX_DS=sta^5;//发送数据中断，自动工作方式下，只有当收到应答信号以后此位才为1，写1清除中断
sbit MAX_RT=sta^4;//达到最多次重发中断，如果它的中断产生，则需要清除后，系统才能进行通讯


//对于NRF24L01的初始化操作，使其初始时处于待机模式1状态
void NRF24L01_Init()
{
	  delay_ms(50);
	  CE = 0;//设置为待机模式，未开始传输数据
		CSN = 1;//未使能模块
	  SCK = 0;//将时钟拉低
	  IRQ = 1;//中断位屏蔽
}

/*主机向模块NRF24L01发送数据*/
//使用SPI总线进行的数据交换，MOSI中放主机输出的位，MISO中放从机输入到主机的位
unsigned char SPI_ExchangeData(unsigned char bufdata)
{
	  unsigned char i = 0;
	  //SCK = 0;             //进来首先把SCK置为0，满足第一次跳变
    for(i = 0;i < 8;i ++)//从高位开始传输！！
		{
			  //判断最高位为零为一
		    MOSI = bufdata & 0x80;
				bufdata <<= 1;//bufdata空出最低位
				SCK = 1;//高电平到来，MOSI中数据被送出
			  bufdata |= MISO;				
				SCK = 0;//下降沿到来，MISO中数据为从机发送过来的
		}
		return bufdata;//将从机发来的八位数据返回
}

/*
思想：这个函数用来读取NRF中某个寄存器中的一个字节数据，
而读取的话，有一个SPI指令，专门读取寄存器的，格式Read_cmd = 0x00 + 需要读取的寄存器地址
详见中文手册P13
*/
//SPI总线，读NRF中寄存器中一个字节的数据
unsigned char SPI_Read_NRF(unsigned char Read_cmd)//该 Read_cmd = 0x00 + 需要读取的寄存器地址
{
	  unsigned char readData;
	  CSN = 0;//模块使能有效
	  SPI_ExchangeData(Read_cmd);//写入SPI命令，格式 000A AAAA，即把0x00 + Addr写入，说明：接着要读取这个寄存器中内容了
	  readData = SPI_ExchangeData(0x00);//开始交换数据，实际是单向读取寄存器数据，因为我只需要读取你的值，所以我发0x00也就可以了
	  CSN = 1;
	
	  return readData;
}


//SPI总线，写数据到NRF的寄存器中，数据为wr_value，长度为一字节
unsigned char SPI_Write_NRF(unsigned char Write_cmd,unsigned char wr_value)
{
	  unsigned char statue;
	  CSN = 0;
	  statue = SPI_ExchangeData(Write_cmd);//首先把写命令通过SPI写入到NRF中
	  SPI_ExchangeData(wr_value);//紧接着就写入数据
	  CSN = 1;
	
	  return statue; //返回状态字，用于判断是否正确传输了*有待验证*
}



//使用SPI，对RXFIFO寄存器的值进行连续的读取，读取完以后，该寄存器内容被清空
unsigned char SPI_Read_RxFIFO(unsigned char Read_cmd,unsigned char *RxData,unsigned char DataLen)
{
	  unsigned char status;
	  unsigned char i = 0;
	  CSN = 0;
	  status = SPI_ExchangeData(Read_cmd);
	  for(i = 0;i < DataLen;i ++)//对某个寄存器进行连续的读取，读取长度为DataLen，读取到的数据在RxData中
		{
				RxData[i] = SPI_ExchangeData(0x00);
		}
		CSN = 1;
		
		return status;
}

//使用SPI，对TXFIFO寄存器进行写操作，写入数据为TxData中数据，长度为DataLen
unsigned char SPI_Write_TxFIFO(unsigned char Write_cmd,unsigned char *TxData,unsigned char DataLen)
{
	  unsigned char status;
	  unsigned char i = 0;
	  CSN = 0;
	  status = SPI_ExchangeData(Write_cmd);
	

	  for(i = 0;i < DataLen;i ++)
		{
			  SPI_ExchangeData(*TxData ++);
		}
		CSN = 1;
		
		return status;

}

/*
设置NRF的工作方式，设置为发送方式
*/
void SetNRF_SendDataMode(unsigned char *TxData)
{
	  CE = 0;//首先使模块进入待机模式，这样可以设置NRF的工作状态
	  /*首先配置发送地址，需要写命令到发送地址寄存器中，然后写入地址，再写入地址的宽度
	  参数一:写寄存器，写入到发送地址寄存器，表示要对其操作
	  参数二：写入地址，收发地址相同
	  参数三：写入的地址长度，为5个字节长度
	  */
	  SPI_Write_TxFIFO(W_REGISTER + TX_ADDR, TxAddr, TX_ADDR_WITDH);
	  /*NRF在
	  然后写入需要使用0通道作为接收端的应答通道，接收端在接收到数据后，记录该地址，然后通过0通道回复应答，
	  所以，0通道的地址需要和发送端的发送地址相同，这样才能保证接收到接收方发来的应答信号
	  参数一：写寄存器，写入到通道0寄存器中
	  参数二：写入通道0的地址，地址和发送端地址相同，保证接收到应答信号
	  参数三：地址长度，为5个字节长度
	  */  
	  SPI_Write_TxFIFO(W_REGISTER + RX_ADDR_P0, TxAddr, TX_ADDR_WITDH);
	  /*紧接着可以开始写入数据了
	  参数一：还是写寄存器，写入到写寄存器中，表示开始写数据了
	  参数二：需要写入的数据
	  参数三：需要写入的数据长度，长度为4个字节
	  */
	  SPI_Write_TxFIFO(W_TX_PAYLOAD,TxData,TX_DATA_WITDH);
		/*接下来需要配置各个寄存器，比如应答方式等，主要配置头开始的六个寄存器*/
		//通过写一个字节就可以设置各个寄存器，所以不使用SPI_Write_TxFIFO（）；函数
		SPI_Write_NRF(W_REGISTER + EN_AA, 0x01);//使能通道0，通道0自动应答允许，要是不需要自动应答，则置0x00
		SPI_Write_NRF(W_REGISTER + EN_RXADDR, 0x01);//允许通道0
		
		//SPI_Write_NRF(W_REGISTER + SETUP_AW, TX_DATA_WITDH);
		
		SPI_Write_NRF(W_REGISTER + SETUP_RETR, 0x1f);//设置自动重发，,自动重发十次
		SPI_Write_NRF(W_REGISTER + RF_CH, 0x00);//设置通道频率是0！！！
	  SPI_Write_NRF(W_REGISTER + RF_SETUP, 0x07);//射频寄存器使用000 0 11 1，采用1Mbps  0dBm,
		SPI_Write_NRF(W_REGISTER + CONFIG, 0x0e);//设置CRC检验使能有效，十六位iCRC校验，上电，发射模式
		
		CE = 1;
		
		delay_us(10);//必须延迟10us以上，现在这里延时了15us
		
}

//接收数据函数，因为是接收，所以也不需要任何参数

void SetNRF_ReceiveDataMode()
{
	  CE = 0;
	  //参数一：写入命令到通道0接收寄存器中，使用的地址和发送地址相同，才能保证正确的传输
	  SPI_Write_TxFIFO(W_REGISTER + RX_ADDR_P0, TxAddr, TX_ADDR_WITDH);//写给NRF告诉它，现在准备接受通道0的数据
	
	  /*接下来设置各个寄存器，使NRF工作在接受模式下！*/
	  SPI_Write_NRF(W_REGISTER + EN_AA, 0x01);//使能通道0，通道0自动应答允许，要是不需要自动应答，则置0x00
		SPI_Write_NRF(W_REGISTER + EN_RXADDR, 0x01);//允许通道0
		//地址宽度，不知要不要写，感觉应该是要写的
		//SPI_Write_NRF(W_REGISTER + SETUP_AW, 0x03);//设置地址宽度，000000XX，因为使用五个地址宽度，所以XX = 11，即0x03
		SPI_Write_NRF(W_REGISTER + RF_CH, 0x00);//设置通道频率是0！！！收发双方必须保持一致
		SPI_Write_NRF(W_REGISTER + RX_PW_P0,RX_DATA_WITDH);//设置接收通道0的数据宽度，宽度需要和发射端一样宽??
	  SPI_Write_NRF(W_REGISTER + RF_SETUP, 0x07);//射频寄存器使用000 0 11 1，采用1Mbps  0dBm,
	  SPI_Write_NRF(W_REGISTER + CONFIG, 0x0f);//设置CRC检验使能有效，十六位iCRC校验，上电，接收模式
	  CE = 1;
	
	  delay_us(50);//必须延迟10us以上，现在这里延时了15us
}


/*检测应答，这是对于发送方而言，因为发送方需要检测是否接受方接收到了有效数据*/
//如果返回0，则说明有应答
unsigned char CheckAck()
{
	  /*测试阶段添加在发送函数中的内容，判断是否发送成功，单纯的调试发送端的测试部分，整合以后最好删除，用Check函数*/
		//新添加，表示发送成功
    sta = SPI_Read_NRF(R_REGISTER + STATUS);
	
	  //由于发送完数据或者自动重发次数达到最大值时，对应位会被置一
	  if((TX_DS == 1) || (MAX_RT == 1))
		{
			  //uart_sendbyte(sta);
			  //需要进行清空中断位，手册中说，置一清除中断，所以可以全部置一，用来清空这三位 
			  SPI_Write_NRF(W_REGISTER + STATUS, 0xff);
				CSN = 0;//片选芯片
			  //清除TXFIFO寄存器,保证数据的正确性
			  SPI_ExchangeData(FLUSH_TX);
			  CSN = 1;//片选使能无效
			  return 0;
		}
		else
		{
			  return 1;
		}
}


/*接收模式下，判断是否读取到了数据，通过Flag判断，RecFlag = 1，说明收到了数据，数据将会保存在RecData数组中*/
unsigned char NRF_RecData(unsigned char *RecData)
{
	  unsigned char RecFlag = 0;
	  //读取状态寄存器，判断是否接受到了数据，接收到了的话，会把RX_DR置一
	  sta = SPI_Read_NRF(R_REGISTER + STATUS);
	
	  /*test*/
	  //uart_sendbyte(sta);
	
	
	  if(RX_DR == 1)//如果产生了接收终端标志位，则说明接收成功
		{

			  //这一段是参考程序中所写，好像有误
			  CE = 0;
			  //参数一：函数中需要首先被写入的命令，准备读取数据！
			  //参数二：承接数据的数组
			  //参数三：数组的宽度，也就是数据的宽度是五个字节
			  SPI_Read_RxFIFO(R_RX_PAYLOAD,RecData,RX_ADDR_WITDH);
			  RecFlag = 1;	 
				
			  CE = 1;
		}
		//不论是否收到有效数据，都要在读取后，清空中断位，以便下一次的接收
		//参数一：清除RXFIFO寄存器命令
		SPI_Write_NRF(FLUSH_RX, 0xff);
		

		return RecFlag;
}




