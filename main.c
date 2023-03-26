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
 * ÷ 切换夜间和白天
 * C\ON 结束本次行程
 * + 按下重新开始行程
 * = 切换行驶, 暂停模式
 * 9 起步价增加, 8起步价减少
 * 6 行程单价增加, 5行程单价减少
 */
char FormatString[17] = {0};		   // 用于格式化字符串
bit Waiting_Mode = 0;				   // 是否等待模式
bit Charging_Mode = 1;				   // 储存收费模式
unsigned int Stepper_Step = 0;		   // 储存步进电机运转步数
unsigned int Total_Travel = 0;		   // 储存总行程
unsigned int Total_Charging = 0;	   // 储存收费价格
unsigned char Price[4] = {4, 2, 1, 5}; // 储存夜间, 白天, 停止收费, 起步价
unsigned char PressKey = 0;			   // 储存按下的按键
unsigned char TimersCount[2] = {0};	   // 用于按钮消抖
unsigned char Timer0_Count = 0;		   // 定时器0中断计数

/**
 * @brief 定时器0中断回调函数
 *
 */
void Timer0_Callback(void) interrupt 1
{
	TL0 = 0xF0;						// 设置定时器0初值
	TH0 = 0xD8;						// 设置定时器0初值
	if (Waiting_Mode)				// 如果是等待
		Total_Charging += Price[2]; // 计算等待价格
	else
	{
		Total_Travel += 1;							// 计算行程
		if (Total_Travle >= 500)					// 行程500后在计算费用
			Total_Charging += Price[Charging_Mode]; // 计算行驶价格
		switch (Stepper_Step++)						// 根据步数驱动电机
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
	LCD_Init();	   // 初始化LCD
	Timer0_Init(); // 初始化定时器0
	sprintf(FormatString, "%s单价: %d\0",
			Charging_Mode ? "白天" : "夜间",
			(int)Price[Charging_Mode]);						   // 格式化字符串
	LCD_ShowString(0, 0, FormatString);						   // 显示字符串
	sprintf(FormatString, "起步价: %d     \0", (int)Price[3]); // 格式化字符串
	LCD_ShowString(1, 0, FormatString);						   // 显示字符串
	LCD_ShowString(2, 0, "当前行程: 0.00  ");				   // 显示字符串
	LCD_ShowString(3, 0, "行程总价: 0.00  ");				   // 显示字符串

	while (1)
	{
		if (TimersCount[0] == 0 && TR0 == 1) 						// 如果刷新屏幕时间为0, 且定时器0运行中
		{
			TimersCount[0] = 800;									// 刷新屏幕时间设置为800ms
			sprintf(FormatString, "当前行程: %d.%02d  ", 
				(int)Total_Travel / 100, (int)Total_Travel % 100);	// 格式化字符串
			LCD_ShowString(2, 0, FormatString);						// 显示字符串
			sprintf(FormatString, "行程总价: %d.%02d  ", 
				(int)Total_Charging / 100, (int)Total_Charging % 100); // 格式化字符串
			LCD_ShowString(3, 0, FormatString);						// 显示字符串
		}
		PressKey = KeypadScan();			 						// 扫描按键
		if (PressKey && TimersCount[1] == 0) 						// 如果按键按下, 且消抖时间为0
		{
			TimersCount[1] = 200;				  					// 消抖时间200ms
			if (PressKey == '=')				  					// 如果按下'='键, 切换运行状态
				Waiting_Mode = !Waiting_Mode;	  					// 翻转等待状态
			else if (PressKey == 'A' && TR0 == 0) 					// 如果按下'/'键
			{
				Charging_Mode = !Charging_Mode; 					// 翻转收费模式
				sprintf(FormatString, "%s单价: %d",
						Charging_Mode ? "白天" : "夜间",
						(int)Price[Charging_Mode]); 				// 格式化字符串
				LCD_ShowString(0, 0, FormatString);  				// 显示字符串
			}
			else if (PressKey == '9')  								// 如果9按下
			{
				Price[3]++;		// 起步价增加
				sprintf(FormatString, "起步价: %d          ", (int)Price[3]); // 格式化字符串
				LCD_ShowString(1, 0, FormatString);							  // 显示字符串
			}
			else if (PressKey == '8') // 如果8按下
			{
				if (Price[3] > 1)											  // 如果起步价大于1
					Price[3]--;												  // 起步价减少
				sprintf(FormatString, "起步价: %d          ", (int)Price[3]); // 格式化字符串
				LCD_ShowString(1, 0, FormatString);							  // 显示字符串
			}
			else if (PressKey == '6') // 如果6按下
			{
				Price[Charging_Mode]++; // 行程单价增加
				sprintf(FormatString, "%s单价: %d   ",
						Charging_Mode ? "白天" : "夜间",
						(int)Price[Charging_Mode]); // 格式化字符串
				LCD_ShowString(0, 0, FormatString); // 显示字符串
			}
			else if (PressKey == '5') // 如果5按下
			{
				if (Price[Charging_Mode] > 1) // 如果行程单价大于1
					Price[Charging_Mode]--;	  // 行程单价减少
				sprintf(FormatString, "%s单价: %d   ",
						Charging_Mode ? "白天" : "夜间",
						(int)Price[Charging_Mode]); // 格式化字符串
				LCD_ShowString(0, 0, FormatString); // 显示字符串
			}
			else if (PressKey == '/' && TR0 == 1) // 如果C/ON按下, 且定时器是运行的
			{
				TR0 = 0;// 定时器停止运行
				sprintf(FormatString, "总行程: %d.%02d  ", (int)Total_Travel / 100, (int)Total_Travel % 100);		// 格式化字符串
				LCD_ShowString(2, 0, FormatString);// 显示字符串
				sprintf(FormatString, "行程总价: %d.%02d  ", (int)Total_Charging / 100, (int)Total_Charging % 100); // 格式化字符串
				LCD_ShowString(3, 0, FormatString);// 显示字符串
			}
			else if (PressKey == 'D' && TR0 == 0) 		// 如果+按下, 且定时器是停止的
			{
				Total_Travel = 0;						// 行程清0
				Total_Charging = (Price[3] * 100);		// 添加起步价
				Waiting_Mode = 0;						// 退出等待模式
				sprintf(FormatString, "当前行程: %d.%02d  ", (int)Total_Travel / 100, (int)Total_Travel % 100);
														// 格式化字符串
				LCD_ShowString(2, 0, FormatString);		// 显示字符串
				sprintf(FormatString, "行程总价: %d.%02d  ", (int)Total_Charging / 100, (int)Total_Charging % 100);
														// 格式化字符串
				LCD_ShowString(3, 0, FormatString);		// 显示字符串
				TL0 = 0xF0;								// 设置定时器0初值
				TH0 = 0xD8;								// 设置定时器0初值
				TR0 = 1;								// 定时器开始运行
														// 开启总中断
			}
		}
		if (TimersCount[0] != 0) // 如果刷新时间不为0
			TimersCount[0]--;	 // 刷新时间自减
		if (TimersCount[1] != 0) // 如果消抖时间不为0
			TimersCount[1]--;	 // 消抖时间自减
		delay_ms(1);			 // 等待1ms
	}
}