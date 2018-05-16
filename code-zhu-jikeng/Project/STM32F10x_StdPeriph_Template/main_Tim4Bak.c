/**
  ******************************************************************************
  * @file    TIM/TimeBase/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"//#include "stm32f10x_lib.h"
 
void RCC_cfg();
void GPIO_cfg();
void TIMER_cfg();
void PWM_cfg();
//ռ�ձȣ�ȡֵ��ΧΪ0-100
int dutyfactor = 10;
 
int main()
{
     int Temp;
       RCC_cfg();
       GPIO_cfg();
       TIMER_cfg();
       PWM_cfg();
 
       //ʹ��TIM3��ʱ������ʼ���PWM
       TIM_Cmd(TIM4, ENABLE);
 
       while(1);
}
 
void RCC_cfg()
{
       //�������״̬����
       //ErrorStatus HSEStartUpStatus;
      
       //��RCC�Ĵ�����������ΪĬ��ֵ
       //RCC_DeInit();
 
//       //���ⲿ����ʱ�Ӿ���
//       RCC_HSEConfig(RCC_HSE_ON);
// 
//       //�ȴ��ⲿ����ʱ�Ӿ�����
//       HSEStartUpStatus = RCC_WaitForHSEStartUp();
//       if(HSEStartUpStatus == SUCCESS)
//       {
//              //����AHBʱ��(HCLK)Ϊϵͳʱ��
//              RCC_HCLKConfig(RCC_SYSCLK_Div1);
// 
//              //���ø���AHBʱ��(APB2)ΪHCLKʱ��
//              RCC_PCLK2Config(RCC_HCLK_Div1);
// 
//              //���õ���AHBʱ��(APB1)ΪHCLK��2��Ƶ
//              RCC_PCLK1Config(RCC_HCLK_Div2);
//             
//              //����FLASH������ʱ
//              FLASH_SetLatency(FLASH_Latency_2);
// 
//              //ʹ��Ԥȡָ����
//              FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
// 
//              //����PLLʱ�ӣ�ΪHSE��9��Ƶ 8MHz * 9 = 72MHz
//              RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
// 
//              //ʹ��PLL
//              RCC_PLLCmd(ENABLE);
// 
//              //�ȴ�PLL׼������
//              while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
// 
//              //����PLLΪϵͳʱ��Դ
//              RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
// 
//              //�ж�PLL�Ƿ���ϵͳʱ��
//              while(RCC_GetSYSCLKSource() != 0x08);
//       }
 
       //����TIM3��ʱ��
       RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
       //����GPIOB��ʱ�Ӻ͸��ù���
       RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
 
}
 
void GPIO_cfg()
{
       GPIO_InitTypeDef GPIO_InitStructure;
 
      
       //����ӳ�䣬��TIM3_CH2ӳ�䵽PB5
//     GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
       //GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
 
       //ѡ������5
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
       //���Ƶ�����50MHz                                                        
       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
       //�����������                                              
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
 
       GPIO_Init(GPIOB,&GPIO_InitStructure);
}
 
void TIMER_cfg()
{
       TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
 
       //���½�Timer����Ϊȱʡֵ
       TIM_DeInit(TIM4);
       //�����ڲ�ʱ�Ӹ�TIM3�ṩʱ��Դ
       TIM_InternalClockConfig(TIM4);
       //Ԥ��Ƶϵ��Ϊ0����������Ԥ��Ƶ����ʱTIMER��Ƶ��Ϊ72MHz
       TIM_TimeBaseStructure.TIM_Prescaler = 0;
       //����ʱ�ӷָ�
       TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
       //���ü�����ģʽΪ���ϼ���ģʽ
       TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
       //���ü��������С��ÿ��7200�����Ͳ���һ�������¼�����PWM�����Ƶ��Ϊ10kHz
       TIM_TimeBaseStructure.TIM_Period = 7200 - 1;
       //������Ӧ�õ�TIM3��
       TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
}
 
void PWM_cfg()
{
       TIM_OCInitTypeDef TimOCInitStructure;
       //����ȱʡֵ
       TIM_OCStructInit(&TimOCInitStructure);
       //PWMģʽ1���
       TimOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
       //����ռ�ձȣ�ռ�ձ�=(CCRx/ARR)*100%��(TIM_Pulse/TIM_Period)*100%
       TimOCInitStructure.TIM_Pulse = dutyfactor * 7200 / 100;
       //TIM����Ƚϼ��Ը�
       TimOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
       //ʹ�����״̬
       TimOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
       //TIM3��CH2���
       TIM_OC2Init(TIM4, &TimOCInitStructure);
       //����TIM3��PWM���Ϊʹ��
       TIM_CtrlPWMOutputs(TIM4,ENABLE);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  while (1)
  {}
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
