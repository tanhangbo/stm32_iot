
#include "ir.h"
#include "pwm.h"
#include "delay.h"



void output_38k(int delay)
{
	pwm_set_enable(1);
	delay_us(delay + PWM_ON_DELAY);
}
void output_38k_off(int delay)
{
	pwm_set_enable(0);
	delay_us(delay - PWM_OFF_DELAY);
}

void send_bit(int bit)
{
	if (bit == 1) {
		output_38k(NEC_BIT_MARK);
		output_38k_off(NEC_ONE_SPACE);
	} else if (bit == 0) {
		output_38k(NEC_BIT_MARK);
		output_38k_off(NEC_ZERO_SPACE);
	} else {
		//printf("critical error!\n");
	}
}

void send_byte(unsigned char byte)
{
	int  i = 0;

	for (i = 7; i >= 0; i--)
		send_bit(((byte & BIT(i)) >> i));
		
}

void send_ir_data(unsigned char* data, int len)
{
	int i  = 0;
	
	output_38k(NEC_HDR_MARK);
	output_38k_off(NEC_HDR_MARK);

	for (i = 0; i < len; i++)
		send_byte(data[i]);

	send_bit(1);
	delay_us(NEC_HDR_MARK- PWM_OFF_DELAY);

}


#if 0

/* 0xb2,0x4d,0x9f,0xe0,0xd8,0xaf */
unsigned int rawCodes_ac_open[] = {4397,4368,561,1578,560,537,557,1581,584,1582,584,486,558,537,584,1554,
	560,538,558,510,558,1607,557,513,585,511,556,1585,581,1583,583,486,557,1609,556,513,558,538,557,512,558,
	1607,558,1583,557,1608,558,1608,557,1580,559,1606,559,1581,556,1609,557,512,559,537,558,512,557,539,557,
	512,558,1607,558,1582,558,537,584,1555,558,1607,556,514,559,537,556,513,558,537,556,514,558,1606,559,511,
	583,513,558,1582,557,1608,558,1580,585,5154,4419,4347,584,1555,558,539,557,1581,584,1581,571,501,577,516,
	571,1568,557,541,581,487,557,1608,557,513,558,538,557,1582,583,1582,584,487,557,1608,557,511,611,486,558,
	510,558,1607,557,1609,556,1583,583,1582,583,1555,558,1607,558,1607,559,1582,557,537,585,486,558,510,558,
	540,558,509,557,1608,558,1608,557,512,559,1606,559,1581,558,537,581,490,558,510,587,511,557,511,559,1606,
	559,510,569,527,558,1582,560,1604,561,1605,561};


void send_raw(int *code, int size)
{

	int i = 0;


	for (i = 0; i < size; i++) {
		if (i%2 == 0) {
			output_38k(code[i]);
		} else {
			output_38k_off(code[i]);
			
		}
	}
}

#endif
