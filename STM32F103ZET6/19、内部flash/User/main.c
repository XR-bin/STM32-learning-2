#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "stmflash.h"

/* 设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小 + 0X08000000) */
#define FLASH_SAVE_ADDR 0X08070000

int main(void)
{
    int i;
    uint16_t buf1[10]={0,1,2,3,4,5,6,7,8,9};
    uint16_t buf2[10]={0,0,0,0,0,0,0,0,0,0};

    SysTick_Init();       /* 延时初始化 */
    USART1_Init(115200);  /* 串口1初始化 115200 */
    LED_Init();           /* LED初始化 */

    STMFLASH_Write(FLASH_SAVE_ADDR, buf1, 10);
    delay_ms(100);
    STMFLASH_Read(FLASH_SAVE_ADDR, buf2, 10);

    for(i=0; i<10; i++) printf("%d\r\n", buf2[i]);

    while(1)
    {
        LED0_ON;
        LED1_ON;
        delay_ms(300);
        LED0_OFF;
        LED1_OFF;
        delay_ms(300);
    }
}


