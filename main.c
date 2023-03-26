#include "reg52.h"
#include "stdio.h"
#include "delay.h"
#include "keypad.h"
#include "timer0.h"
#include "lcd12864.h"

sbit Stepper_A = P3 ^ 4;
sbit Stepper_B = P3 ^ 7;
sbit Stepper_C = P3 ^ 6;
sbit Stepper_D = P3 ^ 5;

/**
 * �� �л�ҹ��Ͱ���
 * C\ON ���������г�
 * + �������¿�ʼ�г�
 * = �л���ʻ, ��ͣģʽ
 * 9 �𲽼�����, 8�𲽼ۼ���
 * 6 �г̵�������, 5�г̵��ۼ���
 */
char FormatString[17] = {0};		   // ���ڸ�ʽ���ַ���
bit Waiting_Mode = 0;				   // �Ƿ�ȴ�ģʽ
bit Charging_Mode = 1;				   // �����շ�ģʽ
unsigned int Stepper_Step = 0;		   // ���沽�������ת����
unsigned int Total_Travel = 0;		   // �������г�
unsigned int Total_Charging = 0;	   // �����շѼ۸�
unsigned char Price[4] = {4, 2, 1, 5}; // ����ҹ��, ����, ֹͣ�շ�, �𲽼�
unsigned char PressKey = 0;			   // ���水�µİ���
unsigned char TimersCount[2] = {0};	   // ���ڰ�ť����
unsigned char Timer0_Count = 0;		   // ��ʱ��0�жϼ���

/**
 * @brief ��ʱ��0�жϻص�����
 *
 */
