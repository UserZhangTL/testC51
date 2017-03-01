//#include <reg52.h>
//#include "delay.h"
//#include "uart.h"
//#include <stdio.h>
//#include <string.h>
//#include "zimo.h"//�����ֿ�ͷ�ļ�����ʹ��

#include "AllFunc.h"
#include "zimo.h"

sbit T_STR = P2^6;       //�������ţ����ϱ�ʶλSTR
sbit R1    = P0^4;       //�������ţ����ϱ�ʶR1
sbit T_CLK = P2^5;       //ʱ�����ţ����ϱ�ʶλCLK
sbit OE    = P2^1;       //ʹ�����ţ����ϱ�ʶEN/OE
sbit A_138     = P0^0;//ABCΪ138������ѡ�������϶�
sbit B_138     = P0^1;
sbit C_138     = P0^2;
sbit D_138     = P0^3;//Ƭѡ��Ƭ138֮һ



/*
���ж�ά����ƴ��
��Ϊ������Ҫ��̬���������
����һ��ָ�����飬���Է�128���֣����ڶ�̬��ʾ
*/


unsigned char *OutPutStr[MAX_LEN] = {space};

unsigned char row = 0;//�У�ÿ�ν����һ������ʮ����
//�ڶ�ά�����еڶ�ά�е��ֽ�λ�ã�Ҳ������һ�й�����У�Ҫ��ʾ���İ˸��ֽ�
unsigned char row_loc = 0;
bit FifteenHundred_us_Come = NO;//Ĭ����û�е�ʱ��ģ����˵Ļ�������while���ж�Ȼ�����
bit One_s_Come         = NO;
bit Four_hundred_ms_Come       = NO;//400ms�����ı�־λ��������ʾ��λ��

//����һ��ȫ�ֱ���������������Ҫ��ʾ�������ַ�����������ͨ��������ά�����е�һλ���鳤��
unsigned int char_len = 0;

unsigned char DIS[]={15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};//������

/*��ģ�е�λ��ƫ��*/
unsigned int temp_char_len = 0;//����ѭ��ʱ�����ַ�����������Ϊ��̬������ֻ����һ�Σ��ֲ������ɼ�

/*
��Ϊ��һ��ʼʹ��ָ�������е�����Ԫ�ض�ָ����space�����ԾͲ�����Ҫ�����
ֻ��Ҫ�ı��ַ������ȣ�Ϊ��̬�л�����
�����������ַ�����
*/
void Modify_Str_Len(unsigned int *old_char_len)
{
	  unsigned char i = 0;
	
    if(((*old_char_len) % 4) != 0)//��Ϊ�ĵı������ַ���Ŀ
		{
		    *old_char_len = *old_char_len + (4 - ((*old_char_len) % 4));
		}	
}


/*��ʼ��ָ�����飬��֤����������õ�����*/
void init_OutPutStr()
{
	  unsigned int i = 0;
	  for(i = 0;i < 128;i ++)
	  {
			  OutPutStr[i] = space;
		}
}

void InputByte( unsigned  char DataR1) //дһ���ֽ�
{ 
		unsigned  char i;
		for(i=8; i>0; i--)
		{
			  R1 = !(DataR1&0x80);                   
			  T_CLK = 0;
			  T_CLK = 1;
			  DataR1 = DataR1 << 1; 
		} 
}

void Init_Timer0(void)
{
		 TMOD |= 0x01;			     
		 EA=1;                      //���ж�ʹ��
	   TH0 = (65536 - TIMERVAL) / 256;//200usһ��
	   TL0 = (65536 - TIMERVAL) % 256; 
	
		 ET0=1;                     //��ʱ�ж�ʹ��
		 TR0=1;                     //�򿪶�ʱ������
}

