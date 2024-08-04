#include "iic.h"



/**********************************************************
* @funcName ：IIC_Init
* @brief    ：对模拟IIC所需要的GPIO口进行初始化设置
* @param    ：void
* @retval   ：void
* @details  ：
*            时钟线  IIC_SCL  PB8   推挽输出
*            数据线  IIC_SDA  PB9   开漏输出
*                 高电平处于空闲状态
* @fn       ：
************************************************************/
void IIC_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;       /* GPIOx配置结构体 */

    /* 时钟使能  GPIOB */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* GPIOx端口配置 */
    /* PB8 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;           /* 选择PB8 */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;        /* 普通输出 */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;        /* 推挽输出 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;    /* 速度为100MHz */
    GPIO_Init(GPIOB, &GPIO_InitStruct);                /* 根据设定参数初始化PB8 */
    /* PB9 */
    /* 因为配置一样，所以一些变量无需改变 */
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;           /* 选择PB9 */
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;        /* 开漏输出 */
    GPIO_Init(GPIOB, &GPIO_InitStruct);                /* 根据设定参数初始化PB9 */

    /* 先将时钟线和数据线设置空闲 */
    GPIO_SetBits(GPIOB,GPIO_Pin_8);                    /* 时钟线拉高 */
    GPIO_SetBits(GPIOB,GPIO_Pin_9);                    /* 数据线拉高 */
}



/**********************************************************
* @funcName ：IIC_Start
* @brief    ：模拟IIC的起始信号
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
*            SCL_H         时钟线拉高
*            SCL_L         时钟线拉低
*            SDA_OUT_H     输出模式下数据线拉高
*            SDA_OUT_L     输出模式下数据线拉低
************************************************************/
void IIC_Start(void)
{
    SCL_L;         /* 时钟线拉低 */
    SDA_OUT_H;     /* 数据线拉高 */
    SCL_H;         /* 时钟线拉高 */
    delay_us(5);   /* 延时5us */
    SDA_OUT_L;     /* 数据线拉低 */
    delay_us(5);   /* 延时5us */
    SCL_L;         /* 时钟线拉低0 */
}



/**********************************************************
* @funcName ：IIC_Stop
* @brief    ：模拟IIC的停止信号
* @param    ：void
* @retval   ：void
* @details  ：
* @fn       ：
*            SCL_H         时钟线拉高
*            SCL_L         时钟线拉低
*            SDA_OUT_H     输出模式下数据线拉高
*            SDA_OUT_L     输出模式下数据线拉低
************************************************************/
void IIC_Stop(void)
{
    SCL_L;        /* 时钟线拉低 */
    SDA_OUT_L;    /* 数据线拉低 */
    SCL_H;        /* 时钟线拉高 */
    delay_us(5);  /* 延时5us */
    SDA_OUT_H;    /* 数据线拉高 */
    delay_us(5);  /* 延时5us */
}



/**********************************************************
* @funcName ：IIC_Send_Ack
* @brief    ：模拟IIC的主机发送应答信号
* @param    ：uint8_t ack
*   @arg    ：0 --- 应答   1 --- 不应答
* @retval   ：void
* @details  ：
* @fn       ：
*            SCL_H         时钟线拉高
*            SCL_L         时钟线拉低
*            SDA_OUT_H     输出模式下数据线拉高
*            SDA_OUT_L     输出模式下数据线拉低
************************************************************/
void IIC_Send_Ack(uint8_t ack)
{
    SCL_L;                /* 时钟线拉低 */
    delay_us(5);          /* 延时5us */
    /*判断是否要发送应答信号*/
    if(ack == 1)          /* 发送不应答信号 */
    {
        SDA_OUT_H;        /* 数据线拉高 */
        delay_us(1);      /* 延时1us */
        SCL_H;            /* 时钟线拉高 */
    }
    else if(ack == 0)     /* 发送应答信号 */
    {
        SDA_OUT_L;        /* 数据线拉低 */
        delay_us(1);      /* 延时1us */
        SCL_H;            /* 时钟线拉高 */
    }

    delay_us(5);          /* 延时5us */
    SCL_L;                /* 时钟线拉低 */
}



/**********************************************************
* @funcName ：IIC_Receive_Ack
* @brief    ：模拟IIC的主机接收应答信号
* @param    ：void
* @retval   ：uint8_t  ---  (0-应答   1-不应答)
* @details  ：
* @fn       ：
*            SCL_H         时钟线拉高
*            SCL_L         时钟线拉低
*            SDA_OUT_H     输出模式下数据线拉高
*            SDA_OUT_L     输出模式下数据线拉低
*            SDA_IN        输入模式
*            SDA_INT       接收到的值
************************************************************/
uint8_t IIC_Receive_Ack(void)
{
    SCL_L;         /* 时钟线拉低 */
    SDA_IN;        /* 进入输入模式 */
    delay_us(5);   /* 延时5us */
    SCL_H;         /* 时钟线拉高 */

    /* 判断从机发送的应答信号 */
    /* 从机不应答 */
    if(SDA_INT)
    {
        SCL_L;      /* 时钟线拉低 */
        return 1;
    }
    /* 从机应答 */
    /* 时钟线拉低 */
    SCL_L;
    return 0;
}



/**********************************************************
* @funcName ：IIC_Send_Byte
* @brief    ：模拟IIC的主机发送一个字节的数据
* @param    ：uint8_t data
* @retval   ：void
* @details  ：
* @fn       ：
*            SCL_H         时钟线拉高
*            SCL_L         时钟线拉低
*            SDA_OUT_H     输出模式下数据线拉高
*            SDA_OUT_L     输出模式下数据线拉低
************************************************************/
void IIC_Send_Byte(uint8_t data)
{
    uint8_t i;

    for(i=0;i<8;i++)
    {
        SCL_L;             /* 时钟线拉低 */
        delay_us(5);       /* 延时5us */

        /*判断发送是0还是1*/
        if(data & 0x80)    /* 发送1 */
        {
            SDA_OUT_H;     /* 数据线拉高 */
        }
        else               /* 发送0 */
        {
            SDA_OUT_L;     /* 数据线拉低 */
        }

        delay_us(1);       /* 延时1us */
        SCL_H;             /* 时钟线拉高 */
        delay_us(5);       /* 延时5us */
        
        data <<= 1;        /* data左移一位 */
    }

    SCL_L;                 /* 时钟线拉低 */
}



/**********************************************************
* @funcName ：IIC_Receive_Byte
* @brief    ：模拟IIC的主机接收一个字节的数据
* @param    ：void
* @retval   ：uint8_t
* @details  ：
* @fn       ：
*            SCL_H         时钟线拉高
*            SCL_L         时钟线拉低
*            SDA_OUT_H     输出模式下数据线拉高
*            SDA_OUT_L     输出模式下数据线拉低
*            SDA_IN        输入模式
*            SDA_INT       接收到的值
************************************************************/
uint8_t IIC_Receive_Byte(void)
{
    uint8_t i;
    uint8_t data = 0;

    SCL_L;               /* 拉低时钟线 */
    SDA_IN;              /* 进入接收模式 */

    for(i=0;i<8;i++)
    {
        SCL_L;           /* 时钟线拉低 */

        data <<= 1;      /* data左移一位 */
        delay_us(5);     /* 延时5us */
        SCL_H;           /* 时钟线拉高 */

        /* 判断接收的是1还是0 */ 
        /* 接收的是1 */
        if(SDA_INT)
        {
            data |= 0x1;
        }
        /* 接收的是0 不做处理 */
    }

    SCL_L;                /* 时钟线拉低 */

    return data;
}



