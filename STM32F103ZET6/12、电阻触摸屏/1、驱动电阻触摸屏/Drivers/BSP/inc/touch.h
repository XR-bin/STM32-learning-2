#ifndef __TOUCH_H
#define __TOUCH_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   宏定义   ********************/
    /* 片选线 */
    #define T_CS_H    GPIO_SetBits(GPIOF,GPIO_Pin_11)      /* 片选失能 */
    #define T_CS_L    GPIO_ResetBits(GPIOF,GPIO_Pin_11)    /* 片选使能 */
    /* 时钟线 */
    #define T_SCK_H   GPIO_SetBits(GPIOB,GPIO_Pin_1)
    #define T_SCK_L   GPIO_ResetBits(GPIOB,GPIO_Pin_1)
    /* 输出线 */
    #define T_MOSI_H  GPIO_SetBits(GPIOF,GPIO_Pin_9)
    #define T_MOSI_L  GPIO_ResetBits(GPIOF,GPIO_Pin_9)
    /* 输入线 */
    #define T_MISO    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)
    /* 笔接触线 */
    #define T_PEN     GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_10)

    /* 指令 */
    #define TOUCH_X   0xD0    //获取x坐标数据
    #define TOUCH_Y   0x90    //获取y坐标数据



    /****************    函数外部声明   *****************/
    void Touch_IO_Init(void);                /* 电阻触摸屏引脚初始化 */
    uint16_t Touch_Data(uint8_t command);    /* 向电阻触摸屏发送数据并接收电阻触摸屏发来的数据 */

#endif







