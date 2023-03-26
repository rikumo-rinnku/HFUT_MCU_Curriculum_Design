
#include "lcd12864.h"

// 引脚配置
sbit LCD_RS = P2 ^ 0;
sbit LCD_RW = P2 ^ 1;
sbit LCD_EN = P2 ^ 2;
#define LCD_DATA_PORT P0
void LCD_CheckBusy(void)
{
    LCD_RS = 0;
    LCD_RW = 1;
    LCD_EN = 1;
    LCD_DATA_PORT = 0xFF;
    while ((LCD_DATA_PORT & 0x80) == 0x80)
        ;
    LCD_EN = 0;
}
/**
 * @brief  LCD12864写命令
 * @param  Command 要写入的命令
 * @retval 无
 */
void LCD_WriteCommand(unsigned char Command)
{
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 0;
    LCD_DATA_PORT = Command;
    LCD_EN = 1;
    delay_ms(5);
    LCD_EN = 0;
    delay_ms(1);
}

/**
 * @brief  LCD12864写数据
 * @param  Data 要写入的数据
 * @retval 无
 */
void LCD_WriteData(unsigned char Data)
{
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_EN = 0;
    LCD_DATA_PORT = Data;
    LCD_EN = 1;
    delay_ms(5);
    LCD_EN = 0;
    delay_ms(1);
}

/**
 * @brief  LCD12864设置光标位置
 * @param  Line 行位置，范围：0~1
 * @param  Column 列位置，范围：0~15
 * @retval 无
 */
void LCD_SetCursor(unsigned char Line, unsigned char Column)
{
    if (Line == 0)
    {
        LCD_WriteCommand(0x80 + Column);
    }
    else if (Line == 1)
    {
        LCD_WriteCommand(0x90 + Column);
    }
    else if (Line == 2)
    {
        LCD_WriteCommand(0x88 + Column);
    }
    else if (Line == 3)
    {
        LCD_WriteCommand(0x98 + Column);
    }
    delay_ms(1);
}

/**
 * @brief  LCD12864初始化函数
 * @param  无
 * @retval 无
 */
void LCD_Init(void)
{
    LCD_RW = 0; // 只写入数据不读取
    LCD_WriteCommand(0x30);
    LCD_WriteCommand(0x0c);
    LCD_WriteCommand(0x01);
    LCD_WriteCommand(0x06);
}
/**
 * @brief  在LCD12864指定位置上显示一个字符
 * @param  Line 行位置，范围：0~1
 * @param  Column 列位置，范围：0~15
 * @param  Char 要显示的字符
 * @retval 无
 */
void LCD_ShowChar(unsigned char Line, unsigned char Column, char Char)
{
    LCD_SetCursor(Line, Column);
    LCD_WriteData(Char);
}
/**
 * @brief  在LCD12864指定位置开始显示所给字符串
 * @param  Line 起始行位置，范围：0~1
 * @param  Column 起始列位置，范围：0~15
 * @param  String 要显示的字符串
 * @retval 无
 */
void LCD_ShowString(unsigned char Line, unsigned char Column, char *String)
{
    unsigned char i;
    LCD_SetCursor(Line, Column);
    for (i = 0; String[i] != '\0'; i++)
    {
        if (i >= 16)
            break;
        LCD_WriteData(String[i]);
    }
}
