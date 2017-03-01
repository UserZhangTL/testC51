#include "AllFunc.h"

#define ERROR 1
#define SUCCESS 0
unsigned char ir_time = 0;
bit start_flag = 0;
bit ir_ok = 0;
bit ir_process_ok = 0;
unsigned char ir_bit_time[33];  //ʱ��
unsigned char ir_code[4];

void time0_init()
{
    EA = 1;
		TMOD |= 0x02; //8λ�Զ���װ
		TL0 = 0;
		TH0 = 0;
		ET0 = 1;
		TR0 = 1;
}

void int0_init()
{
    EA = 1;
		IT0 = 1; //�½��ش���
		EX0 = 1;
}

void ir_process()
{
    unsigned char i = 0;
		unsigned char j = 0; 
		unsigned char k = 1;    //��0λΪ�����룬�ӵ�һλ��ʼ
	
		for(j = 0; j < 4; j++)
		{
				for(i = 0; i < 8; i++)
				{
						ir_code[j] >>= 1;

						if(ir_bit_time[k++] > 6)  //����6ms��λΪ1��С��6ms��λΪ0
						{
								ir_code[j] |= 0x80;
						}
				}
		} 
	
		ir_process_ok = 1;   
}

unsigned char ir_scan()	   //�����߼���ֵ����1�ŵ�һ�������ڶ�����������
{
    if(ir_code[0] + ir_code[1] != 0xff || ir_code[2] + ir_code[3] != 0xff)
		{
				return ERROR;
		}

		switch(ir_code[2])
		{
				case 0x40:return 1;break;
				case 0x45:return 2;break;
				case 0x47:return 3;break;
				default : return 0xff;
		}
}

// void main()
// {
//     time0_init();
// 		int0_init(); 
// 		uart_init();

// 		while(1)
// 		{
// 				if(1 == ir_ok)
// 				{
// 						ir_process();
// 						ir_ok = 0;
// 				}
// 				if(1 == ir_process_ok)
// 				{
// 						uart_sendbyte(ir_code[0]);
// 						uart_sendbyte(ir_code[1]);
// 						uart_sendbyte(ir_code[2]);
// 						uart_sendbyte(ir_code[3]);

// 						ir_process_ok = 0;
// 				}
// 		}   
// }

void int0_isr() interrupt 0
{
    static unsigned char bit_num = 0;

    if(1 == start_flag)
		{
				if((ir_time > 30) && (ir_time < 60))
				{
						bit_num = 0;
				}

				ir_bit_time[bit_num++] = ir_time;

					ir_time = 0;

				if(33 == bit_num)
				{
						bit_num = 0;
						start_flag = 0;
						ir_ok = 1;
		//			uart_send_str(ir_bit_time);	  �����յ���һ������ʱ��
				}
		}
		else
		{
				start_flag = 1;
			ir_time = 0;
		}
}

void time0_isr() interrupt 1
{
    ir_time++;
}
