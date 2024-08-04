#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  /* 2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();               /* 延时初始化 */
    USART1_Init(115200);          /* 串口1初始化 115200 */
    ADC3_In6_Init();              /* ADC3通道6初始化 */

    while(1)
    {
        /* 每收到串口1的数据就转换一次ADC数据 */
        USART1_Receive_Byte();
        /* 开启规则通道转换开关(ADC_CR2寄存器的22位) */
        ADC_SoftwareStartConvCmd(ADC3, ENABLE);
    }
}


