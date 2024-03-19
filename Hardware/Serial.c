#include "Serial.h"

/**
 *@brief Variables used to be called in main
 * 
 */
uint8_t Serial_TxPacket[PACKLENGTH];
uint8_t Serial_RxPacket[PACKLENGTH];
char Serial_txtRxPacket[TXTPACKLENGTH];
uint8_t Serial_Handling_Flag=SERIAL_TRUE;

/**
 * @brief static variable
 *
 */
enum NVIC_GroupConfigFlag_TypeDef NVIC_GroupConfigFlag;

/**
 * @brief can not be called by main 
 * 
 */
enum Send_Pack_Mode_TypeDef Send_Pack_Mode;
enum Recieve_Pack_Mode_TypeDef Recieve_Pack_Mode;
enum Serial_DataException_TypeDef Serial_DataException;
enum Recieve_Area_State_TypeDef Recieve_Area_State;

/**
 * @brief Declaration of internal functions
 * 
 */
void USART1_RxITMethod(void);

/**
 * @brief  this function config USART1 (GPIO-->USART-->NVIC-->CPU)
 * it does the following things:
 *  1. turn on the clock of GPIOA and USART1 on APB2 bus
 *  2. init two GPIO ports
 *  3. init USART1
 *  4. turn on interrupt of USART1
 *  5. turn on the USART1 channel in NVIC
 */
void Serial_Init_1(Serial_InitStructTypeDef* Serial_InitStruct)
{
    NVIC_GroupConfigFlag = UN_USED;
    Send_Pack_Mode = (enum Send_Pack_Mode_TypeDef)Serial_InitStruct->Send_Pack_Mode_Value;
    Recieve_Pack_Mode = (enum Recieve_Pack_Mode_TypeDef)Serial_InitStruct->Recieve_Pack_Mode_Value;
    Recieve_Area_State = NOT_FULL;
    Serial_DataException = NO_EXCEPTION;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStruct);
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    if (NVIC_GroupConfigFlag == UN_USED)
    {
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        NVIC_GroupConfigFlag = USED;
    }
    /**
    *@brief When two interrupts occur at the same time:
    *  1. the interrupt with a higher priority preempts the interrupt with a lower priority
    *  2. preemption of interrupts with the same priority does not involve interrupt nesting,the interrupt with a higher priority responds first
    *  3. If the preemption priority and response priority are the same, compare their hardware interrupt numbers. The smaller the interrupt number, the higher the priority
    */
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =  1;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

/**
 * @brief 
 * @param  USARTx: Select the USART or the UART peripheral. 
 *   This parameter can be one of the following values:
 *   USART1, USART2, USART3, UART4 or UART5.
 * @param  Byte: This param can be of uint_8
 */
void Serial_SendByte(USART_TypeDef *USARTx,uint8_t Byte)
{
    USART_SendData(USARTx, Byte);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

/**
 * @brief Send array through USARTx
 * @param  USARTx: Select the USART or the UART peripheral. 
 *   This parameter can be one of the following values:
 *   USART1, USART2, USART3, UART4 or UART5.
 * @param  Array: This param can be of uint8_t*
 */
void Serial_SendArray(USART_TypeDef *USARTx,uint8_t* Array)
{
    uint8_t Num = sizeof(Array) / sizeof(Array[0]);
    for (uint8_t i = 0;i < Num;i++)
        Serial_SendByte(USARTx,Array[i]);
}

/**
 * @brief Send String through USARTx
 * @param  USARTx: Select the USART or the UART peripheral. 
 *   This parameter can be one of the following values:
 *   USART1, USART2, USART3, UART4 or UART5.  
 * @param  String: This param can be of char*
 */
void Serial_SendString(USART_TypeDef* USARTx, char* String)
{
    for (uint8_t i = 0;String[i] != '\0';i++)
        Serial_SendByte(USARTx,String[i]);
}

/**
 *@brief 
 * 
 * @param USARTx 
 * @param Number 
 */
void Serial_SendNumber(USART_TypeDef* USARTx,uint32_t Number)
{
    uint32_t Quotient;
    uint8_t Reminder;
    uint8_t Count = 0;
    uint8_t Temp[10];
    do {
        Reminder = Number % 10;
        Quotient = Number / 10;
        Temp[Count] = Reminder;
        Count++;
        Number = Quotient;
    } while (Quotient != 0);
    for (;Count > 0;Count--)
        Serial_SendByte(USARTx, Temp[Count - 1]+'0');   /*The reason why adding character '0' is number to char differ a '0' */
}

/**
 *@brief 
 * 
 */
void Serial_SendPacket(USART_TypeDef* USARTx)
{
    if (Send_Pack_Mode == SNUM)
    {
        Serial_SendByte(USARTx,0xFF);
        Serial_SendArray(USARTx, Serial_TxPacket);
        Serial_SendByte(USARTx, 0xFE);
    }
    else
    {
        Serial_SendString(USART1, Serial_txtRxPacket);
    }
}

/**
 *@brief 
 * 
 */
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
        if(Serial_Handling_Flag==SERIAL_TRUE)
            USART1_RxITMethod();
    
}

/**
 *@brief 
 * 
 */
void USART1_RxITMethod(void)
{
    static enum {
        WAITING = 0,
        TRANSMITING = 1,
        ENDING = 2
    } RxState;

    static uint8_t RxCount = 0;

    if (Recieve_Area_State == NOT_FULL)
    {
        if (Recieve_Pack_Mode == RTXT)
        {
            uint8_t RxData = USART_ReceiveData(USART1);
            if (RxState == WAITING)
            {
                if (RxData == '@')
                    RxState = TRANSMITING;
            }
            else if (RxState == TRANSMITING)
            {
                if (RxData == '\r')
                    RxState = ENDING;
                else if (RxCount < TXTPACKLENGTH - 3)
                {
                    Serial_txtRxPacket[RxCount] = RxData;
                    RxCount++;
                }
                else
                {
                    Serial_txtRxPacket[RxCount] = '\0';
                    RxState = WAITING;
                    RxCount = 0;
                    Recieve_Area_State = FULL;          
                    Serial_DataException = SERIAL_RXPACKET_EXCEPTION;
                }
            }
            else if (RxState == ENDING)
            {
                if (RxData == '\n')
                {
                    RxState = WAITING;
                    Serial_txtRxPacket[RxCount] = '\0';
                }
            }
        }
        else
        {
            uint8_t RxData = USART_ReceiveData(USART1);
            if (RxState == WAITING)
            {
                if (RxData == 0xFF)
                    RxState = TRANSMITING;
            }
            else if (RxState == TRANSMITING)
            {
                if (RxCount < PACKLENGTH)
                {
                    Serial_RxPacket[RxCount] = RxData;
                    RxCount++;
                }
                else if (RxData == 0xFE)
                {
                    Serial_txtRxPacket[RxCount] = 0xFE;
                    RxState = WAITING;
                }
                else
                {
                    Serial_txtRxPacket[RxCount] = 0xFE;
                    Serial_DataException = SERIAL_TXPACKET_EXCEPTION;
                }
            }
        }
    }
    else
    {
        Serial_SendString(USART1, "warning: Buffer full");
        Serial_DataException = SERIAL_RXPACKET_EXCEPTION;
        for (uint8_t i = 0;i < sizeof(Serial_txtRxPacket) - 1;i++)
            Serial_txtRxPacket[i] = 0;
    }
}
