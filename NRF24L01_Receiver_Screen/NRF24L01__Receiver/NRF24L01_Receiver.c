/*
ʱ�䣺2015/11/29
���ܣ�ʵ�ַ������ݹ���,������
���ߣ�������
*/
#include "AllFunc.h"
#include "NRF24L01_Receiver.h"


sbit CE   = P1^0;  //RX/TXģʽѡ���
sbit SCK  = P1^1;	//SPIʱ�Ӷ�
sbit MISO = P1^2;	//SPI��������ӻ������
sbit CSN  = P1^3; //SPIƬѡ��//����SS���͵�ƽ��Ч
sbit MOSI = P1^4;	//SPI��������ӻ������
sbit IRQ  = P1^5; //�������ж϶ˣ��͵�ƽʹ��


unsigned char  code TxAddr[]={0x34,0x43,0x10,0x10,0x01};//���͵�ַ
unsigned char  code RxAddr[]={0x34,0x43,0x10,0x10,0x01};//���յ�ַ�����ֺͷ��͵�ַ��ͬ

/*״̬�Ĵ�����4 5 6λ����������ֲ�P17*/
unsigned char bdata sta;   //״̬��־��bdata�������һ����λ�����ݣ���sta��ÿһλ���в������൱��IO��
sbit RX_DR=sta^6;//���������жϣ������յ���Ч���ݺ���һ��д1����ж�
sbit TX_DS=sta^5;//���������жϣ��Զ�������ʽ�£�ֻ�е��յ�Ӧ���ź��Ժ��λ��Ϊ1��д1����ж�
sbit MAX_RT=sta^4;//�ﵽ�����ط��жϣ���������жϲ���������Ҫ�����ϵͳ���ܽ���ͨѶ


//����NRF24L01�ĳ�ʼ��������ʹ���ʼʱ���ڴ���ģʽ1״̬
void NRF24L01_Init()
{
	  delay_ms(50);
	  CE = 0;//����Ϊ����ģʽ��δ��ʼ��������
		CSN = 1;//δʹ��ģ��
	  SCK = 0;//��ʱ������
	  IRQ = 1;//�ж�λ����
}

/*������ģ��NRF24L01��������*/
//ʹ��SPI���߽��е����ݽ�����MOSI�з����������λ��MISO�зŴӻ����뵽������λ
unsigned char SPI_ExchangeData(unsigned char bufdata)
{
	  unsigned char i = 0;
	  //SCK = 0;             //�������Ȱ�SCK��Ϊ0�������һ������
    for(i = 0;i < 8;i ++)//�Ӹ�λ��ʼ���䣡��
		{
			  //�ж����λΪ��Ϊһ
		    MOSI = bufdata & 0x80;
				bufdata <<= 1;//bufdata�ճ����λ
				SCK = 1;//�ߵ�ƽ������MOSI�����ݱ��ͳ�
			  bufdata |= MISO;				
				SCK = 0;//�½��ص�����MISO������Ϊ�ӻ����͹�����
		}
		return bufdata;//���ӻ������İ�λ���ݷ���
}

/*
˼�룺�������������ȡNRF��ĳ���Ĵ����е�һ���ֽ����ݣ�
����ȡ�Ļ�����һ��SPIָ�ר�Ŷ�ȡ�Ĵ����ģ���ʽRead_cmd = 0x00 + ��Ҫ��ȡ�ļĴ�����ַ
��������ֲ�P13
*/
//SPI���ߣ���NRF�мĴ�����һ���ֽڵ�����
unsigned char SPI_Read_NRF(unsigned char Read_cmd)//�� Read_cmd = 0x00 + ��Ҫ��ȡ�ļĴ�����ַ
{
	  unsigned char readData;
	  CSN = 0;//ģ��ʹ����Ч
	  SPI_ExchangeData(Read_cmd);//д��SPI�����ʽ 000A AAAA������0x00 + Addrд�룬˵��������Ҫ��ȡ����Ĵ�����������
	  readData = SPI_ExchangeData(0x00);//��ʼ�������ݣ�ʵ���ǵ����ȡ�Ĵ������ݣ���Ϊ��ֻ��Ҫ��ȡ���ֵ�������ҷ�0x00Ҳ�Ϳ�����
	  CSN = 1;
	
	  return readData;
}


//SPI���ߣ�д���ݵ�NRF�ļĴ����У�����Ϊwr_value������Ϊһ�ֽ�
unsigned char SPI_Write_NRF(unsigned char Write_cmd,unsigned char wr_value)
{
	  unsigned char statue;
	  CSN = 0;
	  statue = SPI_ExchangeData(Write_cmd);//���Ȱ�д����ͨ��SPIд�뵽NRF��
	  SPI_ExchangeData(wr_value);//�����ž�д������
	  CSN = 1;
	
	  return statue; //����״̬�֣������ж��Ƿ���ȷ������*�д���֤*
}



