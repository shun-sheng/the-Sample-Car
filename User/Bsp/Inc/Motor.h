//
// Motor.h - 4 个电机（2 块 L298N）方向控制
//
//   Motor1: PA8 / PB15      Motor2: PA0 / PC15
//   Motor3: PA1 / PA2       Motor4: PB13 / PB14
//   Forward  = IN1 高 IN2 低
//   Backward = IN1 低 IN2 高
//   Stop     = 两路都低（滑行停机）
//

#ifndef SAMPLE_CAR_MOTOR_H
#define SAMPLE_CAR_MOTOR_H
#include "main.h"
#include "stdint.h"

void Motor1_Forward(void);
void Motor1_Backward(void);
void Motor1_Stop(void);

void Motor2_Forward(void);
void Motor2_Backward(void);
void Motor2_Stop(void);

void Motor3_Forward(void);
void Motor3_Backward(void);
void Motor3_Stop(void);

void Motor4_Forward(void);
void Motor4_Backward(void);
void Motor4_Stop(void);

void Motor_StopAll(void);

#endif //SAMPLE_CAR_MOTOR_H
