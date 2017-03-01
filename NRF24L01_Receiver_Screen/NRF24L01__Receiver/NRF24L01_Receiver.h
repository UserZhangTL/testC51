#ifndef __NRF_H__
#define __NRF_H__

//���ڽ��ܵĺ꣬�ݶ�����ָ��
#define CMD1 1
#define CMD2 2
#define CMD3 3
#define CMD4 4
#define NUL  5

#define FAIL 0

//��ַ�����ݵĿ�ȣ�ʹ�õ�ַ����Ϊ5���ֽڣ����ݳ���Ϊ4���ֽ�
#define TX_ADDR_WITDH 5	//���͵�ַ�������Ϊ5���ֽ�
#define RX_ADDR_WITDH 5	//���յ�ַ�������Ϊ5���ֽ�
#define TX_DATA_WITDH 4//�������ݿ��4���ֽ�,32λ
#define RX_DATA_WITDH 4//�������ݿ��4���ֽڣ�32λ

//SPI��ָ���ʽ,�������˵����P13
#define  R_REGISTER      0x00//��ȡ���üĴ��� 000A AAAA,����A AAAAָ���Ĵ�����ַ
#define  W_REGISTER      0x20//д���üĴ���   001A AAAA,����A AAAAָ���Ĵ�����ַ
#define  R_RX_PAYLOAD 	 0x61//��ȡRX��Ч����
#define  W_TX_PAYLOAD	   0xa0//дTX��Ч����
#define  FLUSH_TX		     0xe1//���TXFIFO�Ĵ���
#define  FLUSH_RX		     0xe2//���RXFIFO�Ĵ���
#define  REUSE_TX_PL     0xe3//����ʹ����һ����Ч����
#define  NOP             0xff//�ղ���



//ģ���и����Ĵ�����ַ,���ڱ���Ҫ��ʱ�򣬱�SPIָ������ϣ���NRF24L01����˵����P15
#define  CONFIG          0x00//���üĴ���
#define  EN_AA			     0x01//ʹ���Զ�Ӧ��
#define  EN_RXADDR       0x02//����ͨ��ʹ��0-5��ͨ��
#define  SETUP_AW        0x03//��������ͨ����ַ���3-5
#define  SETUP_RETR      0x04//�����Զ��ط�
#define  RF_CH           0x05//��Ƶͨ������
#define  RF_SETUP        0x06//��Ƶ�Ĵ���
#define  STATUS          0x07//״̬�Ĵ���
#define  OBSERVE_TX      0x08//���ͼ��Ĵ���
#define  CD              0x09//�ز�
#define  RX_ADDR_P0      0x0a//����ͨ��0���յ�ַ
#define  RX_ADDR_P1      0x0b//����ͨ��1���յ�ַ
#define  RX_ADDR_P2      0x0c//����ͨ��2���յ�ַ
#define  RX_ADDR_P3      0x0d//����ͨ��3���յ�ַ
#define  RX_ADDR_P4      0x0e//����ͨ��4���յ�ַ
#define  RX_ADDR_P5      0x0f//����ͨ��5���յ�ַ
#define  TX_ADDR         0x10//���͵�ַ�Ĵ���
#define  RX_PW_P0        0x11//P0ͨ�����ݿ������
#define  RX_PW_P1        0x12//P1ͨ�����ݿ������
#define  RX_PW_P2        0x13//P2ͨ�����ݿ������
#define  RX_PW_P3        0x14//P3ͨ�����ݿ������
#define  RX_PW_P4        0x15//P4ͨ�����ݿ������
#define  RX_PW_P5        0x16//P5ͨ�����ݿ������
#define  FIFO_STATUS     0x17//FIFO״̬�Ĵ���



extern void NRF24L01_Init();
extern unsigned char SPI_ExchangeData(unsigned char bufdata);
extern unsigned char SPI_Read_NRF(unsigned char Read_cmd);
extern unsigned char SPI_Write_NRF(unsigned char Write_cmd,unsigned char wr_value);
extern unsigned char SPI_Read_RxFIFO(unsigned char Read_cmd,unsigned char *RxData,unsigned char DataLen);
extern unsigned char SPI_Write_TxFIFO(unsigned char Write_cmd,unsigned char *TxData,unsigned char DataLen);
extern void SetNRF_SendDataMode(unsigned char *TxData);
extern void SetNRF_ReceiveDataMode();
extern unsigned char CheckAck();
extern unsigned char NRF_RecData(unsigned char *RecData);
extern unsigned char Get_Cmd(unsigned char *RecData);
extern unsigned char Perform_cmd(const unsigned char *RecData);

#endif