
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "delay.h"
#include "misc.h"
#include "uart.h"

/* refer to http://blog.sina.com.cn/s/blog_49cb42490100rp7b.html*/

static void EXTI2_IRQHandler(void)
{
	
	uart_send_data("EXTI2_IRQHandler\n");
	//点亮LED灯
	GPIO_SetBits(GPIOD,GPIO_Pin_6);
	//清空中断标志位，防止持续进入中断
	EXTI_ClearITPendingBit(EXTI_Line2);
}

static void EXTI3_IRQHandler(void)
{
		uart_send_data("EXTI3_IRQHandler\n");

	GPIO_SetBits(GPIOD,GPIO_Pin_3);
	EXTI_ClearITPendingBit(EXTI_Line3);
}

static void EXTI9_5_IRQHandler(void)
{
		uart_send_data("EXTI9_5_IRQHandler\n");

	
	GPIO_SetBits(GPIOB,GPIO_Pin_5);

	EXTI_ClearITPendingBit(EXTI_Line5);
}


static void EXTI_cfg()
{
	EXTI_InitTypeDef EXTI_InitStructure;
	//清空中断标志
	EXTI_ClearITPendingBit(EXTI_Line2);
	EXTI_ClearITPendingBit(EXTI_Line3);
	EXTI_ClearITPendingBit(EXTI_Line5);

	//选择中断管脚PC.2 PC.3 PC.5
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);

	EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3 | EXTI_Line5; //选择中断线路2 3 5
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //设置为中断请求，非事件请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //设置中断触发方式为上下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                          //外部中断使能
	EXTI_Init(&EXTI_InitStructure);
}


static void IO_cfg()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                                             //选择引脚2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           //输出频率最大50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                  //带上拉电阻输出
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOE,GPIO_Pin_2);                              //将PE.2引脚设置为低电平输出

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5; //选择引脚2 3 5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //选择输入模式为浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;          //输出频率最大50MHz
	GPIO_Init(GPIOC,&GPIO_InitStructure);                                 //设置PC.2/PC.3/PC.5
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6;                  //选择引脚3 6
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           //输出频率最大50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                  //带上拉电阻输出
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                                         //选择引脚5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;          //输出频率最大50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                  //带上拉电阻输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);        
}


static void RCC_cfg()
{
	//打开PE PD PC PB端口时钟，并且打开复用时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
}

static void NVIC_cfg()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                          //选择中断分组2
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;     //选择中断通道2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占式中断优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //响应式中断优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                   //使能中断
	NVIC_Init(&NVIC_InitStructure);

	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;            //选择中断通道3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占式中断优先级设置为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //响应式中断优先级设置为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                   //使能中断
	NVIC_Init(&NVIC_InitStructure);

	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;  //选择中断通道5
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占式中断优先级设置为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //响应式中断优先级设置为2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                   //使能中断
	NVIC_Init(&NVIC_InitStructure);
}



#if 0

//test code
static void GPIO_PA7_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 						 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, 1);

}
static void GPIO_PA7_set_input(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}
#endif
void dh11_init()
{
	RCC_cfg();
	IO_cfg();
	EXTI_cfg();
	NVIC_cfg();

	/* wait for dh11 ready */
	delay_ms(1000);
}


void dh11_read_raw()
{
	
	
}

