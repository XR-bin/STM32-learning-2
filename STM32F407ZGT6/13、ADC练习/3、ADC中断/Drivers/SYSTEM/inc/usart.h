#ifndef __USART_H
#define __USART_H

    /****************   外部头文件声明   ****************/
    #include "sys.h" 
    #include "stdio.h"



    /****************    函数外部声明   *****************/
    /*串口1*/
    void USART1_Init(uint32_t baud);          /* 串口1初始化 */
    uint8_t USART1_Receive_Byte(void);        /* 串口1接收一个字节 */

#endif












