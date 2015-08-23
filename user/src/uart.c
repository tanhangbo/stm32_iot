
#include "stm32f10x_usart.h"
#include "misc.h"


char uart_rx_log_buffer[100] = {0};
int uart_rx_log_index = 0;

//RCC时钟配置
static void RCC_cfg()
{
	//打开GPIO时钟，复用功能，串口1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
}

//IO口配置
static void GPIO_cfg()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//PA9作为US1的TX端，打开复用，负责发送数据
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA , &GPIO_InitStructure);

	//PA10作为US1的RX端，负责接收数据
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

//串口初始化
static void USART_cfg()
{
	USART_InitTypeDef USART_InitStructure;
	//将结构体设置为缺省状态
	USART_StructInit(&USART_InitStructure);
	//波特率设置为115200
	USART_InitStructure.USART_BaudRate = 115200;
	//一帧数据的宽度设置为8bits
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	//在帧结尾传输1个停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	//奇偶失能模式，无奇偶校验
	USART_InitStructure.USART_Parity = USART_Parity_No;
	//发送/接收使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//硬件流控制失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//设置串口1
	USART_Init(USART1, &USART_InitStructure);
	//打开串口1的中断响应函数
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

	//打开串口1
	USART_Cmd(USART1, ENABLE);
}

//配置中断
static void NVIC_cfg()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                                               //选择中断分组2
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;                              //选择串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                                 //抢占式中断优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                                            //响应式中断优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                                 //使能中断
	NVIC_Init(&NVIC_InitStructure);
}



void uart_send_data(char *data);

void process_uart_command(char *cmd) {
	
	uart_send_data(cmd);
	//uart_send_data("sending ping to 192.168.0.1\n");

		

}

void USART1_IRQHandler(void)
{
	FlagStatus RX_status;

	
	//确认是否接收到数据
	RX_status = USART_GetFlagStatus(USART1, USART_FLAG_RXNE);
	//接收到数据
	if(RX_status == SET)
	{
		uart_rx_log_buffer[uart_rx_log_index++] = USART_ReceiveData(USART1);
		if (uart_rx_log_buffer[uart_rx_log_index - 1] == '\r') {
			//uart_send_data(uart_rx_log_buffer);
			process_uart_command(uart_rx_log_buffer);
			memset(uart_rx_log_buffer, 0, sizeof(uart_rx_log_buffer));
			uart_rx_log_index = 0;

		}
	}
	

	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{   
		
	}

}


void uart_init()
{
	RCC_cfg();
	GPIO_cfg();
	NVIC_cfg();
	USART_cfg();

	USART_ClearFlag(USART1,USART_FLAG_TC);
}

void uart_send_data(char *data)
{
	
	int len = strlen(data);
	int i = 0;

	
	for(i = 0; i < len; i++) {
		USART_SendData(USART1,data[i]);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
	}

}
