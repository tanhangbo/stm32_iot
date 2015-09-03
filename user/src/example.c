
#include "example.h"
#include "gpio.h"
#include "delay.h"
#include "ir.h"
#include "pwm.h"
#include "uart.h"



/* for printf */
#include "stdio.h"
int fputc(int ch, FILE *f)
{
	uart_init();
	uart_send_byte(ch);
	return 0;
}



void example_uart()
{
	uart_init();
	uart_send_data("I want 998\n");

}

void example_ir()
{
	unsigned char data[6] = {0xb2,0x4d,0x1F,0xE0,0xD8,0x27};

	delay_init();
	init_pwm();

	while(1) {
		
		send_ir_data(data, 6);
		send_ir_data(data, 6);
		delay_ms(100);
	}	

}

void example_led()
{
	delay_init();
	/* using PC13 to output as LED PIN */
	gpio_init(GPIO_Pin_13);
	while (1) {
		gpio_write_high();
		delay_ms(1000);
		gpio_write_low();
		delay_ms(1000);
	}
	
}