//ʹ��SPI����RXFIFO�Ĵ�����ֵ���������Ķ�ȡ����ȡ���Ժ󣬸üĴ������ݱ����
unsigned char SPI_Read_RxFIFO(unsigned char Read_cmd,unsigned char *RxData,unsigned char DataLen)
{
	  unsigned char status;
	  unsigned char i = 0;
	  CSN = 0;
	  status = SPI_ExchangeData(Read_cmd);
	  for(i = 0;i < DataLen;i ++)//��ĳ���Ĵ������������Ķ�ȡ����ȡ����ΪDataLen����ȡ����������RxData��
		{
				RxData[i] = SPI_ExchangeData(0x00);
		}
		CSN = 1;
		
		return status;
}

//ʹ��SPI����TXFIFO�Ĵ�������д������д������ΪTxData�����ݣ�����ΪDataLen
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
����NRF�Ĺ�����ʽ������Ϊ���ͷ�ʽ
*/
void SetNRF_SendDataMode(unsigned char *TxData)
{
	  CE = 0;//����ʹģ��������ģʽ��������������NRF�Ĺ���״̬
	  /*�������÷��͵�ַ����Ҫд������͵�ַ�Ĵ����У�Ȼ��д���ַ����д���ַ�Ŀ��
	  ����һ:д�Ĵ�����д�뵽���͵�ַ�Ĵ�������ʾҪ�������
	  ��������д���ַ���շ���ַ��ͬ
	  ��������д��ĵ�ַ���ȣ�Ϊ5���ֽڳ���
	  */
	  SPI_Write_TxFIFO(W_REGISTER + TX_ADDR, TxAddr, TX_ADDR_WITDH);
	  /*NRF��
	  Ȼ��д����Ҫʹ��0ͨ����Ϊ���ն˵�Ӧ��ͨ�������ն��ڽ��յ����ݺ󣬼�¼�õ�ַ��Ȼ��ͨ��0ͨ���ظ�Ӧ��
	  ���ԣ�0ͨ���ĵ�ַ��Ҫ�ͷ��Ͷ˵ķ��͵�ַ��ͬ���������ܱ�֤���յ����շ�������Ӧ���ź�
	  ����һ��д�Ĵ�����д�뵽ͨ��0�Ĵ�����
	  ��������д��ͨ��0�ĵ�ַ����ַ�ͷ��Ͷ˵�ַ��ͬ����֤���յ�Ӧ���ź�
	  ����������ַ���ȣ�Ϊ5���ֽڳ���
	  */  
	  SPI_Write_TxFIFO(W_REGISTER + RX_ADDR_P0, TxAddr, TX_ADDR_WITDH);
	  /*�����ſ��Կ�ʼд��������
	  ����һ������д�Ĵ�����д�뵽д�Ĵ����У���ʾ��ʼд������
	  ����������Ҫд�������
	  ����������Ҫд������ݳ��ȣ�����Ϊ4���ֽ�
	  */
	  SPI_Write_TxFIFO(W_TX_PAYLOAD,TxData,TX_DATA_WITDH);
		/*��������Ҫ���ø����Ĵ���������Ӧ��ʽ�ȣ���Ҫ����ͷ��ʼ�������Ĵ���*/
		//ͨ��дһ���ֽھͿ������ø����Ĵ��������Բ�ʹ��SPI_Write_TxFIFO����������
		SPI_Write_NRF(W_REGISTER + EN_AA, 0x01);//ʹ��ͨ��0��ͨ��0�Զ�Ӧ������Ҫ�ǲ���Ҫ�Զ�Ӧ������0x00
		SPI_Write_NRF(W_REGISTER + EN_RXADDR, 0x01);//����ͨ��0
		
		//SPI_Write_NRF(W_REGISTER + SETUP_AW, TX_DATA_WITDH);
		
		SPI_Write_NRF(W_REGISTER + SETUP_RETR, 0x1f);//�����Զ��ط���,�Զ��ط�ʮ��
		SPI_Write_NRF(W_REGISTER + RF_CH, 0x00);//����ͨ��Ƶ����0������
	  SPI_Write_NRF(W_REGISTER + RF_SETUP, 0x07);//��Ƶ�Ĵ���ʹ��000 0 11 1������1Mbps  0dBm,
		SPI_Write_NRF(W_REGISTER + CONFIG, 0x0e);//����CRC����ʹ����Ч��ʮ��λiCRCУ�飬�ϵ磬����ģʽ
		
		CE = 1;
		
		delay_us(10);//�����ӳ�10us���ϣ�����������ʱ��15us
		
}

//�������ݺ�������Ϊ�ǽ��գ�����Ҳ����Ҫ�κβ���

