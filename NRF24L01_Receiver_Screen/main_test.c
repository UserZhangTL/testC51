#include "AllFunc.h"

/*����������ģ��ʹ�õ���һЩȫ�ֱ���*/
extern bit FifteenHundred_us_Come;//Ĭ����û�е�ʱ��ģ����˵Ļ�������while���ж�Ȼ�����
extern bit One_s_Come;
extern bit Four_hundred_ms_Come;//400ms�����ı�־λ��������ʾ��λ��
extern unsigned char *OutPutStr[MAX_LEN];//���������ʾ��ָ������
extern unsigned char row;//�У�ÿ�ν����һ������ʮ����
//�ڶ�ά�����еڶ�ά�е��ֽ�λ�ã�Ҳ������һ�й�����У�Ҫ��ʾ���İ˸��ֽ�
extern unsigned char row_loc;
extern unsigned int char_len;
extern unsigned int temp_char_len;//��ʾ������ʹ�õ���ģƫ��

extern unsigned char DIS[];//������


//bit OPEN_Flag = NO;//Ĭ�Ͽ����ǹرյģ��ð����Ž��п������߹رգ������������ܰ�ť����


/*wifiģ����ʹ�õ�*/
//���յ������ݺͳ���
extern unsigned char  Get_str[40];
extern unsigned char Get_str_loc;
bit print_flag = NO;
bit LastInfo_Flag = NO;//�л�Ϊ��һ������
bit NextInfo_Flag = NO;//�л�Ϊ��һ������
unsigned char ContentCount = 6;//���ڼ�����ʾ�ڼ������ݴ�0-6��һ��7�����ݣ�Ĭ��Ϊ�ر�ģʽ

extern int wifi_str_len;//����wifiģ�麯���е��ַ����ȱ���

/*ģʽѡ��*/

unsigned char MODE_CHANGE = ROLL;//ģʽѡ��

/*ʵʱʱ��ʹ�õ��ı���*/
extern unsigned char Time_Buf_ASCII[100];//��������洢��ʱ�ɼ�������ʱ��
bit Collect_Time_Flag_s = NO;//���òɼ�ʱ���źŵ�flag����ʱ����������һֱ����

/*�¶Ȳɼ������*/
bit Collect_Tmp_Flag_4s = NO;//�ɼ�DS18B20����
bit COLLECT_TMP_FLAG = NO;//�ֻ��������Ĳɼ����Ҫ������Ϣ���ֻ���ȥ��
extern int real_tmp;
int tempStorageTmp;//�����洢�Ƚ��������¶�ֵ


/*���ڸ������ݵı�ţ�����ѭ������ʾ����Ҫ�����ֻ��ͻ��˵��������°�ť��*/
void showContentBy_ContentCount()
{
	  switch(ContentCount)
		{
			//��ʾʱ�䣬�������ﻹû������̬���������
			case 0:{showAliveTimeInfo();break;}
			//��ʾ��ʪ��
			case 1:{showAliveTmpInfo();break;}
			//��ʾ��л��Ϣ
			case 2:{showStaitcInfo(ThankInfo,21);break;}
			//��ʾ����������Ŀ
			case 3:{showStaitcInfo(IntroductionInfo1,19);break;}
			//��ʾʹ��ģ��
			case 4:{showStaitcInfo(IntroductionInfo2,40);break;}
			//��ʾ����ͨ�ŷ���
			case 5:{showStaitcInfo(IntroductionInfo3,26);break;}
			//������
			default:{ContentCount = 0;break;}
		}
}


/*ר��Ϊʵʱʱ����¶Ȳɼ����ļ�⣬Ҫʵʱ��⣬Ȼ��ɼ�����*/
void specialForTimeTmp_Collecting()
{
	  /*ר��Ϊʵʱʱ������*/
		if(ContentCount == 0)//���Count==0��Ҳ����˵����ʾʱ�䣬��ô��ʱ������Ҫ��̬��䣬һ��һ��
		{
				/*ʵʱʱ�Ӳɼ����֣����ﵥ����һ��ɼ���������ƴװ����ʾ����*/
				if(Collect_Time_Flag_s == YES)
				{
						Collect_Time_Flag_s = NO;//��ձ�־λ����ʼ�ɼ�
						//�ɼ�ʱ�䣬�ɼ�����ʱ�䱻ƴװ�ɹ̶��ĸ�ʽ��Time_Buf_ASCII�У�ȫ����ASCII��
						P8563_gettime();
						spellRTCTime();//ƴװʱ��
				}
		}
		/*ר��Ϊ�¶�����*/
		else if(ContentCount == 1)
		{
				/*��ʪ���ź�����*/
				if(Collect_Tmp_Flag_4s == YES)
				{
						Collect_Tmp_Flag_4s = NO;//��ձ�־λ����ʼ�ɼ�
						getTmp_and_Sprintf();
					  ModifyTmp();//�����ɼ�����ֵ
						spellTmpWithChinese();//ƴװʱ��
				}
		}
}




