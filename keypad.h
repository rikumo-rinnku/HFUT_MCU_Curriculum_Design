#ifndef KEYPAD__H__
#define KEYPAD__H__

#include "reg52.h"

/**
 * @brief 扫描键盘按下的按键
 * 
 * @return unsigned char 扫描到的按键
 */
unsigned char KeypadScan(void);

#endif
