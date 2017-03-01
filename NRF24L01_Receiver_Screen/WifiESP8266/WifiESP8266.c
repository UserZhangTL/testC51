/********************ʹ��ʾ��**********************************/

// /*����ESP8266��Ŀ��:��û�����Ϣ*/

// #include <reg52.h>
// #include "delay.h"
// #include "WifiESP8266.h"
// #include "lcd.h"
// #include "uart.h"
// #include "delay.h"
// #include <stdio.h>
// #include <string.h>

// extern bit Ok_flag;
// extern bit Str_Ready;
// extern bit Rev_Str_status;
// extern bit str_rev_flag;
// extern bit flag;
// extern unsigned char Get_str[40];
// extern int client_num;
// extern int str_len;


// void main()
// {						  	  
// 	  unsigned char temp_buf[16] = "Tmp:26 Hum:56";
// 	  int i = 0;
// 	  uart_init();
// 	
// 	  lcd_init();
// 	  lcd_clean();
// 	  delay_ms(500);  
// 	 
// 	  WifiESP8266_Init("ZTLTest","0123456789","8000");
// 	
// 	  while(1)
// 		{
// 				if(OK == SendStrToClint(temp_buf,0,strlen(temp_buf)))
// 					//���͸����Ӻ�Ϊ0�Ŀͻ���hhh�������ֽڳ���
// 				{
// 						lcd_clean();
// 						lcd_write_str(1,1,"SEND OK");
// 				}
// 				else
// 				{
// 						lcd_clean();
// 						lcd_write_str(1,1,"ERROR");
// 				}
//         
// 				if(check_revStr() == OK)//Ҳ���Ըĳ�while��ĳ������������
// 				{
// 						/*************д�Ի�ȡ���ַ����Ĳ���**************/
// 						sprintf(temp_buf,"%d %d %s\0",client_num,str_len,Get_str);
// 						lcd_write_str(0,0,temp_buf);
// 						/*****************************************/
// 						
// 					
// 						/*****************���Խ��ճɹ��󣬻ظ���Ϣ���ɲ���******************/
// // 						if(OK == SendStrToClint(Get_str,0,strlen(Get_str)))
// // 							//���͸����Ӻ�Ϊ0�Ŀͻ���hhh�������ֽڳ���
// // 						{
// // 								lcd_clean();
// // 								lcd_write_str(1,1,"SEND OK");
// // 						}
// // 						else
// // 						{
// // 								lcd_clean();
// // 								lcd_write_str(1,1,"ERROR");
// // 						}
// 						/******************************************************/
// 					
// 						
// 						/**************************����Ҫ�ӣ�********************************/
// 						clean_flag();//����wifiģ���е���պ���
// 						/*******************************************************/
// 				}
// 				
// 				

// 		}
// }








/************************************************************/
/*
wifiģ��ESP8266ģ����Ժ���
���ߣ�������
��ʼʱ�䣺2016/1/19
���ܣ���ɻ��������ӵȹ���

�޸�ʱ�䣺2016/1/28
���ܣ��ܹ�ʵ�������ĸ��ͻ��˷�����ʲô����
*/

#include "AllFunc.h"

//����״̬Ĭ��Ϊ����
bit Rev_status      = END;//����״̬λ
bit Rev_Str_status  = END;//�����ַ���״̬λ
bit Send_flag       = NO;//������Ϣ���ֻ�״̬λ
bit OK_flag         = NO;//�ɹ�״̬λ
bit Str_Ready       = NO;//����׼����״̬λ
bit First_dou_flag  = NO;//�ǲ��ǵ�һ������

//��ȡ���ַ�������
unsigned char  Get_str[40];
unsigned char Get_str_loc = 0;

//�ͻ����Ӻţ��Լ��ַ�������
int client_num = 0;
int wifi_str_len    = 0;
bit client_num_flag      = END;
bit str_len_flag         = END;
bit str_rev_flag         = END;



/*�����ʹ�õ���һЩ��־λ*/
extern bit LastInfo_Flag;//�л�Ϊ��һ������
extern bit NextInfo_Flag;//�л�Ϊ��һ������
extern unsigned char MODE_CHANGE;//ģʽ�л���flag

/*��ʼ�ɼ������*/
extern bit COLLECT_TMP_FLAG;





//����Ƿ���յ������ݣ��������������ã�������յ������ݣ���ô���͹�����������ȡȫ�ֱ���Get_str
//�����������������Ժ󣬻�Ҫ������պ�������ձ�־λ���Ա���һ��ʹ��
bit check_revStr()
{
    if(Str_Ready == OK)
		{
		    return OK;
		}
		return NO;
}
	
