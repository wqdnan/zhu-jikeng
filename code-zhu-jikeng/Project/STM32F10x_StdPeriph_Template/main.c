/**
  ******************************************************************************
  * @file    main.c
  * @author  wqdnan
  * @version V3.5.0
  * @date    29-9-2017
  * @brief   Main program body
  ******************************************************************************
  * @attention
  * 初始化硬件外设，主循环中定期检测标志位并处理事务
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "TIM.h"
#include "UART.h"
#include "INOUT.h"
#include "main.h"
#include "SPI.h"
#include "UARTControl.h"



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/  

uint8_t temp = 0;

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	e_state fucFlag = rstFlag;
  TIM2Init();
  UART2Init();
  UART3Init();
  pinTest();
  uartData.workTypeArray[0] = ANGLE_CHECK;//STRAIN_CHECK;//使能应力计检测
//	uartData.workTypeArray[1] = WATER_DEPTH_CHECK;
//	uartData.workTypeArray[2] = ANGLE_CHECK;
  
  while (1)
  {
		
	  if(mainT_state.t10ms_flag == enFlag)
	  {
		  mainT_state.t10ms_flag = rstFlag;

		  if(rx2Flag == enFlag)//接收到从机回应数据
		  {
			  rx2Flag = rstFlag;
			  fucFlag = UartRcvHandle(&rxBuf2[0],rxLength2,&uartData);
			  if(fucFlag == enFlag)//需要回复了，则直接回复
			  {
				  UartTxHandle(MASTER_UART,&txBuf3[0],&uartData);
			  }
		  }
		  if(rx3Flag == enFlag)//接收到DSP的数据信息
		  {
			  rx3Flag = rstFlag;
			  fucFlag = UartRcvHandle(&rxBuf3[0],rxLength3,&uartData);
			  if(fucFlag == enFlag)//需要回复
			  {
					
			  }
		  }
	  }
	  if(mainT_state.t100ms_flag == enFlag)//实际是100ms
	  {
		  mainT_state.t100ms_flag = rstFlag;
		  if(uartData.slaveTransFlag == enFlag)
		  {
			  if(slaveTopCtrl(&uartData,&txBuf2[0]) == enFlag)//发送完成，且等待回复超时了，则直接回复给上位机信息
			  {
				  UartTxHandle(MASTER_UART,&txBuf3[0],&uartData);
				  if(SLAVE_MAXNUM > 48)//当从机数量大于了48，则数据分两段发送
				  {
					 //延迟100ms
					  myDelay_ms(10);
					  UartTxHandle(MASTER_UART,&txBuf3[0],&uartData);
				  }
				  uartData.slaveTypeCheckFlag ++;
				  uartData.crntHandleSlaveNum = 0;
				  if(uartData.workTypeArray[uartData.slaveTypeCheckFlag] == 0)//没有待轮询工作的从机类型了
				  {
					  uartData.slaveTransFlag = rstFlag;//轮询结束了，等待下一次轮询间隔到
					  uartData.slaveTypeCheckFlag = 0;
				  }
			  }
		  }

		  
	  }
	  if(mainT_state.t1s_flag == enFlag)
	  {
			TURN_TEST1();
		  mainT_state.t1s_flag = rstFlag;
		  regularTimeDo(&uartData);//定时操作
	  }
  }
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
  {
  }
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
