#ifndef __TMP_HUM_NEW_H__
#define __TMP_HUM_NEW_H__


#define OK 1
#define ERROR 0
extern void Delay_1ms(unsigned int ms);
//函数的返回值表示读取数据是否成功 OK 表示成功 ERROR 表示失败
extern unsigned char ReadTempAndHumi(void);


#endif