#ifndef __IIC_H__
#define __IIC_H__

extern void IIC_start();
extern void IIC_stop();
extern void IIC_SendByte(unsigned char temp);
extern void IIC_ACK();
extern void IIC_NOACK();
extern unsigned char IIC_RcvByte();
extern void check_ack();
extern unsigned char AT24C02_SendStr(unsigned char deviceaddr,unsigned char romaddr,unsigned char *s,unsigned char num);
extern unsigned char AT24C02_RcvStr(unsigned char deviceaddr,unsigned char romaddr,unsigned char *s,unsigned char num);

#endif