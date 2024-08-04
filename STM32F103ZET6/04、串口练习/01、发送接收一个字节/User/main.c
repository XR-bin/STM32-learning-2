#include "sys.h"
#include "delay.h"
#include "usart.h"



int main(void)
{
    uint8_t data;

    SysTick_Init();           /* 延时初始化 */
    USART1_Init(115200);      /* 串口1初始化  115200 */

    while(1)
    {
        data = USART1_Receive_Byte();
        USART1_Send_Byte(data);
    }
}


