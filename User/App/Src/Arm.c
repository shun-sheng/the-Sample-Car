//
// Arm.c - 机械臂（App 层）
//
// 关节：1 底座(PA6)  2 大臂(PA7)  3 小臂(PB0)  4 夹爪(PB1)
// 现在只做"大臂抬/降 + 全部复位"，想控制别的关节照着加函数即可。
//
#include "Arm.h"

#include "sg90.h"

/* 4 个关节的初始角度，按你的机械结构调整 */
static uint8_t s_angle[4] = {90, 90, 90, 45};

/* 关节限位（度），按机械行程实测后改，别让舵机顶死 */
static const uint8_t s_min[4] = {  0,  15,  20,  20};
static const uint8_t s_max[4] = {180, 165, 160,  80};

void Arm_Reset(void)
{
    s_angle[0] = 90;
    s_angle[1] = 90;
    s_angle[2] = 90;
    s_angle[3] = 45;

    SG001_SetAngle(s_angle[0]);
    SG002_SetAngle(s_angle[1]);
    SG003_SetAngle(s_angle[2]);
    SG004_SetAngle(s_angle[3]);
}

void Arm_Up(void)
{
    if (s_angle[1] + ARM_STEP_DEG <= s_max[1]) {
        s_angle[1] += ARM_STEP_DEG;
    } else {
        s_angle[1] = s_max[1];
    }
    SG002_SetAngle(s_angle[1]);
}

void Arm_Down(void)
{
    if (s_angle[1] >= (uint8_t)(s_min[1] + ARM_STEP_DEG)) {
        s_angle[1] -= ARM_STEP_DEG;
    } else {
        s_angle[1] = s_min[1];
    }
    SG002_SetAngle(s_angle[1]);
}
