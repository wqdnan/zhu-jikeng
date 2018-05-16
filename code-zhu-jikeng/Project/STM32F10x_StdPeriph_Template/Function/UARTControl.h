#ifndef __UARTCONTROL_H
#define __UARTCONTROL_H

#include "main.h"
#include "UARTHandle.h"
#include "UART.h"



void wifiTxHandle(uartCtntStruct *uartData,uint8_t *txBuf);
void regularTimeDo(uartCtntStruct * uartData);
e_state slaveTopCtrl(uartCtntStruct * uartData,uint8_t * txBuffer);

e_state setParaData(uartCtntStruct * uartData);
e_state setCtrlData(uartCtntStruct * uartData);
void mobileDataHandle(uartCtntStruct * uartData);
/* 接收到的数据内容进行功能处理 */
e_state UartRxFctnCtntHandle(uartCtntStruct *uartData);
/* 待发送的数据帧准备处理 */
e_state UartTxFctnCtntHandle(uartCtntStruct *uartData);


e_state UartRcvHandle(char * rxStr,uint8_t length,uartCtntStruct * uartData);
void UartTxHandle(USART_TypeDef* USARTx,uint8_t * txStr,uartCtntStruct * uartData);
#endif
