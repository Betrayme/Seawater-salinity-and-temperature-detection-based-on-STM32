/**
 *@file Serial.h
 * @author Alvari (496199270@qq.com)
 * @brief this is a file witch packing serial port communication
 * @version 0.1
 * @date 2023-08-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __SERIAL_H
#define __SERIAL_H

#include "stm32f10x.h"
#include "stddef.h"

#define SERIAL_FALS 0
#define SERIAL_TRUE 1
#define TXT SERIAL_FALS
#define NUM SERIAL_TRUE

 /**
 * @defgroup PACK
 * @{
 */
#define PACKLENGTH 4                    /*!< this param defines how long a number pack is */
#define TXTPACKLENGTH 100               /*!< this param defines the maximum length of a txtpacket*/
/**
  * @}
  */ 

 /**
 *@brief The statements of variables used to be called in main
 * 
 */
#ifndef __SERIAL_GLOBAL_VARIBALS
#define __SERIAL_GLOBAL_VARIBALS
extern uint8_t Serial_TxPacket[PACKLENGTH];
extern uint8_t Serial_RxPacket[PACKLENGTH];
extern char Serial_txtRxPacket[TXTPACKLENGTH];
extern uint8_t Serial_Handling_Flag;

typedef struct {
  int Send_Pack_Mode_Value;
  int Recieve_Pack_Mode_Value;
}Serial_InitStructTypeDef;

enum NVIC_GroupConfigFlag_TypeDef {
    UN_USED = SERIAL_FALS,
    USED = !UN_USED
};

enum Send_Pack_Mode_TypeDef{
    STXT = SERIAL_FALS,
    SNUM = !STXT
};

enum Recieve_Pack_Mode_TypeDef{
    RTXT = SERIAL_FALS,
    RNUM = !RTXT
};

enum Serial_DataException_TypeDef{
    NO_EXCEPTION = 0,
    SERIAL_TXPACKET_EXCEPTION = 1,
    SERIAL_RXPACKET_EXCEPTION = 2,      /*!<This exception is thrown when the receive area is full in the meanwhile the last data isn't 0xFE */             
};

enum Recieve_Area_State_TypeDef {
    NOT_FULL = SERIAL_FALS,
    FULL=!NOT_FULL
};

extern enum NVIC_GroupConfigFlag_TypeDef NVIC_GroupConfigFlag;

#endif



/**
 *@brief The declaration of external function
 * 
 */
void Serial_Init_1(Serial_InitStructTypeDef* Serial_InitStruct);
extern void Serial_SendByte(USART_TypeDef* USARTx, uint8_t Byte);
extern void Serial_SendArray(USART_TypeDef* USARTx, uint8_t* Array);
extern void Serial_SendString(USART_TypeDef* USARTx, char* String);
extern void Serial_SendNumber(USART_TypeDef* USARTx, uint32_t Number);
extern void Serial_SendPacket(USART_TypeDef* USARTx);

#endif
