/**
 *@file Key.h
 * @author Alvari 
 * @brief 
 * @version 0.1
 * @date 2023-08-19
 * 
 * @copyright Copyright (c) 2023
 * @example
 *int main()	
{
	BitAction Key1_Num = Bit_SET;
	BitAction Key2_Num = Bit_SET;
	LED_Init();
	Key_Init();
	while (1)
	{
		if (Key_Get(Key1) == KEY_ON)
		{
			if (Key1_Num == Bit_SET)
				Key1_Num = Bit_RESET;
			else
				Key1_Num = Bit_SET;
			GPIO_WriteBit(GPIO_LEDGroup, LED1, Key1_Num);
		}

		if (Key_Get(Key2) == KEY_ON)
		{
			if (Key2_Num == Bit_SET)
				Key2_Num = Bit_RESET;
			else
				Key2_Num = Bit_SET;
			GPIO_WriteBit(GPIO_LEDGroup, LED2, Key2_Num);
		}
				
	}
}
 */
#ifndef __KEY_H
#define __KEY_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define GPIO_KeyGroup GPIOB
#define Key1 GPIO_Pin_1
#define Key2 GPIO_Pin_11
#define KEY_ON 0
#define KEY_OFF 1

void Key_Init(void);
uint8_t Key_Get(uint16_t GPIO_Pin);

#endif
