//
// Arm.h - 机械臂（App 层）
//
// 关节：1 底座(PA6)  2 大臂(PA7)  3 小臂(PB0)  4 夹爪(PB1)
//

#ifndef SAMPLE_CAR_ARM_H
#define SAMPLE_CAR_ARM_H

#include "main.h"
#include "stdint.h"

#define ARM_STEP_DEG 5

void Arm_Reset(void);            /* 4 个关节回初始姿态 */

void Arm_Up(void);               /* 大臂抬 10 度（SG002 / PA7） */
void Arm_Down(void);             /* 大臂降 10 度（SG002 / PA7） */

void Arm_Left(void);             /* 底座转 10 度（SG001 / PA6） */
void Arm_Right(void);            /* 底座反向 10 度（SG001 / PA6） */

void Gripper_Open(void);         /* 夹爪开（SG004 / PB1） */
void Gripper_Close(void);        /* 夹爪合（SG004 / PB1） */

#endif //SAMPLE_CAR_ARM_H
