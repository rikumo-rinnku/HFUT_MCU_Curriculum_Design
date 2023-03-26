#ifndef __LCD12864_H__
#define __LCD12864_H__

#include "reg52.h"
#include "delay.h"

void LCD_Init(void);
void LCD_ShowChar(unsigned char Line, unsigned char Column, char Char);
void LCD_ShowString(unsigned char Line, unsigned char Column, char *String);

#endif