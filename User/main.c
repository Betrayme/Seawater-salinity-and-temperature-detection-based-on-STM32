#include "stm32f10x.h"                  // Device header
#include "UserPripheral_conf.h"

/**
 * @brief there is the entrance of the program
 * @param none
 * @return int 
 */
int main(void)
{
	OLED_Init();

	Serial_InitStructTypeDef Serial_InitStruct;
	Serial_InitStruct.Recieve_Pack_Mode_Value = TXT;
	Serial_InitStruct.Send_Pack_Mode_Value = TXT;
	Serial_Init_1(&Serial_InitStruct);
	
	while (1)
	{
		OLED_ShowString(1, 1, Serial_txtRxPacket);
	}
}
