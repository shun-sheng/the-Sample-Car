//
// Created by lenovo on 2026/11/6.
//

#ifndef SAMPLE_CAR_CHASSIC_H
#define SAMPLE_CAR_CHASSIC_H

#include "main.h"
#include "stdint.h"

void Chassic_Forward(void);
void Chassic_Backward(void);
void Chassic_TurnLeft(void);
void Chassic_TurnRight(void);
void Chassic_ShiftLeft(void);
void Chassic_ShiftRight(void);
void Chassic_Stop(void);        /* 新增：停车 */

#endif //SAMPLE_CAR_CHASSIC_H
