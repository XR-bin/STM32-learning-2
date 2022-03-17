#include "stm32f4xx.h"  
#include "stdio.h"
#include "nvic.h"

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
  static u8 i = 0;
  static u8 str[200];
  
  //判断是由什么形式触发中断
	//接收完成中断    SET=1   RESET=0
  if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
  {
    //执行中断任务
    str[i] = USART_ReceiveData(USART1); //读取一个字节的数据(USART1->DR)
    i++;
  }
	//空闲中断        SET=1   RESET=0
  if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)   
  {
		//清除标志
    USART1->SR;
    USART1->DR;
    
    str[i] = '\0';
    printf("你发送的数据是：%s\r\n",str);
    i = 0;
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


/******************************************ADC******************************************/
/******************************************************************
*函数功能  ：ADC中断服务函数
*函数名    ：ADC_IRQHandler
*函数参数  ：void
*函数返回值：void
*描述      ：
*******************************************************************/
void ADC_IRQHandler(void)
{
  u16 data;
  if(ADC_GetITStatus(ADC1,ADC_IT_EOC) == SET)
  { 
    //读取转换后的数据(ADC1->DR)
		data = ADC_GetConversionValue(ADC1);
    printf("data:%d\r\n",data);
  }
	//清除标志
	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
  //开启规则通道转换开关(ADC1->CR2寄存器的第30位)
	ADC_SoftwareStartConv(ADC1);
}

/******************************************TIMx定时器******************************************/
/******************************************
*函数功能  ：定时器6的中断服务函数
*函数名    ：TIM6_DAC_IRQHandler ：
*函数参数  ：无
*函数返回值：无
*函数描述  ：
*********************************************/
void TIM6_DAC_IRQHandler(void)
{
	//判断是否定时器6中断
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET)
	{
		//清除中断标志位
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update); 
		//紧急事件
		GPIOF->ODR ^= ((1<<6)|(1<<9)|(1<<10));
	}
}

/******************************************
*函数功能  ：定时器6的中断服务函数
*函数名    ：TIM6_DAC_IRQHandler ：
*函数参数  ：无
*函数返回值：无
*函数描述  ：
*********************************************/
void TIM7_IRQHandler(void)
{
	static u8 temp = 0;
	static u16 pwm = 500;
	
	//判断是否定时器7中断
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET)
	{
		//清除中断标志位
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update); 
		//紧急事件
		if(temp == 0)
    {
      if(pwm == 0) temp = 1;
      else pwm -=50;
			TIM_SetCompare1(TIM10,pwm);
    }
    if(temp == 1)
    {
      if(pwm == 1000) temp = 0;
      else pwm +=50;
			TIM_SetCompare1(TIM10,pwm);
    }
	}
}

/******************************************
*函数功能  ：定时器5的中断服务函数
*函数名    ：TIM5_IRQHandler
*函数参数  ：无
*函数返回值：无
*函数描述  ：
*********************************************/
void TIM5_IRQHandler(void)
{
  static u16 update_num;
  static u8 update_flay = 0;
  static u16 rising_edge; 
  u16 falling_edge;
  u16 cont; 
  u16 t;
	//更新中断  
	if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET)
	{
		//清中断标志位
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
		//紧急事件
    if(update_flay)
    {
      update_num++;
    }
	}
  //边沿触发中断
  if(TIM_GetITStatus(TIM5,TIM_IT_CC1) != RESET)
  {
		//清中断标志位
		TIM_ClearITPendingBit(TIM5,TIM_IT_CC1);
		
    //判断是不是上升沿触发(直接通过寄存器判断)  0为上升沿  1为下降沿
    if(!(TIM5->CCER & (1<<1)))
    {
      //开启溢出计数标志位
      update_flay = 1;
      //获取上升沿计数(TIMx->CCR1寄存器)
      rising_edge = TIM_GetCapture1(TIM5);
      //把边沿触发形式改为下降沿(TIMx->CCER寄存器的第1-2位)
      TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);	
    }
    else if(TIM5->CCER & (1<<1))
    {
      //获取下升沿计数(TIMx->CCR1寄存器)
      falling_edge = TIM_GetCapture1(TIM5);
      //计算脉冲宽度
			cont = 65535 * update_num - rising_edge + falling_edge;
      t = cont / 10;
      
      printf("脉冲宽度：%d\r\n",t);
      
      update_num = 0;
      update_flay = 0;
      
      //切换成上升沿(TIMx->CCER寄存器的第1-2位)
			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);
    }
  }
}







