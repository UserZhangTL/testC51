#ifndef __ALLFUNC_H__
#define __ALLFUNC_H__

#include <reg52.h>
#include <intrins.h>
#include <stdio.h>

#include <string.h>
#include "AllFunc.h"
#include "delay.h"
//#include "Beep.h"
//#include "keyboard_v.h"
#include "uart.h"
#include "NRF24L01_Receiver.h"
//#include "tmp.h"
//#include "tmp_hum.h"
#include "LED.h"
#include "zimo.h"

#include <string.h>
#include "WifiESP8266.h"

/*PCF8563ʹ�õ���ͷ�ļ�*/
#include <absacc.h>
#include "PCF8563.h"
#include "DS18B20.h"

#include <reg52.h>
#include <intrins.h>
#include "delay.h"
#include "uart.h"
#include <stdio.h>


#define uchar unsigned char
#define ROLL   2//����
#define STATIC 3//��̬�л�


/*��ʱ��0��ʹ�õı���*/
#define NO  0
#define YES 1
#define MAX_LEN 128


#define TIMERVAL     2100//ÿ�ν���ʱ���жϵ����ʱ�� ��λus
#define US_COUNT_MAX (TIMERVAL / TIMERVAL)   //ÿˢ��һ����Ļ�ļ����һ����1500us����
#define MS_COUNT_MAX  (500000 / TIMERVAL)   //���������ֽڵ��ٶȣ�һ��5-10msΪ����
#define S_COUNT_MAX  (1500000 / TIMERVAL)    //һ�뵽���л� ��̬�л�ʱ����
#define S_COUNT_MAX_PCF8563  (1000000 / TIMERVAL) //һ��ɼ�һ��ʵʱʱ��
#define S_COUNT_MAX_TMP (2000000 / TIMERVAL)



#endif