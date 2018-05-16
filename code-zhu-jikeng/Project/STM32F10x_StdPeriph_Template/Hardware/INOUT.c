#include "INOUT.h"




void pinTest(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//绑在单片机端的发光LED引脚
	RCC_APB2PeriphClockCmd(GPIO_CLK_TEST1, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_TEST1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_TEST1, &GPIO_InitStructure);
}
