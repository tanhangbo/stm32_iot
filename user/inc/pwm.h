#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x_tim.h"

void init_pwm(void);
void pwm_set_enable(int enable);

#endif
