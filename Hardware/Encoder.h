/**
 *@file Encoder.h
 * @author Alvari
 * @brief 
 * @version 0.1
 * @date 2023-08-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"

void Encoder_Init(void);
int16_t Encoder_Get(void);

#endif
