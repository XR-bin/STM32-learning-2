#include "sys.h"
#include "delay.h"
#include "led.h"
#include "exti.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* 2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();                         /* 延时初始化 */
    LED_Init();                             /* LED初始化 */
    EXTI0_Init();                           /* 外部中断0初始化 */

    while(1)
    {
    }
}




