#include "sys.h"
#include "delay.h"
#include "usart.h"

int main(void)
{
    uint8_t str[200];

    SysTick_Init();                         /* 延时初始化 */
    USART1_Init(115200);                    /* 串口1初始化   115200 */

    while(1)
    {
        USART1_Receive_Str(str);
        USART1_Send_Str(str);
        printf("\r\n");
        printf("%s\r\n",str);
    }
}




