//
// Arm.c - 机械臂（App 层）
//
// 关节：1 底座(PA6)  2 大臂(PA7)  3 小臂(PB0)  4 夹爪(PB1)
// 每个函数按一次转 10 度，并做上下限保护。
//
#include "Arm.h"

#include "sg90.h"

#define ANGLE_HOME  90
#define GRIP_HOME   45

/* 4 个关节当前角度 */
static uint8_t s_angle[4] = {ANGLE_HOME, ANGLE_HOME, ANGLE_HOME, GRIP_HOME};

/* 关节限位（度），按机械行程实测后改，别让舵机顶死 */
static const uint8_t s_min[4] = {  0,  15,  20,  20};
static const uint8_t s_max[4] = {180, 165, 160,  80};

/* 往正方向转一步（带限位） */
static uint8_t step_up(uint8_t joint)
{
    if (s_angle[joint] + ARM_STEP_DEG <= s_max[joint]) {
        s_angle[joint] += ARM_STEP_DEG;
    } else {
        s_angle[joint] = s_max[joint];
    }
    return s_angle[joint];
}

/* 往反方向转一步（带限位） */
static uint8_t step_down(uint8_t joint)
{
    if (s_angle[joint] >= (uint8_t)(s_min[joint] + ARM_STEP_DEG)) {
        s_angle[joint] -= ARM_STEP_DEG;
    } else {
        s_angle[joint] = s_min[joint];
    }
    return s_angle[joint];
}

void Arm_Reset(void)
{
    s_angle[0] = ANGLE_HOME;
    s_angle[1] = ANGLE_HOME;
    s_angle[2] = ANGLE_HOME;
    s_angle[3] = GRIP_HOME;

    SG001_SetAngle(s_angle[0]);
    SG002_SetAngle(s_angle[1]);
    SG003_SetAngle(s_angle[2]);
    SG004_SetAngle(s_angle[3]);
}

/* ---------------- 大臂（SG002 / PA7） ---------------- */

void Arm_Up(void)
{
    SG002_SetAngle(step_up(1));
}

void Arm_Down(void)
{
    SG002_SetAngle(step_down(1));
}

/* ---------------- 底座（SG001 / PA6） ---------------- */

void Arm_Left(void)
{
    SG001_SetAngle(step_up(0));
}

void Arm_Right(void)
{
    SG001_SetAngle(step_down(0));
}

/* ---------------- 夹爪（SG004 / PB1） ---------------- */

void Gripper_Open(void)
{
    SG004_SetAngle(step_up(3));
}

void Gripper_Close(void)
{
    SG004_SetAngle(step_down(3));
}