/******************************************������***************************************************************/
void main()
{
    unsigned char RecData[5] = {0};//���ڴ�������ݿ����5���ֽڣ�����Ҳʹ������ֽ���Ϊһ������
    unsigned char i = 0;
		
		/*����ƴװ��ʾ���ֻ��ϵ���Ϣ*/
    unsigned char sendToClientBuf[50] = {0};
		
		int str_loc = 0;
		delay_s(2);
		uart_init();
		
		//�ɼ�һ���¶�
		getTmp_and_Sprintf();
		if(real_tmp != 0)//�״βɼ��ͷ��뵽����ı�����
		{
			  tempStorageTmp = real_tmp;
		}
		else
    {
			  tempStorageTmp = 25;//Ҫ��ʧ���ˣ���ô�ȷ���Ƚ����˵��¶�ֵ���ȴ���һ�θ���
		}
		
    Init_Timer0();
	  NRF24L01_Init();
    uart_init();
	  delay_s(1);
		
		//Ҫ��Ҫ�������õ�ǰʵʱʱ�ӣ���Ҫ���������Ȼ����������������ڵ�ʮ�����Ƶ�BCD�룬һ��ֻҪдһ�ξͿ���
		//P8563_init();		
		
		WifiESP8266_Init("Screen","0123456789","8080");
		
		ContentCount = 6;//Ĭ�Ͽ�ʼΪ�ر�ģʽ
		//������ʾ��ʾ��Ϣ
		 showStaitcInfo(InitInfo,4);
		
		//������°�������˵��Ҫ����Ϊ����ģʽ����������
	  //��Ҫ���Ե�STATUS = 0x2e����Ҫ����һ�飬Ȼ���������ڣ������2E,˵�����ͳɹ�
    SetNRF_ReceiveDataMode();
    while(1)
		{ 
        /*������ʾ��Ϣ���ֻ��������¶ȺͲɼ��������ʾ��Ϣ*/
        if(COLLECT_TMP_FLAG == YES)
				{
   					COLLECT_TMP_FLAG = NO;//��ղ���
					  getTmp_and_Sprintf();//����һ������
					  ModifyTmp();
					  sprintf(sendToClientBuf,"Tmp:%03d",tempStorageTmp);
					  SendStrToClint(sendToClientBuf,0,strlen(sendToClientBuf));
					  memset(sendToClientBuf,0,sizeof(sendToClientBuf));//���Ϊ��һ�β���׼��
				}					
			
			
			
				/*ģʽ�л�����*/
				if(MODE_CHANGE == ROLL)
				{
						specialForTimeTmp_Collecting();
						showLeftRollString(OutPutStr,char_len);
				}
				else if(MODE_CHANGE == STATIC)
				{
						specialForTimeTmp_Collecting();
						showStaticString(OutPutStr,char_len);
				}
				
				/*ͨ��wifi����������������л�����*/
				if(NextInfo_Flag == YES)
				{
						NextInfo_Flag = NO;
						if(ContentCount >= 5)
						{
								ContentCount = 0;
						}
						else
						{
								ContentCount ++;
						}
						//�����޸���ɣ��Ϳ�ʼ��ʾ�̶�������
						showContentBy_ContentCount();
				}
				else if(LastInfo_Flag == YES)
				{
						LastInfo_Flag = NO;
						if(ContentCount == 0)
						{
								ContentCount = 5;
						}
						else
						{
								ContentCount --;
						}
						showContentBy_ContentCount();
				}
			

				/*�������wifi������Ϣ��������ôҪ������һЩ����*/
							/*���ڴ�ӡ�յ�������*/
				if(check_revStr() == OK)//Ҳ���Ըĳ�while��ĳ������������
				{
						/*************д�Ի�ȡ���ַ����Ĳ���**************/
						//����1���ַ���������׼����ӡ
						if(wifi_str_len > 1)
						{
								//�����ַ�����������������������ӡ
								for(str_loc = 0;str_loc < wifi_str_len; str_loc ++)
								{
										if((Get_str[str_loc] == '$') || (Get_str[str_loc] == '#')|| (Get_str[str_loc] == '^')
											|| (Get_str[str_loc] == '&')|| (Get_str[str_loc] == '*')|| (Get_str[str_loc] == '['))
										{
												print_flag = NO;
										}
								}
								//����ܹ���ӡ����ӡ���������ܴ�ӡҪ��ԭ��־λ�ܹ���ӡ��״̬
								if(print_flag == OK)
								{
										/*���ô�ASCII��ӿڣ�ƴװ�ַ���*/
										/*���������֣�ע��Ҫ������ʵ��ȷ���ַ�������ֻ����ASCII��*/
										transfer_String_to_OutPutStr(Get_str,Get_str_loc);
									  //��������һ������Ϊ������ʾʵʱʱ����¶ȵ�ʱ�򣬲��ڸ��ֻ�������������
									  ContentCount = 6;
								}
								else//��ԭĬ�ϵ��ܹ���ӡ��־λ
								{
										print_flag = OK;
								}

						}
						clean_flag();//����wifiģ���е���պ���
				}
	

			  
				
			  /*������յ�����������*/
			  if(NRF_RecData(RecData))
				{
					  //��ʼ�����ڵ������ֽڣ����ֽ���Ϊ������
					  switch(RecData[2]) 
						{
							case 0x01:
							{
								  ContentCount = 0;//���ݼ�����־��0��ʾ��ʱ������ʾʱ��ģʽ 
								  showAliveTimeInfo();//��ʾʱ�䣬�������ﻹû������̬���������
								
								  break;
							}
							case 0x02:
							{
								  ContentCount = 1;//���ݼ�����1��ʾ��ʪ����ʾģʽ
								  showAliveTmpInfo();/*��̬��ʾ��ʪ��*/
								  break;
							}
							//��ʾ��л��Ϣ
							case 0x03:
							{ContentCount = 2;showStaitcInfo(ThankInfo,21);break;}
							//��ʾ����������Ŀ
							case 0x04:{ContentCount = 3;showStaitcInfo(IntroductionInfo1,19);break;}
							//��ʾʹ��ģ��
							case 0x05:{ContentCount = 4;showStaitcInfo(IntroductionInfo2,40);break;}
							//��ʾ����ͨ�ŷ���
							case 0x06:
							{ContentCount = 5;showStaitcInfo(IntroductionInfo3,26);break;}
							case 0x07:{MODE_CHANGE = ROLL;break;}
							case 0x08:{MODE_CHANGE = STATIC;break;}
							//��ʾ��ʼ������
							case 0x09:{ContentCount = 6;showStaitcInfo(InitInfo,4);break;}
							//������������л���һ������
							case 0x0a:{NextInfo_Flag = YES;break;}
							//û������ʱ�Ĳ���
							default:{break;}
						}
					
				}
			
				memset(RecData,0,4*sizeof(unsigned char));//ÿ�ν��������
    }
}

