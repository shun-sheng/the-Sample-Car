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

#define BT_DISCONNECTED 0
#define BT_SILENT 1
#define BT_CONNECTED 2

#define BT_DELAY_MS 20
#define BT_TIMEOUT_MS 500
uint8_t g_bl_data=BT_DISCONNECTED;
uint32_t g_bl_time;

static uint8_t g_rx_byte;

static volatile uint8_t g_move_key;     //  底盘
static volatile uint8_t g_arm_key;      //  机械臂



static uint8_t Key_Normalize(uint8_t byte)
{
    if (byte >= '0' && byte <= '9') {
        return byte;
    }
    if (byte >= 'a' && byte <= 'z') {
        return byte;
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
    if (g_bl_data!=BT_CONNECTED) {
        HAL_UART_Receive_IT(&huart1, &g_rx_byte, 1);
        return;
    }
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

    for (;;) {
        osDelay(1000);
    }

}


void StartBlueToothTask(void const * argument)
{
    (void)argument;

    for (;;) {
        uint8_t state;

        state=(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11)==GPIO_PIN_SET);

        switch (g_bl_data) {
            case BT_DISCONNECTED: {
                if (state) {
                    g_move_key=0;
                    g_arm_key=0;
                    g_bl_time=0;
                    Chassic_Stop();
                    g_bl_data=BT_SILENT;
                };
            }
                break;
            case BT_SILENT: {
                if (!state) {
                    g_move_key=0;
                    g_arm_key=0;
                    g_bl_time=0;
                    Chassic_Stop();
                    g_bl_data=BT_DISCONNECTED;
                }
                g_bl_time+=BT_DELAY_MS;
                if (g_bl_time>=BT_TIMEOUT_MS) {
                    g_bl_data=BT_CONNECTED;
                }

            }
            break;

            case BT_CONNECTED: {
                if (!state) {
                    g_move_key=0;
                    g_arm_key=0;
                    g_bl_time=0;
                    Chassic_Stop();
                    g_bl_data=BT_DISCONNECTED;
                }
            }
            break;

            default: {
                g_bl_data=BT_DISCONNECTED;
            }
        }
            osDelay(BT_DELAY_MS);
        }
        // osDelay(1000);
}



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
