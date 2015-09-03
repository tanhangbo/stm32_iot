
#include "pwm.h"
#include "delay.h"




/* refer to http://blog.sina.com.cn/s/blog_49cb42490100s6uh.html */
//duty factor, 0-100
int dutyfactor = 50;
int tim_counter = 1895;//7200


static void RCC_cfg()
{
	//开启TIM3的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//开启GPIOB的时钟和复用功能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
}

static void GPIO_cfg()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//部分映射，将TIM3_CH2映射到PB5
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
	//选择引脚5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	//输出频率最大50MHz                                                        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//复用推挽输出                                              
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

static void TIMER_cfg()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	//重新将Timer设置为缺省值
	TIM_DeInit(TIM3);
	//采用内部时钟给TIM3提供时钟源
	TIM_InternalClockConfig(TIM3);
	//预分频系数为0，即不进行预分频，此时TIMER的频率为72MHz
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	//设置时钟分割
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//设置计数器模式为向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//设置计数溢出大小，每计7200个数就产生一个更新事件，即PWM的输出频率为10kHz
	TIM_TimeBaseStructure.TIM_Period = tim_counter - 1;
	//将配置应用到TIM3中
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
}

static void PWM_cfg()
{
	TIM_OCInitTypeDef TimOCInitStructure;
	//设置缺省值
	TIM_OCStructInit(&TimOCInitStructure);
	//PWM模式1输出
	TimOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//设置占空比，占空比=(CCRx/ARR)*100%或(TIM_Pulse/TIM_Period)*100%
	TimOCInitStructure.TIM_Pulse = dutyfactor * tim_counter / 100;
	//TIM输出比较极性高
	TimOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//使能输出状态
	TimOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//TIM3的CH2输出
	TIM_OC2Init(TIM3, &TimOCInitStructure);
	//设置TIM3的PWM输出为使能
	TIM_CtrlPWMOutputs(TIM3,ENABLE);
}


void init_pwm()
{
	RCC_cfg();
	GPIO_cfg();
	TIMER_cfg();
	PWM_cfg();
}

void pwm_set_enable(int enable) {
	if (enable)
		TIM_Cmd(TIM3, ENABLE);
	else
		TIM_Cmd(TIM3, DISABLE);

}








