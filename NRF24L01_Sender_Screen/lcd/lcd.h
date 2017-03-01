#ifndef __LCD_H__
#define __LCD_H__



extern void lcd_clean();
extern void lcd_write_cmd(unsigned char byte);
extern void lcd_write_data(unsigned char byte);
extern void lcd_init();
extern void lcd_write_char(unsigned char x,unsigned char y,unsigned char byte);
extern void lcd_write_str(unsigned char x,unsigned char y,unsigned char *s);



#endif