/*-----------------------------------------------
             ��ʱ��ִ�ж�̬ɨ��
------------------------------------------------*/
void Timer0_isr(void) interrupt 1
{
	  static unsigned int counter_s  = 0;
    static unsigned int counter_us = 0;
	  static unsigned int counter_ms = 0;
	  
	  static unsigned int counter_s_PCF8563 = 0;//ר��Ϊʵʱʱ�Ӷ���һ����������
	  static unsigned int counter_3s_Tmp = 0;
	
    TH0 = (65536 - TIMERVAL) / 256;
	  TL0 = (65536 - TIMERVAL) % 256;  
	
	  counter_s  ++;
	  counter_us ++;
	  counter_ms ++;
	
	  counter_s_PCF8563 ++;
	  counter_3s_Tmp ++;
	
	  if(counter_us == US_COUNT_MAX)//����һ�ξ���Ϊ��Ч�������while��û����գ���һֱ��Ч
		{
	      FifteenHundred_us_Come = YES;
			  counter_us = 0;//���
		}
		if(counter_ms == MS_COUNT_MAX)//�����ٶȴ�Լ4-5ms��λһ��
		{
			  Four_hundred_ms_Come = YES;
			  counter_ms = 0;
		}
	  if(counter_s == S_COUNT_MAX)//��Լ1s�л���̬������ʾ
		{
			  counter_s = 0;//���
			  One_s_Come = YES;//��־λ��Ч
		}
		
		
		if(counter_s_PCF8563 == S_COUNT_MAX_PCF8563)
		{
			  counter_s_PCF8563 = 0;//��ռ���
			  Collect_Time_Flag_s = YES;
		}
		
		if(counter_3s_Tmp == S_COUNT_MAX_TMP)
		{
			  counter_3s_Tmp = 0;
			  Collect_Tmp_Flag_4s = YES;
		}

}