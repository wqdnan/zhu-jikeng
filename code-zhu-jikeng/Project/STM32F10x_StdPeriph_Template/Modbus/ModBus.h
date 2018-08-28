/*
 * ModBus.h
 *
 *  Created on: 2018年2月5日
 *      Author: Yu
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MODBUS_H_
#define MODBUS_H_
/* Includes ------------------------------------------------------------------*/
//#include "Process.h"//e_EnFlag结构体
//For Modbus
//#include  "variablelist.h"      //触摸屏要用到的全局变量声明
#include  "CRCtable.h"          //CRC冗余校验码表
#include  "ModBus_bsp.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
#define MaxLen  99

typedef enum
{
	EnFlagFalse = 0x33,//False
	EnFlagTrue = 0x55,//True
	UnWantedEnFlag = 0x72//error
} e_EnFlag;//用于指示始能标志位

//================================ Uart参数结构体定义 ===============================
typedef struct UartParms
{
	unsigned char RxCurData;            //接收的当前数据
    unsigned char RxAddr;               //帧地址
    unsigned char RxFuncID;				//接收的功能码标识
    unsigned char RxData[MaxLen];    	//接收数据缓存
    unsigned int  RxTimes;              //接收字节计数
    unsigned int  RxCRC;           		//校验和CRC校验码

    unsigned char TxCurData;            //接收的当前数据
    unsigned char TxAddr;				//发送的功能码标识
	unsigned char TxFuncID;
    unsigned char TxData[MaxLen];       //发送字节计数
    unsigned int  TxTimes;              //发送字节计数
    unsigned int  TxCRC;			    //发送数据的CRC校验码
} tUartParms;
/*---------------异步通信口1结构体定义结束---------------*/

//=================================事件计数结构体=======================================
typedef struct tagModbusCount
{
unsigned int CRCerr_CNT;   		//CRC帧校验出错次数
unsigned int Chara_OvrTime;     //字符超时次数计数
unsigned int Addrerr_CNT;       //地址出错次数
unsigned int IDerr_CNT;         //ID出错次数
unsigned int Frabrk_CNT;	    //残破帧次数
unsigned int Frame_CNT; 		//接收的数据帧帧数
} tModbusCount;

/*---------------事件计数结构体定义结束---------------*/

#define HIVaddr    0x01  //变频器地址
//modbus所处的状态
#define CRC_err      0x0001 //接收到完整的帧后校验错误  0000 0001
#define ADDR_err     0x0002 //帧地址不正确			  0000 0010
#define Length_err   0x0004 //帧长超过标准			  0000 0100
#define FuncID_err 	 0x0008 //帧ID越界错误			  0000 1000
#define Frame_broken 0x0010 //帧是残破的				  0001 0000
#define Frame_OK	 0x0020 //帧正确 					  0010 0000

//数值常数
#define True   1
#define False  0
/* Exported macro ------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
extern Uint16 Mdbs_state;
extern e_EnFlag Mdbs_EnTimerCnt;
extern Uint16 Mdbs_TimerCnt;
extern Uint16 Mdbs_EnRcvCheck;

extern tUartParms UartInfo;				//Uart数据结构
 //tUartParms Uart2Info;				//Uart数据结构
extern tModbusCount MdbsCNT;

extern unsigned char MyModbusID;
/* Exported functions ------------------------------------------------------- */

void ModBus_TIM_Callback(void);
void ModBus_Rcv_Callback(unsigned char RcvData);
void ModBusCheck(void);
unsigned short CRC16(unsigned char *puchMsg,unsigned int DataLen); //CRC校验码生成函数
unsigned int AnalyFrm(void);  //分析捕获到的帧
void Deal_OKfrm(void);        //处理经过校验后的帧



#endif /* MODBUS_H_ */
