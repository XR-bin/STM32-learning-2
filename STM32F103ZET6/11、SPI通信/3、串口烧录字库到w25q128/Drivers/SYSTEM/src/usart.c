#include "usart.h"
#include "w25q128.h"
#include "led.h"



/**********************************************************
* @funcName ：USART1_Init
* @brief    ：对USART1对应的GPIO口进行初始化设置
* @param    ：uint32_t baud
* @retval   ：void
* @details  ：
*             PA9     TX     ---------输出
*             PA10    RX     ---------输入
* @fn       ：
************************************************************/
void USART1_Init(uint32_t baud)
{
    GPIO_InitTypeDef  GPIO_InitStruct;     /* GPIOx配置结构体 */
    USART_InitTypeDef USART_InitStruct;    /* USARTx配置结构体 */
    NVIC_InitTypeDef   NVIC_InitStruct;    /* NVIC(中断)配置结构体 */

    /* 时钟使能   GPIOA   USART1 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE); 

    /* GPIOx初始化设置 */
    /* GPIOx端口配置 */
    /* PA9   USART1_TX ----- 复用串口输出 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;             /* PA9 端口配置 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;        /* 复用推挽输 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;       /* IO口速度为50MHz */
    GPIO_Init(GPIOA, &GPIO_InitStruct);                  /* 根据设定参数初始化PA9 */
    /* PA10  USART1_RX ----- 复用串口输入 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;            /* PA10 端口配置 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  /* 浮空输入 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;       /* IO口速度为50MHz */
    GPIO_Init(GPIOA, &GPIO_InitStruct);                  /* 根据设定参数初始化PA10 */

    /* USART 初始化设置 */
    /* USART1配置 */
    USART_InitStruct.USART_BaudRate      = baud;                 /* 波特率设置(USART->BRR寄存器) */
    USART_InitStruct.USART_WordLength    = USART_WordLength_8b;  /* 字长为8位数据格式:一个起始位， 8个数据位， n个停止位；(USART->CR1寄存器的第12位) */
    USART_InitStruct.USART_StopBits      = USART_StopBits_1;     /* 一个停止位(USART->CR2寄存器的第12、13位) */
    USART_InitStruct.USART_Parity        = USART_Parity_No;      /* 无奇偶校验(USART->CR1寄存器的第10位) */
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;/* 无硬件数据流控制(USART->CR3寄存器的第9位) */
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; /* 收发模式(USART->CR1寄存器的第2、3位) */
    USART_Init(USART1,&USART_InitStruct);                        /* 初始化USART1 */

    /* NVIC配置 */
    /* 选择中断方式并使能该方式中断 */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);     /* 读取数据寄存器不为空(接收完成)中断 (USART->CR1的第5位) */
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);     /* 检测到空闲线路(空闲)中断 (USART->CR1的第4位) */

    /* 串口中断配置 */
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;           /* 选择串口1中断 */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   /* 抢占优先级1 */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;          /* 子优先级1 */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             /* IRQ通道使能 */
    NVIC_Init(&NVIC_InitStruct);                             /* 串口1中断初始化 */

    /* 使能串口1 */
    USART_Cmd(USART1, ENABLE);
}



/**********************************************************
* @funcName ：USART1_Send_Byte
* @brief    ：串口1发送一个字节的数据（uint8_t）
* @param    ：uint8_t data
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void USART1_Send_Byte(uint8_t data)
{
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET); /* 等待发送结束(USART1->SR的第6位) */
    USART_SendData(USART1, data);                            /* 发送一个字节的数据(USART1->DR) */
}



/**********************************************************
* @funcName ：USART1_Send_Str
* @brief    ：串口1发送一个字符串
* @param    ：uint8_t *str
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void USART1_Send_Str(uint8_t *str)
{
    while(*str != '\0')
    {
        USART1_Send_Byte(*str);
        str++;
    }
}



/**********************************************************
* @funcName ：USART1_Receive_Byte
* @brief    ：串口1接收一个字节的数据（uint8_t）
* @param    ：void
* @retval   ：uint8_t
* @details  ：
* @fn       ：
************************************************************/
uint8_t USART1_Receive_Byte(void)
{
    uint8_t data;

    while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)!=SET){}; /* 等待发送结束(USART1->SR的第5位) */
    data = USART_ReceiveData(USART1);                          /* 读取一个字节的数据(USART1->DR) */

    return data;
}



/**********************************************************
* @funcName ：USART1_Receive_Str
* @brief    ：串口1接收一个字符串
* @param    ：uint8_t *str
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void USART1_Receive_Str(uint8_t *str)
{
    while(1)
    {
        *str = USART1_Receive_Byte();
        if(*str == '\r')
        {
            *str = USART1_Receive_Byte();
            if(*str == '\n') break;
        }
        str++;
    }
    *str = '\0';
}



/**********************************************************
* @funcName ：USART1_IRQHandler
* @brief    ：串口1中断
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void USART1_IRQHandler(void)
{
    uint8_t data;
    static uint32_t addr = 0x0b0000;

    /* 判断中断触发的形式 */
    /* 接收完成中断 */
    if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
    {
        /* 中断要执行的任务 */
        LED0_ON;
        /* 字库数据写入W25Q128 */
        data = USART1->DR;
        W25Q128_Page_Write(addr,1,&data);
        addr++;

        /* 先读USART1->SR，再改USART1->DR，可以清除中断标志 */
        /* 或者直接修改USART1->SR，从而清除中断标志 */
        /* 我这里两种方式都用 */
        USART1->SR &= ~(1<<5);
    }
    if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
    {
        LED0_OFF;

        /* 先读USART1->SR，再改USART1->DR，可以清除中断标志 */
        /* 或者直接修改USART1->SR，从而清除中断标志 */
        /* 我这里两种方式都用 */
        USART1->SR &= ~(1<<4);
        USART1->DR;
    }
}
























/**********************************printf函数*****************************************/
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle; 
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while((USART1->SR&0X40)==0);//循环发送,直到发送完毕
    USART1->DR = (u8) ch;
    return ch;
}
#endif
/***********************************************************************************/


