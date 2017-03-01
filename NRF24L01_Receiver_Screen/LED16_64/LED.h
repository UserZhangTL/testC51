#ifndef __LED_16_64_H__
#define __LED_16_64_H__


extern void Modify_Str_Len(unsigned int *old_char_len);
extern void init_OutPutStr();
extern void InputByte( unsigned  char DataR1) ;
extern void Init_Timer0(void);
extern void showLeftRollString(unsigned char *outChar[128],unsigned char char_len);
extern void showStaticString(unsigned char *outChar[128],unsigned char real_char_len);
extern void transfer_String_to_OutPutStr(unsigned char *strbuf,unsigned int str_len);
extern void showStaitcInfo(unsigned char Needed_Info[][32],unsigned int temp_len);
extern void clear_used_extern();
extern void spellRTCTime();//填充时间
extern void showAliveTimeInfo();//显示时间
extern void showAliveTmpInfo();
extern void spellTmpWithChinese();



#endif