void SetNRF_ReceiveDataMode()
{
	  CE = 0;
	  //����һ��д�����ͨ��0���ռĴ����У�ʹ�õĵ�ַ�ͷ��͵�ַ��ͬ�����ܱ�֤��ȷ�Ĵ���
	  SPI_Write_TxFIFO(W_REGISTER + RX_ADDR_P0, TxAddr, TX_ADDR_WITDH);//д��NRF������������׼������ͨ��0������
	
	  /*���������ø����Ĵ�����ʹNRF�����ڽ���ģʽ�£�*/
	  SPI_Write_NRF(W_REGISTER + EN_AA, 0x01);//ʹ��ͨ��0��ͨ��0�Զ�Ӧ������Ҫ�ǲ���Ҫ�Զ�Ӧ������0x00
		SPI_Write_NRF(W_REGISTER + EN_RXADDR, 0x01);//����ͨ��0
		//��ַ��ȣ���֪Ҫ��Ҫд���о�Ӧ����Ҫд��
		//SPI_Write_NRF(W_REGISTER + SETUP_AW, 0x03);//���õ�ַ��ȣ�000000XX����Ϊʹ�������ַ��ȣ�����XX = 11����0x03
		SPI_Write_NRF(W_REGISTER + RF_CH, 0x00);//����ͨ��Ƶ����0�������շ�˫�����뱣��һ��
		SPI_Write_NRF(W_REGISTER + RX_PW_P0,RX_DATA_WITDH);//���ý���ͨ��0�����ݿ�ȣ������Ҫ�ͷ����һ����??
	  SPI_Write_NRF(W_REGISTER + RF_SETUP, 0x07);//��Ƶ�Ĵ���ʹ��000 0 11 1������1Mbps  0dBm,
	  SPI_Write_NRF(W_REGISTER + CONFIG, 0x0f);//����CRC����ʹ����Ч��ʮ��λiCRCУ�飬�ϵ磬����ģʽ
	  CE = 1;
	
	  delay_us(50);//�����ӳ�10us���ϣ�����������ʱ��15us
}


/*���Ӧ�����Ƕ��ڷ��ͷ����ԣ���Ϊ���ͷ���Ҫ����Ƿ���ܷ����յ�����Ч����*/
//�������0����˵����Ӧ��
unsigned char CheckAck()
{
	  /*���Խ׶�����ڷ��ͺ����е����ݣ��ж��Ƿ��ͳɹ��������ĵ��Է��Ͷ˵Ĳ��Բ��֣������Ժ����ɾ������Check����*/
		//����ӣ���ʾ���ͳɹ�
    sta = SPI_Read_NRF(R_REGISTER + STATUS);
	
	  //���ڷ��������ݻ����Զ��ط������ﵽ���ֵʱ����Ӧλ�ᱻ��һ
	  if((TX_DS == 1) || (MAX_RT == 1))
		{
			  //uart_sendbyte(sta);
			  //��Ҫ��������ж�λ���ֲ���˵����һ����жϣ����Կ���ȫ����һ�������������λ 
			  SPI_Write_NRF(W_REGISTER + STATUS, 0xff);
				CSN = 0;//ƬѡоƬ
			  //���TXFIFO�Ĵ���,��֤���ݵ���ȷ��
			  SPI_ExchangeData(FLUSH_TX);
			  CSN = 1;//Ƭѡʹ����Ч
			  return 0;
		}
		else
		{
			  return 1;
		}
}


/*����ģʽ�£��ж��Ƿ��ȡ�������ݣ�ͨ��Flag�жϣ�RecFlag = 1��˵���յ������ݣ����ݽ��ᱣ����RecData������*/
unsigned char NRF_RecData(unsigned char *RecData)
{
	  unsigned char RecFlag = 0;
	  //��ȡ״̬�Ĵ������ж��Ƿ���ܵ������ݣ����յ��˵Ļ������RX_DR��һ
	  sta = SPI_Read_NRF(R_REGISTER + STATUS);
	
	  /*test*/
	  //uart_sendbyte(sta);
	
	
	  if(RX_DR == 1)//��������˽����ն˱�־λ����˵�����ճɹ�
		{

			  //��һ���ǲο���������д����������
			  CE = 0;
			  //����һ����������Ҫ���ȱ�д������׼����ȡ���ݣ�
			  //���������н����ݵ�����
			  //������������Ŀ�ȣ�Ҳ�������ݵĿ��������ֽ�
			  SPI_Read_RxFIFO(R_RX_PAYLOAD,RecData,RX_ADDR_WITDH);
			  RecFlag = 1;	 
				
			  CE = 1;
		}
		//�����Ƿ��յ���Ч���ݣ���Ҫ�ڶ�ȡ������ж�λ���Ա���һ�εĽ���
		//����һ�����RXFIFO�Ĵ�������
		SPI_Write_NRF(FLUSH_RX, 0xff);
		

		return RecFlag;
}




