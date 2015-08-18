#ifndef __USART_H
#define __USART_H
#include <stm32f10x.h>
#include "stdio.h"	 
	  	
extern u8 USART_RX_BUF[256];    //接收缓冲,最大256个字节 
extern u8 USART_RX_STA;         //接收完成状态标志	
extern u8 USART_RX_CNT;         //接收数据计数

//如果想串口中断接收，请不要注释以下宏定义
//#define EN_USART1_RX //使能串口1接收
//void uart_init(u32 pclk2,u32 bound);

#endif	   
















