#include "keypad.h"

// 引脚定义
#define KeyPad_Port P1

/**
 * @brief 扫描键盘按下的按键
 *
 * @return unsigned char 扫描到的按键
 */
unsigned char KeypadScan(void)
{
    // 选中第一行
    KeyPad_Port = 0x7F;
    // 根据列电平返回扫描结果
    if (!(KeyPad_Port & 0x01))
        return 'A';
    if (!(KeyPad_Port & 0x02))
        return '9';
    if (!(KeyPad_Port & 0x04))
        return '8';
    if (!(KeyPad_Port & 0x08))
        return '7';
    // 选中第二行
    KeyPad_Port = 0xBF;
    // 根据列电平返回扫描结果
    if (!(KeyPad_Port & 0x01))
        return 'B';
    if (!(KeyPad_Port & 0x02))
        return '6';
    if (!(KeyPad_Port & 0x04))
        return '5';
    if (!(KeyPad_Port & 0x08))
        return '4';
    // 选中第三行
    KeyPad_Port = 0xDF;
    // 根据列电平返回扫描结果
    if (!(KeyPad_Port & 0x01))
        return 'C';
    if (!(KeyPad_Port & 0x02))
        return '3';
    if (!(KeyPad_Port & 0x04))
        return '2';
    if (!(KeyPad_Port & 0x08))
        return '1';
    // 选中第四行
    KeyPad_Port = 0xEF;
    // 根据列电平返回扫描结果
    if (!(KeyPad_Port & 0x01))
        return 'D';
    if (!(KeyPad_Port & 0x02))
        return '=';
    if (!(KeyPad_Port & 0x04))
        return '0';
    if (!(KeyPad_Port & 0x08))
        return '/';
    return 0;
}
