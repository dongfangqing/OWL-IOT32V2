#ifndef _OLED_DEVICE_H_
#define _OLED_DEVICE_H_






void LCD_Reset(void);
void LCD_Set_Pos(unsigned char x, unsigned char y);
void LCD_Fill(unsigned char bmp_dat);
void LCD_CLS(void);

void LCD_Init(void);
void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char bmp[]);


#endif

