#include "timer0.h"

/**
 * @brief 初始化定时器0, 10ms中断
 *
 */
void Timer0_Init(void)
{
    TMOD &= 0xF0; // 清除定时器0模式
    TMOD |= 0x01; // 设置定时器0模式
    TL0 = 0xF0;   // 设置定时器0初值
    TH0 = 0xD8;   // 设置定时器0初值
    TF0 = 0;      // 清除TF0标识
    TR0 = 0;      // 定时器0停止计时
    ET0 = 1;      // 开启定时器0中断
    EA = 1;       // 开启总中断
}
