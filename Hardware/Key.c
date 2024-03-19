#include "Key.h"
/**
 *@brief 
 *@param none
 */
void Key_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_KeyGroup, &GPIO_InitStructure);
}
/**
 * @brief 
 * 
 * @param Key_Pin: choose witch pin to KEY
 * @return uint8_t 
 */
uint8_t Key_Get(uint16_t Key_Pin)
{
    if (GPIO_ReadInputDataBit(GPIO_KeyGroup, Key_Pin) == 0)
    {
        Delay_ms(20);
        if (GPIO_ReadInputDataBit(GPIO_KeyGroup, Key_Pin) == 0)
        {
            while (1)
            {
                while (!GPIO_ReadInputDataBit(GPIO_KeyGroup, Key_Pin))
                {
                    
                }
                Delay_ms(20);
                if (GPIO_ReadInputDataBit(GPIO_KeyGroup, Key_Pin) == 1)
                    return KEY_ON;
                else
                    return KEY_OFF;
            }
        }
        else
            return KEY_OFF;
    }
    else
        return KEY_OFF;
}
