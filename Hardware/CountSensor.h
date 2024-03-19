/**
 *@file CountSensor.h
 * @author Alvari
 * @brief 
 * @version 0.1
 * @date 2023-08-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __COUNT_SENSOR_H
#define __COUNT_SENSOR_H

#include "stm32f10x.h"

void CountSensor_Init(void);
uint16_t CountSensor_Get(void);

#endif
