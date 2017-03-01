#ifndef __WIFIESP8266_H__
#define __WIFIESP8266_H__

#define BEGIN  1
#define END    0

#define OK     1
#define NO    0

extern bit check_revStr();
extern void clean_flag();
extern bit SendStrToClint(unsigned char *str,int client_num,int str_len);
extern void Func_Cmd(unsigned char cmd);
extern void WifiESP8266_Init(unsigned char *,unsigned char *,unsigned char *);

#endif