#include "sys.h"

/*****************************************************
*函数功能  ：对独立看门狗进行配置
*函数名    ：IWDG_Init
*函数参数  ：u16 rlr
*函数返回值：void
*描述      ：每计625次为1s
********************************************************/
void IWDG_Init(u16 rlr)
{
	//开启内部低速振荡器(RCC_CSR寄存器的第0位)
	RCC_LSICmd(ENABLE);
	//等待内部低速振荡器开启完成(RCC_CSR寄存器的第1位)
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	
	//解除写保护
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //输入0x5555 (IWDG->KR寄存器)
	//设置IWDG分频系数
	IWDG_SetPrescaler(0x3);                       //32分频(IWDG_PR寄存器)
	//重载寄存器
	IWDG_SetReload(rlr);                          //设置IWDG装载值(IWDG->RLR寄存器)
	//开启前必须喂一次狗
	IWDG_ReloadCounter();                         //0xAAAA(IWDG->KR寄存器)
	//使能看门狗
	IWDG_Enable(); 
}

















