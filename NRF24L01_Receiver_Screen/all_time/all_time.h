#ifndef __ALL_TIME_H__
#define __ALL_TIME_H__

//ÍòÄêÀú
extern void ds1302_writebyte(unsigned char addr,unsigned char byte);
extern unsigned char ds1302_readbyte(unsigned char addr);
extern void ds1302_writetime();
extern void ds1302_readtime();
extern void distime();

#endif