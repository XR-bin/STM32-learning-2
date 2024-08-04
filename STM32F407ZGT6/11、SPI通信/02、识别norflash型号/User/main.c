#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "norflash.h"

int main(void)
{
    SysTick_Init();                         /* 延时初始化 */
    USART1_Init(115200);                    /* 串口初始化  115200 */
    NorFlash_Init();                        /* W25Q128初始化 */

    NorFlash_Read_ID();

    while(1)
    {
    }
}