/*
��װһ�㺯������Ϊ�ӿڵ���
����һ�������ά����ָ��
�������������ά�����һά���鳤�ȣ�Ҳ�����ж��ٸ���Ҫ��ʾ
*/
void showLeftRollString(unsigned char *outChar[128],unsigned char char_len)
{
	  //static unsigned int temp_char_len = 0;//����ѭ��ʱ�����ַ�����������Ϊ��̬������ֻ����һ�Σ��ֲ������ɼ�
	
	  unsigned int loc_count = 0;//����ѭ���ı��������ڶ�̬�ҵ��ĸ��ַ��е�ĳһ��
	
		//200us��������ˢ��
		if(FifteenHundred_us_Come == YES)
		{
				FifteenHundred_us_Come = NO;//һ�������ձ�־λ
				
				T_STR=0;       //�����ͷ�
				//���һ�е��ֽ��ڵ�������ĳһ����
				for(loc_count = temp_char_len;loc_count < (    temp_char_len + 4      );loc_count ++) //�ĸ��֣��ַ�������char_len��
				{
						InputByte(outChar[loc_count][row_loc]);//��ʾ��ά�����е�ĳ�����������ֽڣ�����һ������16����
						InputByte(outChar[loc_count][row_loc + 1]);		 
				}
				OE=1;  //��ʱ�ر���Ļ����ֹ���ֹ�Ӱ�����û��ʹ�ø����ſ�����������Ӱ����
				A_138 = DIS[row] & 0x01;
				B_138 = DIS[row] & 0x02;
				C_138 = DIS[row] & 0x04;
				D_138 = DIS[row] & 0x08;
				T_STR=1;      //������Ч����ʱһ�е�������ʾ������
				OE=0;//ʹ��
			
				
				row ++;//����ɨ�裬ɨ��ʮ���н�����Ҳ����˵һ����ʾ�ĸ������Ѿ�����
				if(row == 16)
				{
						row = 0;
						//ʹ�ù���400ms�Ž�����һ�ε��ƶ����Ӿ��ϱȽ�����
						if(Four_hundred_ms_Come == YES)
						{
								Four_hundred_ms_Come = NO;//���һ��  
							
								temp_char_len ++;//˳���������ʾһ���ַ�
								//Ҫ��������ӼӵĻ����ϣ�����4�ʹ���������ֽ�������ô�Ͳ���Ҫ�ټ�����ʾ�ˣ���ͷ��ʼ
								/*�������һ����߼����⣬�����ַ���˳����λ����ʱ�ô�ͷ��ʼ�������⣬���ɶ��߱���*/
								if((temp_char_len + 4) > char_len)
								{
										temp_char_len = 0;
								}
								
						}
				}
			 
			 //һ��һά�����а�����32���ֽ�
				row_loc += 2;
				if(row_loc == 32)
				{
						row_loc = 0;   
				}
		}
		
	
}




/*��̬��ʾ���������ֵı任ѭ��ˢ��*/
void showStaticString(unsigned char *outChar[128],unsigned char real_char_len)
{
	  //static unsigned int temp_char_len = 0;//����ѭ��ʱ�����ַ�����������Ϊ��̬������ֻ����һ�Σ��ֲ������ɼ�
		
	  unsigned int loc_count;//��������

	  
	  if(FifteenHundred_us_Come == YES)
		{
				FifteenHundred_us_Come = NO;//һ�������ձ�־λ
			
				T_STR=0;       //�����ͷ�
				//���һ�е��ֽ��ڵ�������ĳһ���ϣ���Ҫ�ж��Ƿ����ĵı�����Ҫ���ǣ���Ҫ����������ʾ����
			  for(loc_count = temp_char_len;loc_count < ( ((temp_char_len + 4) > real_char_len) ? real_char_len : (temp_char_len + 4)  );loc_count ++) //�ĸ��֣��ַ�������char_len��
				{
						InputByte(outChar[loc_count][row_loc]);//��ʾ��ά�����е�ĳ�����������ֽڣ�����һ������16����
						InputByte(outChar[loc_count][row_loc + 1]);		 
				}
				OE=1;  //��ʱ�ر���Ļ����ֹ���ֹ�Ӱ�����û��ʹ�ø����ſ�����������Ӱ����
				A_138 = DIS[row] & 0x01;
				B_138 = DIS[row] & 0x02;
				C_138 = DIS[row] & 0x04;
				D_138 = DIS[row] & 0x08;
				T_STR=1;      //������Ч����ʱһ�е�������ʾ������
				OE=0;//ʹ��
			
				row ++;//����ɨ��
				if(row == 16)
				{
						row = 0;
					
					  if(One_s_Come == YES)//һ���л�һ��
						{
							  One_s_Come = NO;
							  
                if((temp_char_len + 4) < real_char_len)
								{
									  temp_char_len += 4;
								}
								else
								{
									  temp_char_len = 0;
								}
 							
						}
				}
			 
			 //һ��һά�����а�����32���ֽ�
				row_loc += 2;
				if(row_loc == 32)
				{
						row_loc = 0;   
				}
		}
}


