
#include "stm32f10x.h"
#include "delay.h"
extern void init_pwm();
extern void send_ir_data(unsigned char* data, int len);


int main()
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
