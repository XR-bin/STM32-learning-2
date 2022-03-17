#include "stm32f4xx.h"  
#include "stdio.h"
#include "nvic.h"
#include "w25q64.h"
#include "led.h"

static void delay_ms(u32 ms)
{
	u32 i = 168 / 4 * 1000 * ms;
	while(i)
	{
		i--;
	}
}

/******************************************串口中断******************************************/
/******************************************************************
*函数功能  ：串口1的中断服务函数
*函数名    ：USART1_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
*******************************************************************/
void USART1_IRQHandler(void)
{
  u8 data;
  static u32 addr = 0x0b0000;
  
  //判断是由什么形式触发中断
	//接收完成中断    SET=1   RESET=0
  if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
  {
    //执行中断任务
    data = USART_ReceiveData(USART1); //读取一个字节的数据(USART1->DR)
		W25Q64_Page_Write(addr,1,&data);
		addr++;
    LED1_ON;
  }
	//空闲中断        SET=1   RESET=0
  if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)   
  {
		//清除标志
    USART1->SR;
    USART1->DR;
    
    LED1_OFF;
  }
}

/******************************************外部中断******************************************/
/******************************************************************
*函数功能  ：外部中断1服务函数
*函数名    ：EXTI0_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
*******************************************************************/
void EXTI0_IRQHandler(void)
{
	//判断外部中断线0是否真的触发   SET=1   RESET=0
  if(EXTI_GetFlagStatus(EXTI_Line0) == SET)
  {
    delay_ms(10);
		//滤波
    if((GPIOA->IDR & (1<<0)))
    {
      EXTI->PR |= (1<<0);
      GPIOF->ODR ^= (1<<6);
      GPIOF->ODR ^= (1<<9);
      GPIOF->ODR ^= (1<<10);
      GPIOC->ODR ^= (1<<0);
    }
  }
}

/******************************************************************
*函数功能  ：软件外部中断发初始化
*函数名    ：EXTI9_5_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
             无需端口
*******************************************************************/
void EXTI9_5_IRQHandler(void)
{
	//判断外部中断线5是否真的触发   SET=1   RESET=0
  if(EXTI_GetFlagStatus(EXTI_Line5) == SET)
  {
    EXTI->PR |= (1<<5);
    GPIOF->ODR ^= (1<<6);
    GPIOF->ODR ^= (1<<9);
    GPIOF->ODR ^= (1<<10);
    GPIOC->ODR ^= (1<<0);
  }
}







