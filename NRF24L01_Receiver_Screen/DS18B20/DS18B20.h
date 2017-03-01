#ifndef __DS18B20_H__
#define __DS18B20_H__


extern void ds_reset();
extern void ds_writebyte(unsigned char byte);
extern bit ds_readbit();
extern unsigned char ds_readbyte();
extern void getTmp_and_Sprintf();
extern void ModifyTmp();


#endif