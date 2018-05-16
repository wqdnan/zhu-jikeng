#ifndef __TIM_H
#define __TIM_H



#include "stm32f10x.h"
#include "main.h"


#define TIM2_CCR1_VAL 10000 //定时单位为1us，定时长度为10ms
#define TIM2_CNT_100MS 10   //TIM2定时100ms所需时间
#define TIM2_CNT_1S 100		//TIM2定时1s所需时间

#define TIM3_CCR1_VAL 100   //定时单位为1us，定时频率为10KHz


typedef  struct
{
	e_state t10ms_flag;
	e_state t100ms_flag;
	e_state t1s_flag;
}TIM2_STATE;

extern TIM2_STATE mainT_state;
extern volatile uint16_t tickTime_10ms;

void TIM2Init(void);
void TIM3Init(void);
void TIM4Init(void);

void myDelay_ms(uint16_t cnt);

void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);



#endif
