/*
 * ModBus.h
 *
 *  Created on: 2018��2��5��
 *      Author: Yu
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MODBUS_H_
#define MODBUS_H_
/* Includes ------------------------------------------------------------------*/
//#include "Process.h"//e_EnFlag�ṹ��
//For Modbus
//#include  "variablelist.h"      //������Ҫ�õ���ȫ�ֱ�������
#include  "CRCtable.h"          //CRC����У�����
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
} e_EnFlag;//����ָʾʼ�ܱ�־λ

//================================ Uart�����ṹ�嶨�� ===============================
typedef struct UartParms
{
	unsigned char RxCurData;            //���յĵ�ǰ����
    unsigned char RxAddr;               //֡��ַ
    unsigned char RxFuncID;				//���յĹ������ʶ
    unsigned char RxData[MaxLen];    	//�������ݻ���
    unsigned int  RxTimes;              //�����ֽڼ���
    unsigned int  RxCRC;           		//У���CRCУ����

    unsigned char TxCurData;            //���յĵ�ǰ����
    unsigned char TxAddr;				//���͵Ĺ������ʶ
	unsigned char TxFuncID;
    unsigned char TxData[MaxLen];       //�����ֽڼ���
    unsigned int  TxTimes;              //�����ֽڼ���
    unsigned int  TxCRC;			    //�������ݵ�CRCУ����
} tUartParms;
/*---------------�첽ͨ�ſ�1�ṹ�嶨�����---------------*/

//=================================�¼������ṹ��=======================================
typedef struct tagModbusCount
{
unsigned int CRCerr_CNT;   		//CRC֡У��������
unsigned int Chara_OvrTime;     //�ַ���ʱ��������
unsigned int Addrerr_CNT;       //��ַ�������
unsigned int IDerr_CNT;         //ID�������
unsigned int Frabrk_CNT;	    //����֡����
unsigned int Frame_CNT; 		//���յ�����֡֡��
} tModbusCount;

/*---------------�¼������ṹ�嶨�����---------------*/

#define HIVaddr    0x01  //��Ƶ����ַ
//modbus������״̬
#define CRC_err      0x0001 //���յ�������֡��У�����  0000 0001
#define ADDR_err     0x0002 //֡��ַ����ȷ			  0000 0010
#define Length_err   0x0004 //֡��������׼			  0000 0100
#define FuncID_err 	 0x0008 //֡IDԽ�����			  0000 1000
#define Frame_broken 0x0010 //֡�ǲ��Ƶ�				  0001 0000
#define Frame_OK	 0x0020 //֡��ȷ 					  0010 0000

//��ֵ����
#define True   1
#define False  0
/* Exported macro ------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
extern Uint16 Mdbs_state;
extern e_EnFlag Mdbs_EnTimerCnt;
extern Uint16 Mdbs_TimerCnt;
extern Uint16 Mdbs_EnRcvCheck;

extern tUartParms UartInfo;				//Uart���ݽṹ
 //tUartParms Uart2Info;				//Uart���ݽṹ
extern tModbusCount MdbsCNT;

extern unsigned char MyModbusID;
/* Exported functions ------------------------------------------------------- */

void ModBus_TIM_Callback(void);
void ModBus_Rcv_Callback(unsigned char RcvData);
void ModBusCheck(void);
unsigned short CRC16(unsigned char *puchMsg,unsigned int DataLen); //CRCУ�������ɺ���
unsigned int AnalyFrm(void);  //�������񵽵�֡
void Deal_OKfrm(void);        //������У����֡



#endif /* MODBUS_H_ */
