#ifndef _LOW_POWER_H
#define _LOW_POWER_H

	#include "stm32f10x.h"
	
	void LowPower_Init(void);
	void Sys_Standby(void);
	void Sys_Stop(void);
	u8 Check_WKUP(void);

#endif
