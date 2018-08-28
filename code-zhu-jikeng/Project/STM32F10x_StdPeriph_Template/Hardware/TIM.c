#include "TIM.h"
#include "ModBus.h"

/**
******************************************************************************
* @file    TIM.c
* @author  wqdnan
* @version V3.5.0
* @date    28-09-2017
* @brief   产生主循环的定时时间间隔，定时产生步进电机的脉冲信号，定时产生光电调制脉冲信号
******************************************************************************
* @attention
*
*
*
******************************************************************************
*/


TIM2_STATE mainT_state = {rstFlag,rstFlag,rstFlag};

volatile uint16_t tickTime_10ms = 0;


/**
  * @brief  主定时器，产生最快为10ms的定时中断
  * @param  None
  * @retval None
  *
  */
void TIM2Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	uint16_t PrescalerValue =  0;
	
	RCC_PCLK1Config(RCC_HCLK_Div4);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//使用第0组，组优先级最高
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//主优先级0，从优先级0，最高的优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock / 2000000) - 1;//设定计数单位频率1MHz
	TIM_TimeBaseStructure.TIM_Period = 65535;//该值没有实际作用
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);

	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = TIM2_CCR1_VAL;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

/**
  * @brief TIM3初始化，用于产生水平和垂直步进电机 step 定时，定时频率为10KHz
  * @param  None
  * @retval None
  * 通过在最小定时间隔中累加计数值，可以实现调速的效果
  */
void TIM3Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	uint16_t PrescalerValue =  0;

	RCC_PCLK1Config(RCC_HCLK_Div4);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//使用第0组，组优先级最高
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//主优先级1，从优先级0，较高的优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock / 2000000) - 1;//设定计数单位频率1MHz
	TIM_TimeBaseStructure.TIM_Period = 65535;//该值没有实际作用
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);

	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = TIM3_CCR1_VAL;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);

	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

/**
  * @brief
  * @param  None
  * @retval None
  *
  */
void TIM4Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	uint16_t PrescalerValue =  0;

	RCC_PCLK1Config(RCC_HCLK_Div4);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//使用第0组，组优先级最高
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;//主优先级1，从优先级0，较高的优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock / 2000000) - 1;//设定计数单位频率1MHz
	TIM_TimeBaseStructure.TIM_Period = 65535;//该值没有实际作用
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_PrescalerConfig(TIM4, PrescalerValue, TIM_PSCReloadMode_Immediate);

	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = TIM4_CCR1_VAL;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);

	TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

/**
  * @brief 和TIM2定时配合使用，计数变化间隔为10ms
  * @param  None
  * @retval None
  * tickTime_10ms为全局易变 变量
  */
void myDelay_ms(uint16_t cnt)
{
	uint16_t tick = tickTime_10ms;
	while(1)
	{
		if((tickTime_10ms-tick)>cnt)
			break;
	}
}

/**
  * @brief  TIM2对应的中断函数，用于产生定时中断，定时为10ms，100ms,1s
  * @param  None
  * @retval None
  * 通过修改 TIM2_CCR1_VAL、TIM2_CNT_100MS、TIM2_CNT_1S，可以改变定时间隔
  */
void TIM2_IRQHandler(void)
{
	static uint16_t capture = 0;
	static uint16_t timerNum_1S = 0;
	static uint16_t timerNum_100MS = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		//GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12)));
		capture = TIM_GetCapture1(TIM2);
		TIM_SetCompare1(TIM2, capture + TIM2_CCR1_VAL);
		//功能函数添加
		mainT_state.t10ms_flag = enFlag;
		timerNum_100MS ++;
		timerNum_1S ++;
		if(timerNum_100MS >= TIM2_CNT_100MS)
		{
			timerNum_100MS = 0;
			mainT_state.t100ms_flag = enFlag;
		}
		if(timerNum_1S >= TIM2_CNT_1S)
		{
			timerNum_1S = 0;
			mainT_state.t1s_flag = enFlag;
		}
		tickTime_10ms ++;//使用来进行延时
	}

}
/**
  * @brief  TIM3对应的中断函数，用于产生步进电机脉冲，并控制脉冲的生成速度
  * @param  None
  * @retval None
  * 通过修改horiMotorCmd.speed，vertMOtorCmd.speed ，可以改变脉冲频率，最小为10KHz
  */
void TIM3_IRQHandler(void)
{
	static uint16_t capture = 0;
	static uint16_t spdHoriCnt = 0;
	static uint16_t spdVertCnt = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		//GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12)));
		capture = TIM_GetCapture1(TIM3);
		TIM_SetCompare1(TIM3, capture + TIM3_CCR1_VAL);
		//功能函数添加
		

	}

}

/**
  * @brief  TIM4对应的中断函数，用于modbus接收一帧数据的检测
  * @param  None
  * @retval None
  *
  */
void TIM4_IRQHandler(void)
{
	static uint16_t capture = 0;
	static uint16_t spdHoriCnt = 0;
	static uint16_t spdVertCnt = 0;
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		//GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12)));
		capture = TIM_GetCapture1(TIM4);
		TIM_SetCompare1(TIM4, capture + TIM4_CCR1_VAL);
		//功能函数添加
		ModBus_TIM_Callback();

	}

}


