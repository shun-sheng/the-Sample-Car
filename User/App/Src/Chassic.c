//
// Created by lenovo on 2026/11/6.
//
#include "Chassic.h"

#include "Motor.h"

void Chassic_Forward(void) {
    Motor1_Forward();
    Motor2_Forward();
    Motor3_Forward();
    Motor4_Forward();
}

void Chassic_Backward(void) {
    Motor1_Backward();
    Motor2_Backward();
    Motor3_Backward();
    Motor4_Backward();
}

void Chassic_TurnLeft(void) {
    Motor1_Forward();
    Motor2_Backward();
    Motor3_Backward();
    Motor4_Forward();
}

void Chassic_TurnRight(void) {
    Motor1_Backward();
    Motor2_Forward();
    Motor3_Forward();
    Motor4_Backward();
}

void Chassic_ShiftLeft(void) {
    Motor1_Forward();
    Motor2_Backward();
    Motor3_Forward();
    Motor4_Backward();
}

void Chassic_ShiftRight(void) {
    Motor1_Backward();
    Motor2_Forward();
    Motor3_Backward();
    Motor4_Forward();
}

void Chassic_Stop(void) {
    Motor_StopAll();
}
