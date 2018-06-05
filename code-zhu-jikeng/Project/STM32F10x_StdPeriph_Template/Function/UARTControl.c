#include "UARTControl.h"
#include "UARTHandle.h"
#include "UART.h"
/**
******************************************************************************
* @file    UARTHandle.c
* @author  wqdnan
* @version V3.5.0
* @date    26-12-2017
* @brief   完成主机与从机的通信数据交互，和主机与上位机的通信交互
******************************************************************************
* @attention
*
*
*
******************************************************************************
*/

/**
  * @brief  控制与从机的数据交互，按照每秒间隔进行执行
  * @param  ctrlComm-从机通信数据交互结构体
  * @retval None
  * 格式：帧头|功能码|主机编号|从机编号|数据长度|数据内容|校验和|帧尾
  * 字节数    1    1     1       1       1      不定        1     1
  */
void regularTimeDo(uartCtntStruct * uartData)
{
	uint8_t i = 0;
	uint8_t j = 0;
	static const uint8_t CHECK_DATA_TIME = 5-1;
	static const uint8_t MOBILE_DATA_TIME = 5-1;
	static uint8_t checkDataCnt = 0;
	static uint8_t mobileDataCnt = 0;
	if(checkDataCnt < CHECK_DATA_TIME)
		checkDataCnt ++;
	else
	{
		checkDataCnt = 0;
		uartData->slaveTransFlag = enFlag;
		uartData->crntHandleSlaveNum = 0;
		if(uartData->workTypeArray[uartData->slaveTypeCheckFlag] == ANGLE_CHECK)
		{
			//先清零之前的倾角传感器数据中的值，全部
			for(i=0;i<SLAVE_MAXNUM;i++)
				for(j=0;j<ANGLE_CHECK_NUM;j++)
					uartData->angleXYReal[i][j] = 0;
		}
	}
	if(mobileDataCnt < MOBILE_DATA_TIME)
	{
		mobileDataCnt ++;
	}
	else
	{
		mobileDataCnt = 0;
		//直接发送移动端广播数据信息
		//wifiTxHandle(uartData,&txBuf4[0]);
	}
}

/**
  * @brief  控制与从机的数据交互，按照固定间隔进行执行（100ms执行一次）
  * @param  ctrlComm-从机通信数据交互结构体
  * @retval rstFlag-不需要回复DSP enFlag-需要回复DSP
  * 格式：帧头|功能码|主机编号|从机编号|数据长度|数据内容|校验和|帧尾
  * 字节数    1    1     1       1       1      不定        1     1
  * 操作的优先级按照手动控制，手动设置，更新从机表，询问数据依次排列
  *
  */
e_state slaveTopCtrl(uartCtntStruct * uartData,uint8_t * txBuffer)
{
	e_state result = rstFlag;
	result = pollSlaveHandle(uartData,txBuffer);
	return result;

}

/**
  * @brief  当接收到设定从机号的数据设定参数，记录下从机的相关参数
  * @param  ctrlComm-从机通信数据交互结构体
  * @retval rstFlag-不需要回复 enFlag-需要串口回复
  */
e_state setParaData(uartCtntStruct * uartData)
{
	e_state result = rstFlag;
	uint8_t i = 0,j = 0;
	if(uartData->crntSlaveNum <= SLAVE_MAXNUM)//有触点控制的从机范围号
	{
		switch(uartData->slaveType)
		{
		case WATER_DEPTH_CHECK://水位
			uartData->waterLevelReal[uartData->crntSlaveNum-1] = uartData->rxFrameCtnt[0]*100+uartData->rxFrameCtnt[1];
			uartData->waterFlowReal[uartData->crntSlaveNum-1] = uartData->rxFrameCtnt[2]*100+uartData->rxFrameCtnt[3];
			break;
		case ANGLE_CHECK://倾角
			for(i=0;i<uartData->rxFrameCtntNum;i+=2)
				uartData->angleXYReal[uartData->crntSlaveNum-1][j++] = uartData->rxFrameCtnt[i]*100+uartData->rxFrameCtnt[i+1];
			break;
		case STRAIN_CHECK://应力计
			uartData->strainFreReal[uartData->crntSlaveNum-1][0] = uartData->rxFrameCtnt[0]*100+uartData->rxFrameCtnt[1];
			uartData->strainFreReal[uartData->crntSlaveNum-1][1] = uartData->rxFrameCtnt[2]*100+uartData->rxFrameCtnt[3];
			break;
		}
	}
	if(uartData->slaveType == uartData->workTypeArray[uartData->slaveTypeCheckFlag])//修改为当前接收的帧为倾角模块的数据帧，则做单独回复，其他类型做统一一次性回复
	{
		result = enFlag;
		uartData->fucNum = 0x14;//准备回复DSP功能数据
	}
	return result;
}

/**
  * @brief  当接收到DSP的数据设定参数，记录下相关从机的相关参数
  * @param  uartData-通信数据交互结构体
  * @retval rstFlag-不需要回复 enFlag-需要串口回复
  */
