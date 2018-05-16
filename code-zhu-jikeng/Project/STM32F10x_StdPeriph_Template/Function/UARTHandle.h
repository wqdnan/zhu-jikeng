#ifndef __UARTHandle_H
#define __UARTHandle_H

#include "UART.h"
#include <String.h>




#define WATER_DEPTH_CHECK 1  //水位监测
#define ANGLE_CHECK 2        //倾角监测
#define STRAIN_CHECK 3       //应力计检测


#define FUCMAXNUM 20

#define SLAVEALL  0x10          //所有从机号，包括0~SLAVEMAXNUM-1

typedef enum{FIRST_HALF,SECOND_HALF}frameTypeEnum;

typedef struct
{

	uint16_t waterLevelReal[SLAVE_MAXNUM];      //水位实时值   直接就是接收到的数据  在传递中比实际数值都扩大了100倍 单位：0.01m
	uint16_t waterFlowReal[SLAVE_MAXNUM];       //流量实时值，扩大10倍 单位：0.1立方米
	uint16_t strainFreReal[SLAVE_MAXNUM][2];    //应力计频率值，0-频率1,1-频率2
	uint16_t angleXYReal[SLAVE_MAXNUM][ANGLE_CHECK_NUM];     //倾角角度值，偶数下标-x倾角，奇数下标-y倾角
	uint8_t relayState[32];                    //记录继电器动作状态 0~4位对应Q1~Q5状态  0-断开  1-闭合
	uint8_t slaveType;                         //当前接收到的从机类型
	uint8_t crntSlaveNum;                      //当前接收到的从机号
	uint8_t fucNum;                            //功能码
	uint8_t masterNum;                         //主机编号，大部分时间都是0x1F,当stm32->DSP 为00
	uint8_t rxFrameCtnt[33];                   //接收组帧数据内容
	uint8_t rxFrameCtntNum;                     //接收组帧数据内容字节长度
	uint8_t txFrameCtnt[192];                   //发送组帧数据内容
	uint8_t txFrameCtntNum;                     //发送组帧数据内容字节长度
	frameTypeEnum frameTypeFlag;                //发送帧类型 前一半-FIRST_HALF，后一半-SECOND_HALF
	uint8_t crntHandleSlaveNum;                //在从机轮询中，操作的从机号
	e_state slaveTransFlag;                    //与从机的通信持续标志 enFlag-还需继续通信，rstFlag-可以通信结束了
	uint8_t slaveTypeCheckFlag;                //当有多个从机类型时，用于主机按照顺序做轮询用
	uint8_t workTypeArray[5];                  //用于存放轮询的从机类型
}uartCtntStruct;//定义主机和从机交互的数据结构

extern uartCtntStruct uartData;

e_state pollSlaveHandle(uartCtntStruct * slaveData,uint8_t * txBuffer);
void analyzeFrameContent(uint8_t * rxStr,uartCtntStruct * uartData);
e_state uartRcvContentHandle(char * rxStr,uint8_t length,uartCtntStruct * uartData);
void getDataToFrame(uint8_t *dest,uint8_t * destLength,uint8_t fctnNum,uint8_t masterNum,uint8_t slaveType,uint8_t slaveNum,uint8_t dataLength,uint8_t * dataCtnt);



#endif