void clean_flag()
{
	  //��ղ���
	  memset(Get_str,0,sizeof(Get_str));
	  client_num = 0;
	  wifi_str_len = 0;
	  Get_str_loc = 0;//���һ����ȫ�ֱ������Ա���һ�β���,Get_strҪ�����溯�������������
	  Str_Ready = NO;//��ս�������׼���ñ�־λ
}


/*δ���ƣ�û�м���Ƿ��ͳɹ������ַ����Ľ�ȡ�ͽ�������*/
//�����ַ��������ͻ������Ӻ�Ϊnum�Ŀͻ��ˣ�����Ϊ����str
//str���������ݣ�str_len�����͵��ֽ�����client_num:�ͻ������Ӻţ�ע�⣺������Ϊ�ַ�����ʽ��
bit SendStrToClint(unsigned char *str,int client_num,int str_len)//�д������ַ��������������ж��Ƿ��ͳɹ�
{
	  //����ƴ��ATָ���buffer
	  unsigned char AT_temp[30] = {0};
    //ƴ�ӳɣ� "AT+CIPSEND=client_num,str_len\r\n"  ���÷��ͣ�=�����һ�������ǿͻ����Ӻż�client_num,�ڶ�������Ҫ���͵��ֽ���

    sprintf(AT_temp,"AT+CIPSEND=%d,%d\r\n",client_num,str_len);
    uart_sendstr(AT_temp);//����װ���ATָ��ͳ�ȥ
    delay_ms(50);
		//������Ҫ���͵��ֽ�
    uart_sendstr(str);
    delay_ms(200);
    //����һ���Ժ󣬲鿴�Ƿ��з��ͳɹ���־λ��Ҫ�Ƿ��ͳɹ��ˣ���ô����Send_flagΪ��Ч
    if(Send_flag == NO)
		{
	      return NO;
		}
		else
		{
			  //��������Ժ󣬻�Ҫ��ձ�־λΪ��Ч���Ա��´�ʹ��
			  Send_flag = NO;
			  return OK;
		}
}

//wifiģ��ĳ�ʼ������  wifi���ֺ�wifi����Ͷ˿����ƣ�IP���̶�Ϊ192.168.4.1
void WifiESP8266_Init(unsigned char *name,unsigned char *password,unsigned char *port)
{
	  //����ƴ��AT�ַ�������
	  unsigned char AT_tempBuf[50] = {0};

	  //����ATָ�����wifiģʽ��
	  uart_sendstr("AT+RST\r\n");//����
	  delay_s(1);

	  uart_sendstr("AT+CWMODE=2\r\n");//����ΪAPģʽ��wifiģ�鵱��·����
	  delay_s(1);

    //����wifi���ƺ�����
    sprintf(AT_tempBuf,"AT+CWSAP=\"%s\",\"%s\",11,4\r\n",name,password);
    uart_sendstr(AT_tempBuf);
    memset(AT_tempBuf,0,sizeof(AT_tempBuf));//�������
    delay_s(1);

    uart_sendstr("AT+RST\r\n");//����
	  delay_s(1);

    uart_sendstr("AT+CIPMUX=1\r\n");//��Ϊ��·
	  delay_s(1);

    //��ʼƴ�Ӵ��ж˿ںŵ��ַ���
		sprintf(AT_tempBuf,"AT+CIPSERVER=1,%s\r\n",port);
	  uart_sendstr(AT_tempBuf);//�򿪷�����Ҫƴ��
		memset(AT_tempBuf,0,sizeof(AT_tempBuf));//�������
	  delay_s(1);
}




