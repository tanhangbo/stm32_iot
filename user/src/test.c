
#include "stm32f10x.h"
#include "delay.h"
extern void init_pwm();
extern void send_ir_data(unsigned char* data, int len);

void ir_test()
{
	unsigned char data[6] = {0xb2,0x4d,0x1F,0xE0,0xD8,0x27};

	delay_init(72);
	init_pwm();

	while(1) {
		
		send_ir_data(data, 6);
		send_ir_data(data, 6);
		delay_ms(100);
	}	

}


extern void uart_init();
void uart_test()
{
	uart_init();
	uart_send_data("I want 998\n");

}


int main()
{

	uart_test();
	while(1);
}
