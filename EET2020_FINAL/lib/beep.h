/*
 * beep.h
 *
 * Created: 2020/06/29 9:46:52
 *  Author: gyneco2d
 */ 


#ifndef beep_H_
#define beep_H_


typedef enum {
	CONF,
	EXIT,
	UP,
	DOWN
} action;

void beep(action act);


#endif /* beep_H_ */