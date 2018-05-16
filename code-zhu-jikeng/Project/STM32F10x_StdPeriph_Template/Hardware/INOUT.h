#ifndef __INOUT_H
#define __INOUT_H


#include "stm32f10x.h"

//output test
#define GPIO_PIN_TEST1 GPIO_Pin_13
#define	GPIO_CLK_TEST1 RCC_APB2Periph_GPIOC
#define	GPIO_TEST1 GPIOC

#define TURN_TEST1() GPIO_WriteBit(GPIO_TEST1, GPIO_PIN_TEST1, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIO_TEST1, GPIO_PIN_TEST1)))
#define SET_TEST1()  GPIO_SetBits(GPIO_TEST1,GPIO_PIN_TEST1)
#define RESET_TEST1() GPIO_ResetBits(GPIO_TEST1,GPIO_PIN_TEST1)


//input function 拨码开关S1~S8   脉冲输入
#define GPIO_PIN_SLAVE_TYPE1 GPIO_Pin_0
#define GPIO_PIN_SLAVE_TYPE2 GPIO_Pin_1
#define GPIO_PIN_SLAVE_TYPE3 GPIO_Pin_2
#define GPIO_PIN_SLAVE_NUMBER1 GPIO_Pin_3
#define GPIO_PIN_SLAVE_NUMBER2 GPIO_Pin_4
#define GPIO_PIN_SLAVE_NUMBER3 GPIO_Pin_5
#define GPIO_PIN_SLAVE_NUMBER4 GPIO_Pin_6
#define GPIO_PIN_SLAVE_NUMBER5 GPIO_Pin_7
#define	GPIO_CLK_SLAVE RCC_APB2Periph_GPIOA
#define	GPIO_SLAVE GPIOA

#define READ_SLAVE_TYPE() ((GPIO_ReadInputDataBit(GPIO_SLAVE, GPIO_PIN_SLAVE_TYPE1))<<2| \
                           ((GPIO_ReadInputDataBit(GPIO_SLAVE, GPIO_PIN_SLAVE_TYPE2))<<1)| \
                           ((GPIO_ReadInputDataBit(GPIO_SLAVE, GPIO_PIN_SLAVE_TYPE3)))  )                       )
#define READ_SLAVE_NUMBER() ((GPIO_ReadInputDataBit(GPIO_SLAVE, GPIO_PIN_SLAVE_NUMBER1))<<4| \
                           ((GPIO_ReadInputDataBit(GPIO_SLAVE, GPIO_PIN_SLAVE_NUMBER2))<<3)| \
                           ((GPIO_ReadInputDataBit(GPIO_SLAVE, GPIO_PIN_SLAVE_NUMBER3))<<2)| \
                           ((GPIO_ReadInputDataBit(GPIO_SLAVE, GPIO_PIN_SLAVE_NUMBER4))<<1)| \
                           ((GPIO_ReadInputDataBit(GPIO_SLAVE, GPIO_PIN_SLAVE_NUMBER5))) )

#define GPIO_PIN_MC1 GPIO_Pin_6
#define GPIO_PIN_MC2 GPIO_Pin_7
#define	GPIO_CLK_MC RCC_APB2Periph_GPIOC
#define	GPIO_MC GPIOC

#define READ_MC1() GPIO_ReadInputDataBit(GPIO_MC, GPIO_PIN_MC1)
#define READ_MC2() GPIO_ReadInputDataBit(GPIO_MC, GPIO_PIN_MC2)

//output function  数字输出  测试LED
#define GPIO_PIN_CTRL1 GPIO_Pin_10
#define GPIO_PIN_CTRL2 GPIO_Pin_11
#define	GPIO_CLK_CTRL RCC_APB2Periph_GPIOB
#define	GPIO_CTRL GPIOB
#define TURN_CTRL1() GPIO_WriteBit(GPIO_CTRL, GPIO_PIN_CTRL1, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIO_CTRL, GPIO_PIN_CTRL1)))
#define SET_CTRL1()  GPIO_SetBits(GPIO_CTRL,GPIO_PIN_CTRL1)
#define RESET_CTRL1() GPIO_ResetBits(GPIO_CTRL,GPIO_PIN_CTRL1)
#define TURN_CTRL2() GPIO_WriteBit(GPIO_CTRL, GPIO_PIN_CTRL2, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIO_CTRL, GPIO_PIN_CTRL2)))
#define SET_CTRL2()  GPIO_SetBits(GPIO_CTRL,GPIO_PIN_CTRL2)
#define RESET_CTRL2() GPIO_ResetBits(GPIO_CTRL,GPIO_PIN_CTRL2)

#define GPIO_PIN_LED0 GPIO_Pin_0
#define GPIO_PIN_LED1 GPIO_Pin_1
#define	GPIO_CLK_LED RCC_APB2Periph_GPIOB
#define	GPIO_LED GPIOB
#define TURN_LED0() GPIO_WriteBit(GPIO_LED, GPIO_PIN_LED0, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIO_LED, GPIO_PIN_LED0)))
#define SET_LED0()  GPIO_SetBits(GPIO_LED,GPIO_PIN_LED0)
#define RESET_LED0() GPIO_ResetBits(GPIO_LED,GPIO_PIN_LED0)
#define TURN_LED1() GPIO_WriteBit(GPIO_LED, GPIO_PIN_LED1, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIO_LED, GPIO_PIN_LED1)))
#define SET_LED1()  GPIO_SetBits(GPIO_LED,GPIO_PIN_LED1)
#define RESET_LED1() GPIO_ResetBits(GPIO_LED,GPIO_PIN_LED1)

void pinTest(void);

#endif
