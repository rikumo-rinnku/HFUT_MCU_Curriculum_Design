#include "keypad.h"

// ���Ŷ���
#define KeyPad_Port P1

/**
 * @brief ɨ����̰��µİ���
 *
 * @return unsigned char ɨ�赽�İ���
 */
unsigned char KeypadScan(void)
{
    // ѡ�е�һ��
    KeyPad_Port = 0x7F;
    // �����е�ƽ����ɨ����
    if (!(KeyPad_Port & 0x01))
        return 'A';
    if (!(KeyPad_Port & 0x02))
        return '9';
    if (!(KeyPad_Port & 0x04))
        return '8';
    if (!(KeyPad_Port & 0x08))
        return '7';
    // ѡ�еڶ���
    KeyPad_Port = 0xBF;
    // �����е�ƽ����ɨ����
    if (!(KeyPad_Port & 0x01))
        return 'B';
    if (!(KeyPad_Port & 0x02))
        return '6';
    if (!(KeyPad_Port & 0x04))
        return '5';
    if (!(KeyPad_Port & 0x08))
        return '4';
    // ѡ�е�����
    KeyPad_Port = 0xDF;
    // �����е�ƽ����ɨ����
    if (!(KeyPad_Port & 0x01))
        return 'C';
    if (!(KeyPad_Port & 0x02))
        return '3';
    if (!(KeyPad_Port & 0x04))
        return '2';
    if (!(KeyPad_Port & 0x08))
        return '1';
    // ѡ�е�����
    KeyPad_Port = 0xEF;
    // �����е�ƽ����ɨ����
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
