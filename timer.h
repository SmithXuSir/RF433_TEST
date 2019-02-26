#ifndef _TIMER_H
#define _TIMER_H

#include "sys.h"
#include "stdio.h"

extern u8 flag;

void Timer_Init(u16 arr,u16 psc); //arr为自动重装载寄存器周期的值 psc时钟频率除数的预分频值

#endif
