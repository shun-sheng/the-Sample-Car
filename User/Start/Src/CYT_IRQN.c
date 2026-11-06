//
// CYT_IRQN.c - 初始化 + 串口回调 + 4 个任务
//
// 这个文件只做"调度"：收到按键，就调 App 层已经写好的函数。
//   底盘动作在 User/App/Src/Chassic.c
//   机械臂动作在 User/App/Src/Arm.c
//
// 蓝牙按键（0~9 单个数字）：
//   1 前进   2 后退   3 原地左转   4 原地右转
//   5 左移   6 右移   7 停车
//   8 机械臂抬   9 机械臂降   0 机械臂复位
//
#include "CYT_IRQN.h"

#include "cmsis_os.h"
#include "usart.h"

#include "Arm.h"
#include "Chassic.h"
#include "sg90.h"


static uint8_t g_rx_byte;

static volatile uint8_t g_move_key;     //1-9 底盘
static volatile uint8_t g_arm_key;      //089三个控制舵机



void EverythingInit(void)
{
    Chassic_Stop();
    SG90_Init();
    Arm_Reset();

    HAL_UART_Receive_IT(&huart1, &g_rx_byte, 1);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        if (g_rx_byte >= '1' && g_rx_byte <= '7') {
            g_move_key = g_rx_byte;
        }
        else  (g_rx_byte == '8' || g_rx_byte == '9' || g_rx_byte == '0') {
            g_arm_key = g_rx_byte;
        }

        HAL_UART_Receive_IT(&huart1, &g_rx_byte, 1);
    }
}



void StartDefaultTask(void const * argument)
{
    (void)argument;
    EverythingInit();
    for (;;) {
        osDelay(1000);
    }
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
            case '1':Chassic_Forward();
                break;
            case '2':Chassic_Backward();
                break;
            case '3':Chassic_TurnLeft();
                break;
            case '4':Chassic_TurnRight();
                break;
            case '5':Chassic_ShiftLeft();
                break;
            case '6':Chassic_ShiftRight();
                break;
            case '7':Chassic_Stop();
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
            case '8':Arm_Up();
                break;
            case '9':Arm_Down();
                break;
            case '0':Arm_Reset();
                break;

        }

        osDelay(20);
    }
}
