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

/*PCF8563使用到的头文件*/
#include <absacc.h>
#include "PCF8563.h"
#include "DS18B20.h"

#include <reg52.h>
#include <intrins.h>
#include "delay.h"
#include "uart.h"
#include <stdio.h>


#define uchar unsigned char
#define ROLL   2//滚动
#define STATIC 3//静态切换


/*定时器0中使用的变量*/
#define NO  0
#define YES 1
#define MAX_LEN 128


#define TIMERVAL     2100//每次进定时器中断的溢出时间 单位us
#define US_COUNT_MAX (TIMERVAL / TIMERVAL)   //每刷新一次屏幕的间隔，一般在1500us左右
#define MS_COUNT_MAX  (500000 / TIMERVAL)   //滚屏左移字节的速度，一般5-10ms为适宜
#define S_COUNT_MAX  (1500000 / TIMERVAL)    //一秒到来切换 静态切换时间间隔
#define S_COUNT_MAX_PCF8563  (1000000 / TIMERVAL) //一秒采集一次实时时间
#define S_COUNT_MAX_TMP (2000000 / TIMERVAL)



#endif