void Timer0_Callback(void) interrupt 1
{
	TL0 = 0xF0;						// ���ö�ʱ��0��ֵ
	TH0 = 0xD8;						// ���ö�ʱ��0��ֵ
	if (Waiting_Mode)				// ����ǵȴ�
		Total_Charging += Price[2]; // ����ȴ��۸�
	else
	{
		Total_Travel += 1;							// �����г�
		if (Total_Travle >= 500)					// �г�500���ڼ������
			Total_Charging += Price[Charging_Mode]; // ������ʻ�۸�
		switch (Stepper_Step++)						// ���ݲ����������
		{
		case 1:
			Stepper_A = 1;
			Stepper_B = 0;
			Stepper_C = 0;
			Stepper_D = 0;
			break;
		case 2:
			Stepper_A = 1;
			Stepper_B = 1;
			Stepper_C = 0;
			Stepper_D = 0;
			break;
		case 3:
			Stepper_A = 0;
			Stepper_B = 1;
			Stepper_C = 0;
			Stepper_D = 0;
			break;
		case 4:
			Stepper_A = 0;
			Stepper_B = 1;
			Stepper_C = 1;
			Stepper_D = 0;
			break;
		case 5:
			Stepper_A = 0;
			Stepper_B = 0;
			Stepper_C = 1;
			Stepper_D = 0;
			break;
		case 6:
			Stepper_A = 0;
			Stepper_B = 0;
			Stepper_C = 1;
			Stepper_D = 1;
			break;
		case 7:
			Stepper_A = 0;
			Stepper_B = 0;
			Stepper_C = 0;
			Stepper_D = 1;
			break;
		case 8:
			Stepper_A = 1;
			Stepper_B = 0;
			Stepper_C = 0;
			Stepper_D = 1;
			Stepper_Step = 0;
			break;
		}
	}
}
void main(void)
{
	LCD_Init();	   // ��ʼ��LCD
	Timer0_Init(); // ��ʼ����ʱ��0
	sprintf(FormatString, "%s����: %d\0",
			Charging_Mode ? "����" : "ҹ��",
			(int)Price[Charging_Mode]);						   // ��ʽ���ַ���
	LCD_ShowString(0, 0, FormatString);						   // ��ʾ�ַ���
	sprintf(FormatString, "�𲽼�: %d     \0", (int)Price[3]); // ��ʽ���ַ���
	LCD_ShowString(1, 0, FormatString);						   // ��ʾ�ַ���
	LCD_ShowString(2, 0, "��ǰ�г�: 0.00  ");				   // ��ʾ�ַ���
	LCD_ShowString(3, 0, "�г��ܼ�: 0.00  ");				   // ��ʾ�ַ���

	while (1)
	{
		if (TimersCount[0] == 0 && TR0 == 1) 						// ���ˢ����Ļʱ��Ϊ0, �Ҷ�ʱ��0������
		{
			TimersCount[0] = 800;									// ˢ����Ļʱ������Ϊ800ms
			sprintf(FormatString, "��ǰ�г�: %d.%02d  ", 
				(int)Total_Travel / 100, (int)Total_Travel % 100);	// ��ʽ���ַ���
			LCD_ShowString(2, 0, FormatString);						// ��ʾ�ַ���
			sprintf(FormatString, "�г��ܼ�: %d.%02d  ", 
				(int)Total_Charging / 100, (int)Total_Charging % 100); // ��ʽ���ַ���
			LCD_ShowString(3, 0, FormatString);						// ��ʾ�ַ���
		}
		PressKey = KeypadScan();			 						// ɨ�谴��
		if (PressKey && TimersCount[1] == 0) 						// �����������, ������ʱ��Ϊ0
		{
			TimersCount[1] = 200;				  					// ����ʱ��200ms
			if (PressKey == '=')				  					// �������'='��, �л�����״̬
				Waiting_Mode = !Waiting_Mode;	  					// ��ת�ȴ�״̬
			else if (PressKey == 'A' && TR0 == 0) 					// �������'/'��
			{
				Charging_Mode = !Charging_Mode; 					// ��ת�շ�ģʽ
				sprintf(FormatString, "%s����: %d",
						Charging_Mode ? "����" : "ҹ��",
						(int)Price[Charging_Mode]); 				// ��ʽ���ַ���
				LCD_ShowString(0, 0, FormatString);  				// ��ʾ�ַ���
			}
			else if (PressKey == '9')  								// ���9����
			{
				Price[3]++;		// �𲽼�����
				sprintf(FormatString, "�𲽼�: %d          ", (int)Price[3]); // ��ʽ���ַ���
				LCD_ShowString(1, 0, FormatString);							  // ��ʾ�ַ���
			}
			else if (PressKey == '8') // ���8����
			{
				if (Price[3] > 1)											  // ����𲽼۴���1
					Price[3]--;												  // �𲽼ۼ���
				sprintf(FormatString, "�𲽼�: %d          ", (int)Price[3]); // ��ʽ���ַ���
				LCD_ShowString(1, 0, FormatString);							  // ��ʾ�ַ���
			}
			else if (PressKey == '6') // ���6����
			{
				Price[Charging_Mode]++; // �г̵�������
				sprintf(FormatString, "%s����: %d   ",
						Charging_Mode ? "����" : "ҹ��",
						(int)Price[Charging_Mode]); // ��ʽ���ַ���
				LCD_ShowString(0, 0, FormatString); // ��ʾ�ַ���
			}
			else if (PressKey == '5') // ���5����
			{
				if (Price[Charging_Mode] > 1) // ����г̵��۴���1
					Price[Charging_Mode]--;	  // �г̵��ۼ���
				sprintf(FormatString, "%s����: %d   ",
						Charging_Mode ? "����" : "ҹ��",
						(int)Price[Charging_Mode]); // ��ʽ���ַ���
				LCD_ShowString(0, 0, FormatString); // ��ʾ�ַ���
			}
			else if (PressKey == '/' && TR0 == 1) // ���C/ON����, �Ҷ�ʱ�������е�
			{
				TR0 = 0;// ��ʱ��ֹͣ����
				sprintf(FormatString, "���г�: %d.%02d  ", (int)Total_Travel / 100, (int)Total_Travel % 100);		// ��ʽ���ַ���
				LCD_ShowString(2, 0, FormatString);// ��ʾ�ַ���
				sprintf(FormatString, "�г��ܼ�: %d.%02d  ", (int)Total_Charging / 100, (int)Total_Charging % 100); // ��ʽ���ַ���
				LCD_ShowString(3, 0, FormatString);// ��ʾ�ַ���
			}
			else if (PressKey == 'D' && TR0 == 0) 		// ���+����, �Ҷ�ʱ����ֹͣ��
			{
				Total_Travel = 0;						// �г���0
				Total_Charging = (Price[3] * 100);		// ����𲽼�
				Waiting_Mode = 0;						// �˳��ȴ�ģʽ
				sprintf(FormatString, "��ǰ�г�: %d.%02d  ", (int)Total_Travel / 100, (int)Total_Travel % 100);
														// ��ʽ���ַ���
				LCD_ShowString(2, 0, FormatString);		// ��ʾ�ַ���
				sprintf(FormatString, "�г��ܼ�: %d.%02d  ", (int)Total_Charging / 100, (int)Total_Charging % 100);
														// ��ʽ���ַ���
				LCD_ShowString(3, 0, FormatString);		// ��ʾ�ַ���
				TL0 = 0xF0;								// ���ö�ʱ��0��ֵ
				TH0 = 0xD8;								// ���ö�ʱ��0��ֵ
				TR0 = 1;								// ��ʱ����ʼ����
														// �������ж�
			}
		}
		if (TimersCount[0] != 0) // ���ˢ��ʱ�䲻Ϊ0
			TimersCount[0]--;	 // ˢ��ʱ���Լ�
		if (TimersCount[1] != 0) // �������ʱ�䲻Ϊ0
			TimersCount[1]--;	 // ����ʱ���Լ�
		delay_ms(1);			 // �ȴ�1ms
	}
}