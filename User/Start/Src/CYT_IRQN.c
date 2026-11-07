//
// CYT_IRQN.c - 初始化 + 串口回调 + 4 个任务
//
// 这个文件只做"调度"：收到按键，就调 App 层已经写好的函数。
//   底盘动作在 User/App/Src/Chassic.c
//   机械臂动作在 User/App/Src/Arm.c
//
// 蓝牙按键（单字符）：
//   1 前进   2 后退   3 原地左转   4 原地右转
//   5 左移   6 右移   7 停车
//   8 大臂抬   9 大臂降       0 机械臂回初始姿态
//   A 底座左转  B 底座右转    C 夹爪开   D 夹爪合
//
// 注意：APP 文本框里填 "8"，串口收到的就是 0x38（字符 '8'），
//       不能写 '38'（多字符常量，值不一样！）
//
#include "CYT_IRQN.h"

#include "cmsis_os.h"
#include "usart.h"

#include "Arm.h"
#include "Chassic.h"
#include "sg90.h"

/* 舵机自检：改成 1 后重新烧录，不用蓝牙，上电就自动扫角
 *   用来单独判断"舵机本体能转多大"：
 *     拆掉机械臂/连杆扫角 -> 能 0~180 转满 = 舵机好的，问题在机械负载
 *     转不满             -> 舵机本体/供电/信号问题
 *   扫的是 SG001(PA6) 和 SG002(PA7) 两个通道，看哪个是你的大臂即可 */
#define SERVO_SWEEP_TEST 0

#define SERVO_SWEEP_CH1  1      /* 1 = 一起扫 SG001+SG002 */
#define SERVO_SWEEP_STEP 10     /* 每步 10 度 */
#define SERVO_SWEEP_MS   300    /* 每步停 300ms */

static uint8_t g_rx_byte;

static volatile uint8_t g_move_key;     //  底盘
static volatile uint8_t g_arm_key;      //  机械臂



static uint8_t Key_Normalize(uint8_t byte)
{
    if (byte >= '0' && byte <= '9') {
        return byte;
    }
    if (byte >= 0x41 && byte <= 0x5a) {
        return byte;
    }
    if (byte >= 'a' && byte <= 'z') {
        return (uint8_t)(byte - 'a' + 'A');
    }
    if (byte <= 9) {
        return (uint8_t)('0' + byte);
    }
    return 0;
}


void EverythingInit(void)
{
    Chassic_Stop();
    SG90_Init();
    Arm_Reset();

    HAL_UART_Receive_IT(&huart1, &g_rx_byte, 1);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint8_t key;

    if (huart->Instance == USART1) {
        key = Key_Normalize(g_rx_byte);

        if (key >= '0' && key <= '9') {
            g_move_key = key;
        } else if (key == 'a' || key == 'b' || key == 'c' ||
                   key == 'd' || key == 'e' || key == 'f' ) {
            g_arm_key = key;
        } else {

        }

        HAL_UART_Receive_IT(&huart1, &g_rx_byte, 1);
    }
}


void StartDefaultTask(void const * argument)
{
    (void)argument;

#if SERVO_SWEEP_TEST
    /* 舵机自检：SG001 + SG002 一起慢慢扫 0 -> 180 -> 0 */
    for (;;) {
        uint8_t a;

        for (a = 0; a <= 180; a += SERVO_SWEEP_STEP) {
            SG001_SetAngle(a);
            SG002_SetAngle(a);
            osDelay(SERVO_SWEEP_MS);
        }
        for (a = 180; a >= SERVO_SWEEP_STEP; a -= SERVO_SWEEP_STEP) {
            SG001_SetAngle(a);
            SG002_SetAngle(a);
            osDelay(SERVO_SWEEP_MS);
        }
    }
#else
    for (;;) {
        osDelay(1000);
    }
#endif
}

/* 蓝牙任务：按键已经在中断里分好了，这里暂时用不到 */
void StartBlueToothTask(void const * argument)
{
    (void)argument;

    for (;;) {
        osDelay(1000);
    }
}

/* 底盘任务：处理 1~7 */
void StartMoveTask(void const * argument)
{
    uint8_t key;

    (void)argument;

    for (;;) {
        key = g_move_key;
        g_move_key = 0;

        switch (key) {
        case '1': Chassic_Forward();    break;
        case '2': Chassic_Backward();   break;
        case '3': Chassic_TurnLeft();   break;
        case '4': Chassic_TurnRight();  break;
        case '0':Chassic_Stop();        break;
        default:                        break;
        }

        osDelay(10);
    }
}

/* 机械臂任务：处理 8 / 9 / 0 */
void StartArmTask(void const * argument)
{
    uint8_t key;

    (void)argument;

    for (;;) {
        key = g_arm_key;
        g_arm_key = 0;

        switch (key) {
        case 'a': Arm_Up();    break;
        case 'b': Arm_Down();  break;
        case 'c': Arm_Left();  break;
        case 'd': Arm_Right(); break;
        case 'e': Gripper_Open();break;
        case 'f': Gripper_Close(); break;
        case 'g': Arm_Reset(); break;
        default:               break;
        }

        osDelay(20);
    }
}
