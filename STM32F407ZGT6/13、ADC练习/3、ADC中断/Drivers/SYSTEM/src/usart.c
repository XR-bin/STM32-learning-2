#include "usart.h"



/**********************************************************
* @funcName ：USART1_Init
* @brief    ：对USART1对应的GPIO口进行初始化设置
* @param    ：uint32_t baud (波特率)
* @retval   ：void
* @details  ：
*             PA9     TX     ---------输出
*             PA10    RX     ---------输入
* @fn       ：
************************************************************/
void USART1_Init(uint32_t baud)
{
    GPIO_InitTypeDef   GPIO_InitStruct;    /* GPIOx配置结构体 */
    USART_InitTypeDef  USART_InitStruct;   /* USART配置结构体 */
//    NVIC_InitTypeDef   NVIC_InitStruct;   /* NVIC(中断)配置结构体 */

    /* 时钟使能   GPIOA   USART1 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);   /* 使能GPIOA时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);  /* 使能USART1时钟 */

    /* GPIO口配置 */
    /* PA9、PA10 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10; /* 选择PA9、PA10 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;             /* 复用功能 */
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;             /* 上拉 */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;            /* 推挽输出 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;        /* 速度100MHz */
    GPIO_Init(GPIOA,&GPIO_InitStruct);                     /* 根据设定参数初始化PA9，PA10 */

    /* 串口1对应引脚复用映射 */
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);  /* PA9 复用为USART1的输出(TX) */
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); /* PA10复用为USART1的输入(RX) */

    /* 串口初始化 */
    /* USART1 */
    USART_InitStruct.USART_BaudRate            = baud;                           /* 波特率设置(USART->BRR寄存器) */
    USART_InitStruct.USART_WordLength          = USART_WordLength_8b;            /* 字长为8位数据格式(USART->CR1寄存器的第12位) */
    USART_InitStruct.USART_StopBits            = USART_StopBits_1;               /* 一个停止位(USART->CR2寄存器的第12、13位) */
    USART_InitStruct.USART_Parity              = USART_Parity_No;                /* 无奇偶校验位(USART->CR1寄存器的第10位) */
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /* 无硬件数据流控制(USART->CR3寄存器的第9位) */
    USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  /* 收发模式(USART->CR1寄存器的第2、3位) */
    USART_Init(USART1, &USART_InitStruct);                                       /* 初始化串口1 */

    /* 中断配置 */
    /* 选择中断方式并使能该方式中断 */
//    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);     /* 读取数据寄存器不为空(接收完成)中断 (USART->CR1的第5位) */
//    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);     /* 检测到空闲线路(空闲)中断 (USART->CR1的第4位) */

    /* 串口1中断配置-----NVIC 配置 */
//    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;         /* 串口1中断通道 */
//    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 1;  /* 抢占优先级1 */
//    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;        /* 子优先级1 */
//    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;           /* IRQ通道使能 */
//    NVIC_Init(&NVIC_InitStruct);                           /* 根据指定的参数初始化VIC寄存器*/

    /* 串口使能 */
    USART_Cmd(USART1, ENABLE);                             /* 使能串口1 */
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

    while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) != SET );    /* 等待发送结束(USART1->SR的第5位) */
    data = USART_ReceiveData(USART1);                              /* 读取一个字节的数据(USART1->DR) */

    return data;
}






















/******************************************************************************************/
/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */

#if 1
#if (__ARMCC_VERSION >= 6010050)            /* 使用AC6编译器时 */
__asm(".global __use_no_semihosting\n\t");  /* 声明不使用半主机模式 */
__asm(".global __ARM_use_no_argv \n\t");    /* AC6下需要声明main函数为无参数格式，否则部分例程可能出现半主机模式 */

#else
/* 使用AC5编译器时, 要在这里定义__FILE 和 不使用半主机模式 */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* 不使用半主机模式，至少需要重定义_ttywrch\_sys_exit\_sys_command_string函数,以同时兼容AC6和AC5模式 */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}

/* FILE 在 stdio.h里面定义. */
FILE __stdout;

/* 重定义fputc函数, printf函数最终会通过调用fputc输出字符串到串口 */
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0);     /* 等待上一个字符发送完成 */

    USART1->DR = (uint8_t)ch;             /* 将要发送的字符 ch 写入到DR寄存器 */
    return ch;
}
#endif
/******************************************************************************************/





