/*
 * beep.c
 *
 * Created: 2020/06/29 9:47:36
 *  Author: gyneco2d
 */ 

#include <atmel_start.h>
#include "beep.h"

void beep(action act)
{
	int freq = act == CONF ? 330 : 247;
	switch (act) {
		case CONF:
			freq = 330;
			break;
		case EXIT:
			freq = 247;
			break;
		case UP:
			freq = 698;
			break;
		case DOWN:
			freq = 784;
			break;
		default:
			break;
	}
	pwm_disable(&PWM_1);
	pwm_set_parameters(&PWM_1, freq, freq/2);
	pwm_enable(&PWM_1);
	delay_ms(60);
	pwm_disable(&PWM_1);
}
