//
// Arm.h - 机械臂（App 层）
//

#ifndef SAMPLE_CAR_ARM_H
#define SAMPLE_CAR_ARM_H

#include "main.h"
#include "stdint.h"

#define ARM_STEP_DEG 10          /* 每按一次，大臂转 10 度 */

void Arm_Reset(void);            /* 4 个关节回初始姿态 */
void Arm_Up(void);               /* 大臂抬 10 度 */
void Arm_Down(void);             /* 大臂降 10 度 */

#endif //SAMPLE_CAR_ARM_H