e_state setCtrlData(uartCtntStruct * uartData)
{
	uint8_t i = 0;
	for(i=0; i<uartData->rxFrameCtntNum; i++)
	{
		uartData->relayState[i] = uartData->rxFrameCtnt[i];
	}
	return rstFlag;
}
/**
  * @brief  串口向wifi模块发送数据帧，不需要回复，ASCII码方式
  * @param  txBuf-数据帧首地址
  * @retval None
  * 将当前的12个从机的实时压力值进行组帧（按照百、十、个、十分位）
  * 帧头 | 功能码 | 主机编号 | 从机编号  | 数据长度（十位） | 数据长度（个位） | 数据内容 | 帧尾
  * A       B       0        0          4                8                     E
  */
void wifiTxHandle(uartCtntStruct *uartData,uint8_t *txBuf)
{
	uint8_t bias = 0;
	uint8_t i = 0;
	txBuf[bias++] = 'A';
	txBuf[bias++] = 'B';
	txBuf[bias++] = '0';
	txBuf[bias++] = '0';
	txBuf[bias++] = '4';
	txBuf[bias++] = '8';
	for(i=0; i<12; i++)
	{
		txBuf[bias++] = (uartData->waterLevelReal[i]/1000%10)+'0';
		txBuf[bias++] = (uartData->waterLevelReal[i]/100%10)+'0';
		txBuf[bias++] = (uartData->waterLevelReal[i]/10%10)+'0';
		txBuf[bias++] = (uartData->waterLevelReal[i]%10)+'0';
	}
	txBuf[bias++] = 'E';
	txBuf[bias++] = '\n';
	SendStr(WIFI_UART,txBuf,bias);
}
/**
  * @brief  对数据帧中提取的数据内容，进行处理，并根据相应内容决定是否要回复
  * @param  uartData-数据帧内容结构体
  *         commData-通信数据内容结构体（需要用户自定义）
  * @retval enFlag-需要回复  rstFlag-不需要回复
  * 注：该函数需要根据实际功能进行重写
  */
e_state UartRxFctnCtntHandle(uartCtntStruct * uartData)
{
	e_state result = rstFlag;//不需要回复

	switch(uartData->fucNum)
	{
		case 0x0D:
		{
			result = setParaData(uartData);
			break;
		}
		case 0x11:
		{
			result = setCtrlData(uartData);
			break;
		}
	}
	return result;
}
/**
  * @brief  对待发送的数据帧的数据内容进行准备，并反馈是否准备好
  * @param  uartData-待写入的数据帧内容结构体
  *         commData-通信数据内容结构体（需要用户自定义）
  * @retval enFlag-数据帧准备好  rstFlag-数据帧未准备好，不做发送操作
  * 注：该函数需要根据实际功能进行重写
  */
