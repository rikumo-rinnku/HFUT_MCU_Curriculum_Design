#include "delay.h"

/**
 * @brief ��ʱָ����ms��
 *
 * @param ms Ҫ��ʱ��ms��
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
//  * @brief ��ʱ10us
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
