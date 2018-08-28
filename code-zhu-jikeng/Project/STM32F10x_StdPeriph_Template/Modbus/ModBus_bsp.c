/**
******************************************************************************
* @file    ModBus_bsp.c
* @author  Yu
* @version V1.0
* @date    2018��5��31��
* @brief   BSP(board support package)�ǰ弶֧�ְ���ȡ������ֲʱ���޸ĸ��ļ�
******************************************************************************
* @attention
*
*
*
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include  "ModBus_bsp.h"
#include  "ModBus.h"
#include "UART.h"

/** @addtogroup ModBus_bsp
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void ModBus1_send(Uint16 n)  //����n���ֽ�
{
//	Uint16 i;
//	  HAL_GPIO_WritePin(DE1_GPIO_Port, DE1_Pin, GPIO_PIN_RESET);
//	  HAL_Delay(1);
////	for(i=0;i<n;i++)
////	{
//		HAL_UART_Transmit(&huart1,UartInfo.TxData,n,0xffff);
////	}

//	HAL_GPIO_WritePin(DE1_GPIO_Port, DE1_Pin, GPIO_PIN_SET);
	SendStr(TOP_UART,UartInfo.TxData,n);
}

/**
  * @}
  */ 

/******************* (C) COPYRIGHT YuRobo *****END OF FILE****/
