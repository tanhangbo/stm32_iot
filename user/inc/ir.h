#ifndef __IR_H
#define __IR_H


#define NEC_HDR_MARK    4500
#define NEC_HDR_SPACE   4500
#define NEC_BIT_MARK     560
#define NEC_ONE_SPACE   1690
#define NEC_ZERO_SPACE   560
/* compensate for accuracy */
#define PWM_ON_DELAY 13
#define PWM_OFF_DELAY 15
#define BIT(x) (1<<x)


void send_ir_data(unsigned char* data, int len);


#endif
