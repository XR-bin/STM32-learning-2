#include "sys.h"
#include "delay.h"
#include "led.h"

int main(void)
{
    SysTick_Init();                         /* 延时初始化 */
    LED_Init();                             /* LED初始化 */

    while(1)
    {
    }
}




