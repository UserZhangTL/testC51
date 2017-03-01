#ifndef __UART_H__
#define __UART_H__

extern void uart_init();
extern void uart_sendbyte(unsigned char byte);
extern void uart_sendstr(unsigned char *s);

#endif