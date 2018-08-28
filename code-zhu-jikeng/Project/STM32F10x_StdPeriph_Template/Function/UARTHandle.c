#include "UARTHandle.h"
#include "UARTControl.h"


/**
******************************************************************************
* @file    UARTHandle.c
* @author  wqdnan
* @version V3.5.0
* @date    25-12-2017
* @brief   处理接收到的串口数据，并进行数据提取；完成串口发送数据的组帧，并发送
******************************************************************************
* @attention
*
*
*
******************************************************************************
*/

uartCtntStruct uartData;



/**
  * @brief  主机和从机之间的通信交互，主机依次轮询从机号，并获取从机的数据信息.
  * @param  uartData-主从机数据交互结构体
  *         txBuffer-发送数据帧的首地址
  * @retval rstFlag-正常轮询状态，enFlag-轮询完成，且回应超时，需要上报数据给DSP
  */
e_state pollSlaveHandle(uartCtntStruct * slaveData,uint8_t * txBuffer)
{
	uint8_t i = 0;
	e_state fucFlag = enFlag;
	e_state result = rstFlag;
	uint8_t bias = 0;
	uint8_t txDataLength = 0;
	if(slaveData->crntHandleSlaveNum < SLAVE_MAXNUM)
	{
		slaveData->crntHandleSlaveNum ++;
		//to be done
		for(i=0;i<32;i++)
		{
			slaveData->txFrameCtnt[bias++] = slaveData->relayState[i];
		}
		slaveData->fucNum = 0x0D;
		getDataToFrame(&txBuffer[0],&txDataLength,slaveData->fucNum,MASTERNUM,uartData.workTypeArray[uartData.slaveTypeCheckFlag],slaveData->crntHandleSlaveNum,bias,&slaveData->txFrameCtnt[0]);
	}
	else//已经获取完成，并且已经轮询完最后一个从机，但是最后一个从机没有回复，所以，进入这里，要完成数据的上报
	{
		fucFlag = rstFlag;
		result = enFlag;
		slaveData->fucNum = 0x14;
	}
	if(fucFlag == enFlag)
		SendStr(SLAVE_UART,&txBuffer[0],txDataLength);

	return result;
}

/**
  * @brief  主机端根据协议内容进行组帧，包括主从机协议和主机和中心控制室协议内容
  * @param  dest-组帧完成的数据帧的存放地址
  *         destLength-数据帧长度
  *         fctnNum-功能码
  *         masterNum-主机号
  *         slaveType-从机类型
  *         slaveNum-从机号
  *         dataLength-数据内容字节长度
  *         dataCtnt-数据内容存放的首地址
  * @retval None
  * 格式：帧头|功能码|主机编号|从机编号|数据长度|数据内容|校验和|帧尾
  * 字节数    1    1     1       1       1      不定        1     1
  */
void getDataToFrame(uint8_t *dest,uint8_t * destLength,uint8_t fctnNum,uint8_t masterNum,uint8_t slaveType,uint8_t slaveNum,uint8_t dataLength,uint8_t * dataCtnt)
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t temp = 0;
	dest[i++] = FRAME_START;
	dest[i++] = fctnNum;
	dest[i++] = masterNum;
	dest[i++] = slaveType;
	dest[i++] = slaveNum;
	dest[i++] = dataLength;
	for(j=0; j<dataLength; j++)//当dataLength==0时同样适用
		dest[i++] = dataCtnt[j];
	for(j=1; j<i; j++)
		temp ^=  dest[j];
	if((temp == FRAME_START) || (temp == FRAME_END))//校验位与帧头帧尾进行规避
		temp += 1;
	dest[i++] = temp;
	dest[i] = FRAME_END;
	*destLength = i+1;//记录数据帧的长度
}

/**
  * @brief  提取分解出不同数据帧中的数据内容
  * @param  dataCtnt-数据内容存放的首地址
  *         rxStr-处理数据帧的首地址（去掉了帧头和帧尾）
  *         sFctnType-从机的工作状态指示
  *         mFctnType-主机的工作状态指示
  * @retval None
  *         0      1      2       3       4     5
  * 格式：功能码|主机编号|从机类型|从机编号|数据长度|数据内容|校验和
  * 字节数      1     1       1       1      1       不定        1
  */
void analyzeFrameContent(uint8_t * rxStr,uartCtntStruct * uartData)
{
	uint8_t dataLen = 0;
	uint8_t i = 0, j = 0;
	uint16_t dataTemp[40] = {0};
	uint8_t bias = 0;//用于记录手动控制命令数据内容个数的偏移量
	uint8_t * pData = &rxStr[5];//指向数据内容的首地址

	if((rxStr[3] <= SLAVE_MAXNUM) || (rxStr[3] == SLAVEALL))//有效的从机号
	{
		//开始提取从机中的有效数据了
		dataLen = rxStr[4];
		//放入到数据结构体中
		uartData->fucNum = rxStr[0];
		switch(uartData->fucNum)
		{
			case 0x11://全部是阀控制命令，还有倾角标定信息
			{
				if((rxStr[2] == ANGLE_TYPE) && (rxStr[3] == 00))
				{
					uartData->angleCalibration[0] = enFlag;
					uartData->angleCalibration[1] = rxStr[5];
				}
//				for(i=0; i<dataLen; i++)
//					uartData->rxFrameCtnt[bias++] = pData[i];//rxStr[i+5];
				break;
			}
			case 0x0D://按照每两个字节是水位值、流量值或者应力频率值或者倾角x轴角度、y轴角度
			{
				for(i=0; i<dataLen; i++)
				{
					uartData->rxFrameCtnt[bias++] = ((pData[i]&0xF0)>>4)*10+(pData[i]&0x0F);
				}
				uartData->slaveType = rxStr[2];
				uartData->crntSlaveNum = rxStr[3];
				break;
			}
		}
		uartData->rxFrameCtntNum = bias;
	}
}

/**
  * @brief  用于在刚接收到一帧数据后，按照帧格式对数据内容进行提取
  * @param  rxStr-接收数据内容的首地址
  *         length-接收数据的实际长度（去掉了帧头和帧尾）
  *         uartData-串口按照数据帧提取的数据结构体
  * @retval
  *  格式：帧头|功能码|主机编号|从机编号|数据长度|数据内容|校验和|帧尾
  * 字节数    1    1     1       1       1      不定        1     1
  */
e_state uartRcvContentHandle(char * rxStr,uint8_t length,uartCtntStruct * uartData)
{
	e_state result = rstFlag;
	uint8_t i = 0;
	uint8_t tempCheck = 0;
	for(i=0;i<(length-1);i++)
	{
		tempCheck ^= rxStr[i];
	}
	if((tempCheck == FRAME_START) || (tempCheck == FRAME_END))
		tempCheck += 1;
	if(tempCheck == rxStr[length-1])//校验和正确 开始提取数据
	{
		result = enFlag;
		analyzeFrameContent(rxStr,uartData);
	}
	return result;
}
