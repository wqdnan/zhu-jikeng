/**
******************************************************************************
* @file    ModBus_user.c
* @author  Yu
* @version V1.0
* @date    2018��5��31��
* @brief   TODO(��һ�仰�������ļ���ʲô)
******************************************************************************
* @attention
*
*
*
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include  "ModBus.h"
#include  "ModBus_user.h"
#include  "ModBus_bsp.h"
#include "UARTHandle.h"
/** @addtogroup DMA_FSMC
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




void Reply_01(void) 				//����Ȧ����
{
	//unsigned int i;
	unsigned short temp;
    //��ѯ��ǰ��λ״̬��
	UartInfo.TxData[0] = MyModbusID;      	//վ��
	UartInfo.TxData[1] = 0x01;      	//�ظ�������
	UartInfo.TxData[2] = UartInfo.RxData[5]/8; 	//�ֽڼ���(N:N���ֽڱ�ʾ��8*N����Ȧ״̬)             ��

	UartInfo.TxData[3] =  0xFF;     	//��Ȧ״̬�ֽ�1
	UartInfo.TxData[4] =  0xFF;			//��Ȧ״̬�ֽ�2

    temp = CRC16(UartInfo.TxData,5);
	UartInfo.TxData[5] = temp/256; //У������ֽ�
	UartInfo.TxData[6] = temp%256; //У������ֽ�

	ModBus1_send(7);
}

void Reply_02(void)   				//����ɢ������ظ�����
{
	unsigned short temp;
    //��ѯ��ǰ��λ״̬��
	UartInfo.TxData[0] = MyModbusID;      	//վ��
	UartInfo.TxData[1] = 0x02;      	//�ظ�������
	UartInfo.TxData[2] = UartInfo.RxData[5]/8; 	//�ֽڼ���(N:N���ֽڱ�ʾ��8*N����Ȧ״̬)

	UartInfo.TxData[3] =  0xFF;     	//��Ȧ״̬�ֽ�1
	UartInfo.TxData[4] =  0xFF;			//��Ȧ״̬�ֽ�2

    temp = CRC16(UartInfo.TxData,5);
	UartInfo.TxData[5] = temp/256;     //У������ֽ�
	UartInfo.TxData[6] = temp%256;     //У������ֽ�
	ModBus1_send(7);
}

void Reply_03(void)			      //�����ּĴ������Ĵ���1Ϊ�趨Ƶ�ʣ��Ĵ���2Ϊĸ���趨��ѹ
{
	Uint16 temp;
	Uint16 i = 0;
	Uint16 tmpData = 0;
	unsigned short TxDataCnt = 0;
	if(UartInfo.RxData[0] == MyModbusID)
	{
		UartInfo.TxData[0] = MyModbusID;           //վ��
		UartInfo.TxData[1] = 0x03;         //�ظ�������
		UartInfo.TxData[2] = (uint8_t)(UartInfo.RxData[4]*256+UartInfo.RxData[5])*2;//�ֽڼ���
		TxDataCnt = 3;
		tmpData = (UartInfo.RxData[2]*256+UartInfo.RxData[3]);
		if(tmpData < 0x1000)
		{
			switch(tmpData)
			{
				case 0x0000://水位
				{
					for(i=0;i<UartInfo.RxData[5];i++)
					{
						if(i<SLAVE_MAXNUM)
						{
							UartInfo.TxData[TxDataCnt] = (uint8_t)((uartData.waterLevelReal[i]&0xFF00)>>8);
							TxDataCnt ++;
							UartInfo.TxData[TxDataCnt] = (uint8_t)(uartData.waterLevelReal[i]&0x00FF);
							TxDataCnt ++;
						}

					}
					break;
				}
				case 0x0100://应力
				{
					for(i=0;i<UartInfo.RxData[5];i++)
					{
						if(i<SLAVE_MAXNUM)
						{
							UartInfo.TxData[TxDataCnt] = (uint8_t)((uartData.strainFreReal[i][0]&0xFF00)>>8);
							TxDataCnt ++;
							UartInfo.TxData[TxDataCnt] = (uint8_t)(uartData.strainFreReal[i][0]&0x00FF);
							TxDataCnt ++;
						}

					}
					break;
				}
				case 0x0200://沉降
				{
					break;
				}
				case 0x0300://温度
				{
					break;
				}
				default: break;
			}
		}
		else//是倾角
		{	tmpData -= 0x1000;
			if(tmpData&0x0080)
				temp = 1;//计算询问的从机号
			else
				temp = 0;
			temp += ((tmpData&0xF000)>>12)*30;
			temp += ((tmpData&0x0F00)>>8)*2;
			if((temp < SLAVE_MAXNUM) && (UartInfo.RxData[5] < (ANGLE_CHECK_NUM*2)))
			{
				tmpData = UartInfo.RxData[5];
				for(i=0;i<tmpData;i++)
				{
					UartInfo.TxData[TxDataCnt] = (uint8_t)((uartData.angleXYReal[temp][i]&0xFF00)>>8);
					TxDataCnt ++;
					UartInfo.TxData[TxDataCnt] = (uint8_t)(uartData.angleXYReal[temp][i]&0x00FF);
					TxDataCnt ++;
				}
			}

		}

		temp = CRC16(UartInfo.TxData,TxDataCnt);   //���CRCУ����
		UartInfo.TxData[TxDataCnt] = temp/256;TxDataCnt ++;//У������ֽ�
		UartInfo.TxData[TxDataCnt] = temp%256;TxDataCnt ++;//У������ֽ�
		ModBus1_send(TxDataCnt);
	}







//	UartInfo.TxData[TxDataCnt] = (SysParameterVar.MyID & 0xff00)>>8;TxDataCnt ++;//�Ĵ���1����
//	UartInfo.TxData[TxDataCnt] = SysParameterVar.MyID & 0xff;TxDataCnt ++;
//
//	tmpData = 0;
//	for(i = 0; i < MAX_CHANNEL_NUM; i ++)
//	{
//		tmpData = tmpData << 1;
//		if(SysParameterVar.EnChannelMask[i] == EnFlagFalse)
//		{
//			tmpData += 1;
//		}
//	}
//	UartInfo.TxData[TxDataCnt] = (tmpData & 0xff00)>>8;TxDataCnt ++;//�Ĵ���2����
//	UartInfo.TxData[TxDataCnt] = tmpData & 0xff;TxDataCnt ++;
//
//	//3~14: ͨ��1~12�ĵ�ǰѹ��ֵ
//	for(i = 0; i < MAX_CHANNEL_NUM; i ++)
//	{
//		tmpData = SysStateVar.CurrentPressure[i];
//		UartInfo.TxData[TxDataCnt] = (tmpData & 0xff00)>>8;TxDataCnt ++;//�Ĵ���3~14����
//		UartInfo.TxData[TxDataCnt] = tmpData & 0xff;TxDataCnt ++;
//	}





}

void Reply_04(void)                      //������Ĵ���
{
	Uint16 temp;
	Uint16 i = 0;
	Uint16 tmpData = 0;
	unsigned short TxDataCnt = 0;
	UartInfo.TxData[0] = MyModbusID;           //վ��
	UartInfo.TxData[1] = 0x04;         //�ظ�������
	UartInfo.TxData[2] = (UartInfo.RxData[4]*256+UartInfo.RxData[5])*2;//�ֽڼ���

	TxDataCnt = 3;
//	tmpData = stcAngle.Angle[0]/32768.0*180.0*10.0 + 1800;
//	UartInfo.TxData[TxDataCnt] = (tmpData & 0xff00)>>8;TxDataCnt ++;//�Ĵ���1����
//	UartInfo.TxData[TxDataCnt] = tmpData & 0xff;TxDataCnt ++;
//
//	tmpData = stcAngle.Angle[1]/32768.0*180.0*10.0 + 1800;
//	UartInfo.TxData[TxDataCnt] = (tmpData & 0xff00)>>8;TxDataCnt ++;//�Ĵ���2����
//	UartInfo.TxData[TxDataCnt] = tmpData & 0xff;TxDataCnt ++;


    temp = CRC16(UartInfo.TxData,TxDataCnt);   //���CRCУ����
    UartInfo.TxData[TxDataCnt] = temp/256;TxDataCnt ++;//У������ֽ�
	UartInfo.TxData[TxDataCnt] = temp%256;TxDataCnt ++;//У������ֽ�

	ModBus1_send(TxDataCnt);

}

void Reply_05(void)            		    //д������Ȧ����
{
	unsigned short temp;

    switch(	UartInfo.RxData[2]*256+	UartInfo.RxData[3])
    {
   	  case 0:      break;         //��Ӧ1����Ȧ��ַ��"����"��ť
	  case 1:      break;         //��Ӧ2����Ȧ��ַ����ֹͣ����ť
        default:    									break;
	}

    UartInfo.TxData[0] = MyModbusID;         		    //վ��
    UartInfo.TxData[1] = 0x05;         		    //������
    UartInfo.TxData[2] = UartInfo.RxData[2];        //��ַ��λ
    UartInfo.TxData[3] = UartInfo.RxData[3];        //��ַ��λ
    UartInfo.TxData[4] = UartInfo.RxData[4];        //���ֵ��λ
    UartInfo.TxData[5] = UartInfo.RxData[5];        //���ֵ��λ

	temp = CRC16(UartInfo.TxData,6);   		    //���CRCУ����
	UartInfo.TxData[6] = temp/256;     		    //У������ֽ�
	UartInfo.TxData[7] = temp%256;     		    //У������ֽ�

	ModBus1_send(8); 					    //����
}

void Reply_06(void)					    //д�����Ĵ�������
{
	unsigned short temp;
	switch(	UartInfo.RxData[2]*256+	UartInfo.RxData[3])
	{
		case 0:
			 	 break; 				//�趨Ƶ�ʼĴ���
		case 1:
				 break;				//ĸ�ߵ�ѹ�Ĵ���
	   default:  break;
	}

    UartInfo.TxData[0] = MyModbusID;         		    //վ��
    UartInfo.TxData[1] = 0x06;         		    //������
    UartInfo.TxData[2] = UartInfo.RxData[2];        //��ַ��λ
    UartInfo.TxData[3] = UartInfo.RxData[3];        //��ַ��λ
    UartInfo.TxData[4] = UartInfo.RxData[4];        //���ֵ��λ
    UartInfo.TxData[5] = UartInfo.RxData[5];        //���ֵ��λ

    temp = CRC16(UartInfo.TxData,6);   				//���CRCУ����
    UartInfo.TxData[6] = temp/256;     				//У������ֽ�
    UartInfo.TxData[7] = temp%256;     				//У������ֽ�

    ModBus1_send(8); 									//����
}

/**
  * @}
  */ 

/******************* (C) COPYRIGHT YuRobo *****END OF FILE****/
