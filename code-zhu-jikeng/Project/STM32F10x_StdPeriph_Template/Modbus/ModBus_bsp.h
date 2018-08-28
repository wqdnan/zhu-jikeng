/**
******************************************************************************
* @file    ModBus_bsp.h
* @author  Yu
* @version V1.0
* @date    2018年5月31日
* @brief   BSP(board support package)是板级支持包，争取作到移植时仅修改该文件
******************************************************************************
* @attention
*
*
*
******************************************************************************
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MODBUSBSP_H_
#define MODBUSBSP_H_
/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Exported types ------------------------------------------------------------*/
typedef int                int16;
typedef long               int32;
typedef long long          int64;
typedef unsigned int       Uint16;
typedef unsigned long      Uint32;
typedef unsigned long long Uint64;
typedef float              float32;
typedef long double        float64;
/* Exported constants --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
//extern UART_HandleTypeDef huart1;
extern struct SAngle 	stcAngle;
/* Exported functions ------------------------------------------------------- */
void ModBus1_send(Uint16 n);  	//发送 n个字节



#endif /* MODBUSBSP_H_ */
/******************* (C) COPYRIGHT YuRobo *****END OF FILE****/
