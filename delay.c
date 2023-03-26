#include "delay.h"

/**
 * @brief 延时指定的ms数
 *
 * @param ms 要延时的ms数
 */
void delay_ms(unsigned int ms)
{
    unsigned int i;
    do
    {
        i = 125;
        while (--i)
            ;
    } while (--ms);
}

// /**
//  * @brief 延时10us
//  *
//  */
// void delay_10us(void)
// {
//     unsigned char i;
//     _nop_();
//     i = 2;
//     while (--i)
//         ;
// }