e_state UartTxFctnCtntHandle(uartCtntStruct * uartData)
{
	e_state result = rstFlag;
	uint8_t i = 0,j = 0;
	uint8_t bias = 0;
	uint8_t cnt_num = 0;
	uint8_t temp = 0;
	//根据标志位进行数据帧准备
	switch(uartData->fucNum)
	{
		case 0x14:
		{
			if(uartData->frameTypeFlag == FIRST_HALF)
			{
				if(SLAVE_MAXNUM > 48)
				{
					cnt_num = 48;
					uartData->frameTypeFlag = SECOND_HALF;
				}
				else
					cnt_num = SLAVE_MAXNUM;
				switch(uartData->workTypeArray[uartData->slaveTypeCheckFlag])
				{
				case WATER_DEPTH_CHECK:
					for(i=0; i<cnt_num; i++)
					{
						uartData->txFrameCtnt[bias] =    (uartData->waterLevelReal[i]/1000%10)<<4;//单位：0.01m
						uartData->txFrameCtnt[bias++] += (uartData->waterLevelReal[i]/100%10);
						uartData->txFrameCtnt[bias] =    (uartData->waterLevelReal[i]/10%10)<<4;
						uartData->txFrameCtnt[bias++] += (uartData->waterLevelReal[i]%10);
						uartData->txFrameCtnt[bias] =    (uartData->waterFlowReal[i]/1000%10)<<4;//单位：0.1立方米
						uartData->txFrameCtnt[bias++] += (uartData->waterFlowReal[i]/100%10);
						uartData->txFrameCtnt[bias] =    (uartData->waterFlowReal[i]/10%10)<<4;
						uartData->txFrameCtnt[bias++] += (uartData->waterFlowReal[i]%10);

					}
					uartData->crntHandleSlaveNum = 0x20;
					break;
				case ANGLE_CHECK://倾角传感器设定为根据从机号单独传递，则不考虑从机总数量的问题
					temp = uartData->crntSlaveNum-1;
					for(j=0; j<(ANGLE_CHECK_NUM*2); j+=2)
					{
						uartData->txFrameCtnt[bias] =    (uartData->angleXYReal[temp][j]/1000%10)<<4;//单位：0.01m
						uartData->txFrameCtnt[bias++] += (uartData->angleXYReal[temp][j]/100%10);
						uartData->txFrameCtnt[bias] =    (uartData->angleXYReal[temp][j]/10%10)<<4;
						uartData->txFrameCtnt[bias++] += (uartData->angleXYReal[temp][j]%10);
						uartData->txFrameCtnt[bias] =    (uartData->angleXYReal[temp][j+1]/1000%10)<<4;//单位：0.01m
						uartData->txFrameCtnt[bias++] += (uartData->angleXYReal[temp][j+1]/100%10);
						uartData->txFrameCtnt[bias] =    (uartData->angleXYReal[temp][j+1]/10%10)<<4;
						uartData->txFrameCtnt[bias++] += (uartData->angleXYReal[temp][j+1]%10);
					}
					uartData->crntHandleSlaveNum = uartData->crntSlaveNum;
					break;
				case STRAIN_CHECK:
					for(i=0; i<cnt_num; i++)
					{
						uartData->txFrameCtnt[bias] =    (uartData->strainFreReal[i][0]/1000%10)<<4;//单位：0.01m
						uartData->txFrameCtnt[bias++] += (uartData->strainFreReal[i][0]/100%10);
						uartData->txFrameCtnt[bias] =    (uartData->strainFreReal[i][0]/10%10)<<4;
						uartData->txFrameCtnt[bias++] += (uartData->strainFreReal[i][0]%10);
						uartData->txFrameCtnt[bias] =    (uartData->strainFreReal[i][1]/1000%10)<<4;//单位：0.1立方米
						uartData->txFrameCtnt[bias++] += (uartData->strainFreReal[i][1]/100%10);
						uartData->txFrameCtnt[bias] =    (uartData->strainFreReal[i][1]/10%10)<<4;
						uartData->txFrameCtnt[bias++] += (uartData->strainFreReal[i][1]%10);
					}
					uartData->crntHandleSlaveNum = 0x20;
					break;
				}
			}
			else if(uartData->frameTypeFlag == SECOND_HALF)
			{
				uartData->frameTypeFlag = FIRST_HALF;
				cnt_num = SLAVE_MAXNUM;
				for(i=48; i<cnt_num; i++)
				{
					uartData->txFrameCtnt[bias] =    (uartData->waterLevelReal[i]/1000%10)<<4;//单位：0.01m
					uartData->txFrameCtnt[bias++] += (uartData->waterLevelReal[i]/100%10);
					uartData->txFrameCtnt[bias] =    (uartData->waterLevelReal[i]/10%10)<<4;
					uartData->txFrameCtnt[bias++] += (uartData->waterLevelReal[i]%10);
					uartData->txFrameCtnt[bias] =    (uartData->waterFlowReal[i]/1000%10)<<4;//单位：0.1立方米
					uartData->txFrameCtnt[bias++] += (uartData->waterFlowReal[i]/100%10);
					uartData->txFrameCtnt[bias] =    (uartData->waterFlowReal[i]/10%10)<<4;
					uartData->txFrameCtnt[bias++] += (uartData->waterFlowReal[i]%10);
				}
				uartData->crntHandleSlaveNum = 0x30;
			}
			uartData->txFrameCtntNum = bias;
			uartData->masterNum = 0;
			result = enFlag;
			break;
		}
	}
	return result;
}
/**
  * @brief  串口接收中断函数，负责处理串口接收到的数据帧的提取、数据内容、相应的回应等.
  * @param  rxSuf-接收数据缓存数组首地址，
  *         length-接收数据的实际长度（不包括帧头和帧尾）
  *         commData-通信数据内容结构体（需要用户自定义）
  * @retval None
  */
e_state UartRcvHandle(char * rxStr,uint8_t length,uartCtntStruct * uartData)
{
	e_state fucFlag = rstFlag;
	fucFlag = uartRcvContentHandle(rxStr,length,uartData);//调用串口内容提取函数  返回是否正确
	if(fucFlag == enFlag)//成功提取出数据帧中的数据内容
	{
		//进行数据处理，此处内容需要复写
		fucFlag = UartRxFctnCtntHandle(uartData);//调用自定义的串口内容处理函数 返回是否需要回应
	}
	return fucFlag;
}

/**
  * @brief  根据当前的状态情况，确定需要发送的数据内容
  * @param  txStr-发送数据帧的首地址
  *         length-发送数据帧字节长度
  *         commData-通信数据内容结构体（需要用户自定义）
  * @retval 无
  */
void UartTxHandle(USART_TypeDef* USARTx,uint8_t * txStr,uartCtntStruct * uartData)
{
	e_state fucFlag = rstFlag;
	uint8_t txLength = 0;
	fucFlag = UartTxFctnCtntHandle(uartData);
	if(fucFlag == enFlag)
	{
		getDataToFrame(&txStr[0],&txLength,\
				       uartData->fucNum,uartData->masterNum,uartData->workTypeArray[uartData->slaveTypeCheckFlag],uartData->crntHandleSlaveNum,\
				       uartData->txFrameCtntNum,&uartData->txFrameCtnt[0]);
		SendStr(USARTx,&txStr[0],txLength);
	}
}
