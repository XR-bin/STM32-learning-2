#include "sys.h"  
#include "stdio.h"
#include "nvic.h"
#include "delay.h"
#include "led.h"
#include "w25q128.h"

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
    //中断要执行的任务
    data = USART1->DR;
    W25Q128_Page_Write(addr,1,&data);
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
*函数功能  ：外部中断4服务函数
*函数名    ：EXTI0_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
*******************************************************************/
void EXTI4_IRQHandler(void)
{
	//判断外部中断线0是否真的触发   SET=1   RESET=0
  if(EXTI_GetFlagStatus(EXTI_Line4) == SET)
  {
    delay_ms(10);
		//滤波
    if(!(GPIOE->IDR & (1<<4)))
    {
      EXTI->PR |= (1<<4);
      GPIOB->ODR ^= (1<<5);
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
		GPIOB->ODR ^= (1<<5);
  }
}

/******************************************ADC中断******************************************/
/******************************************************************
*函数功能  ：ADC3中断服务函数
*函数名    ：ADC3_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
*******************************************************************/
void ADC3_IRQHandler(void)
{
	u16 data;
	
	//是否转换完成
	if(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == SET)
	{
		//读取转换后的数据(ADC_DR寄存器)
		data = ADC_GetConversionValue(ADC3);
		printf("data:%d\r\n",data);
	}
	//清除标志
	ADC_ClearITPendingBit(ADC3,ADC_IT_EOC);
	//开启规则通道转换开关(ADC_CR2寄存器的22位)
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);	
}



/******************************************定时器中断******************************************/
/******************************************
*函数功能  ：定时器6的中断服务函数
*函数名    ：TIM6_DAC_IRQHandler ：
*函数参数  ：无
*函数返回值：无
*函数描述  ：
*********************************************/
void TIM6_IRQHandler(void)
{
	//检查TIM6更新中断发生与否
  if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
  { 
    //中断事件
    GPIOE->ODR ^= (1<<5);
  }
	//清除TIMx更新中断标志 
  TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
}