//�жϷ�������������λ
void uart_isr() interrupt 4
{
	  //loc������ret_msgȫ�ֱ�����ƫ�ƣ�������װһ���ַ���
	  static unsigned char i = 0;
	  unsigned char temp;
	  ES = 0;
	  temp = SBUF;//temp���ܱ��ı䣬��Ϊ��λ��Ҫ�õ�
	
	  switch(temp)
		{
			//��һ������
			case '$':
			{
				  LastInfo_Flag = YES;
				  break;
			}
			case '#':
			{
				  NextInfo_Flag = YES;
				  break;
			}
			//������л�Ϊ����ģʽ
			case '&':
			{
				  MODE_CHANGE = ROLL;
				  break;
			}
			//�Ҽ����л�Ϊ��̬�л�ģʽ
			case '*':
			{
				  MODE_CHANGE = STATIC;
				  break;
			}
			//�ɼ�����
			case '[':
			{
				  COLLECT_TMP_FLAG = YES;
				  break;
			}
			
			default:break;
		}			
	
	
	
	
	  /*ESP8266��ȡ�ַ�������*/
    if(temp == '\n')//��ʼ��
		{
			  Rev_status = BEGIN;//���ÿ�ʼ����
		}
		else if(temp == '\r')//������
		{
			  Rev_status = END;//���ý�������
		}
		else//����\r\n������ַ�
		{
			
				/************************************************/
				//ר����������IPD��CIFSR����������ʱҪ��Rev_Str_status��Ϊ��Ч���ٴν��������ѭ��������һ���ַ�
				if(Rev_Str_status == BEGIN)
				{
					  if((str_rev_flag == END) &&(temp != ':') && (str_len_flag == BEGIN))//��ʼ�����ַ�������
						{
							  wifi_str_len = wifi_str_len * 10 + (temp - '0');
						}
						//Ϊ�˳�ֱ�֤��ֻ��һ����������ҽ���һ�Σ����ö��flag
					  if((str_rev_flag == END) &&(First_dou_flag == OK) && (temp == ',') && (client_num_flag == BEGIN) && (str_len_flag == END))//��һ�ν��յ��˶��ţ���ʼ�����ַ�������
						{
							  str_len_flag = BEGIN;
							  client_num_flag = END;
						}
						//��һ���������٣����룬�Ժ��ٽ���
					  if((str_rev_flag == END) &&(First_dou_flag == NO) && (temp == ',') && (client_num_flag == END))//��������
						{
							  client_num_flag = BEGIN;
							  First_dou_flag = OK;//�ǵ�һ������
						}
						//��ʼ�����û����Ӻ�
						if((str_rev_flag == END) && (temp != ',') && (client_num_flag == BEGIN))
						{
							  client_num = client_num * 10 + (temp - '0');
						}
						
						//��ʼ�����ַ���
						if(str_rev_flag == BEGIN)
						{
							  //�����ַ�����ȫ�ֱ����У��Ա�������
							  Get_str[Get_str_loc] = temp;
							  Get_str_loc ++;
							  //����ַ����ĳ��Ⱥ͸ոս��յ���ָ���ַ���������ͬ�����ٽ��գ�����β����
							  if(Get_str_loc == wifi_str_len)
								{
									  Get_str[Get_str_loc] = '\0';
									  Str_Ready       = OK;//���ñ�־λ��˵�����Ѿ����յ���һ�������ַ����ˣ����Խ��в����ˡ�
									  Rev_Str_status  = END;//��ս����ַ�����־λ��ʹ�ÿ����ٴν�������switchѭ��
									  client_num_flag = END;//��ս���client_num��־λ
										str_len_flag    = END;//��ս���str_len�ַ������ȱ�־λ
										str_rev_flag    = END;//��ս��������ַ�����־λ
									  First_dou_flag  = NO;//������ֵڼ������ŵı�־λ
								}
						}
						
						if(temp == ':')//Ҫ�ǿ�ʼ�ˣ�����ô���濪ʼ�����ַ�����Ϊstr_len���ַ�������
						{
							  str_rev_flag = BEGIN;
						}
				}
				/************************************************/
			
			 /************************************************/
			  //Ҫ�ڽ����ַ����ı�־λ��Ч�����ҽ���״̬λ��Ч��ʱ�����
			  if((Rev_Str_status == END) && (Rev_status == BEGIN))
				{
					  //��⵽��һ���Ժ������ý��ձ�־λ��Ч��ʱ������
					  switch(temp)
					  {
								case '+'://Ҫô���յ�+IPD Ҫô���յ� +CIFSR
								{
									  Rev_Str_status = BEGIN;//��ʼ���գ���ʱ���������switchѭ��
										Rev_status = END;
									  break;
								}
								case 'E'://����ʧ�ܣ�����ERROR
								{
									  Send_flag = NO;
									  Rev_status = END;
									  break;
								}
								case 'S'://���ͳɹ�������SEND OK
								{
									  Send_flag = OK;
									  Rev_status = END;
									  break;
								}
								default:
								{
									  Rev_status = END;
									  break;
								}
						}
				}

		}
	  
	  
	

		//��λʱʹ��
	  if(0x7f == temp)//special for Doflye
		{
			  i ++;
			  if(10 == i)
				{
					  i = 0;
					  ISP_CONTR = 0xe0;
        }
		}
		else
		{
			  i = 0;
		}
		RI = 0;
		ES = 1;
}



