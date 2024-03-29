#include "wwdg.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\wwdg.c
*              .\Drivers\BSP\inc\wwdg.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-08-05
* @brief     ：窗口看门狗练习代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/* 保存WWDG计数器的设置值,默认为最大. */
uint8_t g_wwdg_cnt = 0x7f;


/**********************************************************
* @funcName ：WWDG_Init
* @brief    ：初始化窗口看门狗
* @param    ：uint8_t tr (计数器值)
* @param    ：uint8_t wr (窗口值)
* @param    ：uint8_t fprer (分频系数（WDGTB）,范围:0~7,表示2^WDGTB分频)
* @retval   ：void
* @details  ：
* @fn       ：
*           Fwwdg=PCLK1/(4096*2^fprer). 一般PCLK1=36Mhz
*           溢出时间=(4096*2^fprer)*(tr-0X3F)/PCLK3
*           假设fprer=4,tr=7f,PCLK1=36Mhz
*           则溢出时间=4096*16*64/36Mhz=116.50ms
************************************************************/
void WWDG_Init(uint8_t tr, uint8_t wr, uint8_t fprer)
{
    NVIC_InitTypeDef   NVIC_InitStruct;    /* NVIC(中断)配置结构体 */

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);  /* WWDG时钟使能 */

    g_wwdg_cnt=tr&g_wwdg_cnt;   /* 初始化WWDG_CNT */
    WWDG_SetPrescaler(fprer);   /* 设置IWDG预分频值 */
    WWDG_SetWindowValue(wr);    /* 设置窗口值 */
    WWDG_ClearFlag();           /* 清除提前唤醒中断标志位 */

    NVIC_InitStruct.NVIC_IRQChannel = WWDG_IRQn;             /* WWDG中断 */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   /* 抢占优先级1 */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;          /* 子优先级1 */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             /* IRQ通道使能 */
    NVIC_Init(&NVIC_InitStruct);                             /* WWDG中断初始化 */
    
    WWDG_EnableIT();            /* 开启窗口看门狗中断 */
    WWDG_Enable(g_wwdg_cnt);    /* 使能看门狗 ,设置 counter */
}



/**********************************************************
* @funcName ：WWDG_Set_Counter
* @brief    ：重设置WWDG计数器的值
* @param    ：uint8_t cnt (计数值)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void WWDG_Set_Counter(uint8_t cnt)
{
    WWDG_SetCounter(cnt & 0x7F);    /* 重设置7位计数器 */
}



/**********************************************************
* @funcName ：WWDG_IRQHandler
* @brief    ：窗口看门狗中断服务程序
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void WWDG_IRQHandler(void)
{  
    WWDG_Set_Counter(g_wwdg_cnt);   /* 当禁掉此句后,窗口看门狗将产生复位 */
    GPIOE->ODR ^= (1<<5);           /* LED1绿灯闪烁 */
    WWDG_ClearFlag();               /* 清除提前唤醒中断标志位 */
}





