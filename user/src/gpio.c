
#include "gpio.h"


void gpio_init(uint16_t gpio_pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE); 						 	 
  GPIO_InitStructure.GPIO_Pin = gpio_pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void gpio_write_high()
{
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
}

void gpio_write_low()
{
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
}