//��װһ�㺯���������ַ���������Ŀ���Ϊ�ַ����飬��������̬Ѱ���ֿ⣬��OutPutStr����ƴ��
/*ֻ�Գ���ASCII����Ч*/
/*ʹ����OutPutStr��char_len����ȫ�ֱ���*/
/*ע�⣺������ַ������ȣ�Ҫ��Ϊ�ַ����飬����ʹ��strlen����Ϊû\0
  ����ʹ��sizeof
*/
void transfer_String_to_OutPutStr(unsigned char *strbuf,unsigned int str_len)
{
	  unsigned int i = 0;
	  /*������б�־λ��*/
    clear_used_extern();
	  /*��ʼ��ָ������*/
    init_OutPutStr();
	  for(i = 0;i < str_len;i ++)
	  {
			  OutPutStr[i] = ASCII[(strbuf[i] - ' ')];//ASCII��ӿո�ʼ�������±�Ϊ����ڿո��ƫ��
		}
		char_len = str_len;
		Modify_Str_Len(&char_len);//�޸���ɣ�ͬʱ����ո���ɣ�������ר�Ÿ���̬�л������
}

/*ÿ�ζ�Ҫ���һ�¸�����־λ��ȫ�ֱ�������֤��һ�εĴ�ͷ��ʼ������ʾ*/
void clear_used_extern()
{
	  row = 0;
	  row_loc = 0;
	  temp_char_len = 0;
}

/*ͨ����ģ�����Ҫ��ʾ�ľ�̬��������*/
void showStaitcInfo(unsigned char Needed_Info[][32],unsigned int temp_len)
{
	  unsigned int i = 0;
	  /*������б�־λ��*/
    clear_used_extern();
	  init_OutPutStr();
	
	
    for(i = 0;i < temp_len;i ++)
	  {
			  OutPutStr[i] = Needed_Info[i];
		}
		
		char_len = temp_len;
		Modify_Str_Len(&char_len);
}


//������ʾʱ�������
/*
ʱ�䳤�Ƚ�Ϊ�̶����Ƚϲ�ĺ��������������޸ģ���Ҫ���ǵ����رȽ϶�
ǰ����ʾʱ�䳤�ȣ���������ʱ�䳤�����߶���Ҫ�ֶ����㳤��

*/
extern unsigned char Time_Buf_ASCII[100];

//ƴװһ��ʵʱʱ�䣬��ʾ����������˵
void spellRTCTime()
{
	  unsigned int i = 0;
	
	  //ǰ����ʾ���ַ�����Ϊ4���ո�Ӻ�������ַ�
    for(i = 0;i < 9;i ++)
	  {
			  OutPutStr[i] = TimeInfo[i];
		}
		for(i = 9;i < 28;i ++)
		{
			  OutPutStr[i] = ASCII[ (Time_Buf_ASCII[i-9] - ' ')];//������
		}
		
}

/*��ʾ��̬���ݣ���ʾ����ȫ������ȫ�ֱ���*/
void showAliveTimeInfo()
{
	  unsigned int i = 0;
	  /*������б�־λ��*/
    clear_used_extern();
	  init_OutPutStr();
	
    spellRTCTime();//ƴװʱ��
		
		char_len = 28;//�̶�������Ⱦ���28
		Modify_Str_Len(&char_len);
}



/*************************��ʪ����ʾ����*********************/

///*��ʪ��ƴװ��ʾ����*/
extern unsigned char tmpInfo_buf[8];
void spellTmpWithChinese()
{
	  unsigned int i = 0;
	   //ǰ����ʾ���ַ�����Ϊ4���ո�Ӻ�������ַ�
    for(i = 0;i < 9;i ++)
	  {
			  OutPutStr[i] = TmpInfo[i];
		}
		for(i = 9;i < 12;i ++)
		{
			  OutPutStr[i] = ASCII[ (tmpInfo_buf[i-9] - ' ')];//������
		}
		OutPutStr[12] = TmpInfo[7];//�������ƴ��һ������
}

/*��ʾ��̬���ݣ���ʾ����ȫ������ȫ�ֱ���*/
void showAliveTmpInfo()
{
	  unsigned int i = 0;
	  /*������б�־λ��*/
    clear_used_extern();
	  init_OutPutStr();
	
    spellTmpWithChinese();//ƴװʱ��
		
		char_len = 13;//�̶�������Ⱦ���12
		Modify_Str_Len(&char_len);
}





/*����ʹ�õ�����ʱ����*/
//void main()
//{
//	  unsigned char i = 0;
//	  unsigned char buf[10] = "abcdefghij";
//	  
//	  uart_init();
//	  delay_ms(100);
//	  //WifiESP8266_Init("Car","01234567","8080");  
//	
//    Init_Timer0(); //��ʼ����ʱ��
//    
//	  
//    transfer_String_to_OutPutStr(buf,sizeof(buf));	
//	
//	
//	  while(1)
//		{
//				//showStaticString(OutPutStr,char_len);
//			  showLeftRollString(OutPutStr,char_len);
//			  
//			
//		}
//		
//}







/***********************�·���**************************/


