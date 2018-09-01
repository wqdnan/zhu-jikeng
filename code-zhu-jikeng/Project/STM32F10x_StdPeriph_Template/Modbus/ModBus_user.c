/**
******************************************************************************
* @file    ModBus_user.c
* @author  Yu
* @version V1.0
* @date    2018锟斤拷5锟斤拷31锟斤拷
* @brief   TODO(锟斤拷一锟戒话锟斤拷锟斤拷锟斤拷锟侥硷拷锟斤拷什么)
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




void Reply_01(void) 				//锟斤拷锟斤拷圈锟斤拷锟斤拷
{
	//unsigned int i;
	unsigned short temp;
    //锟斤拷询锟斤拷前锟斤拷位状态锟斤拷
	UartInfo.TxData[0] = MyModbusID;      	//站锟斤拷
	UartInfo.TxData[1] = 0x01;      	//锟截革拷锟斤拷锟斤拷锟斤拷
	UartInfo.TxData[2] = UartInfo.RxData[5]/8; 	//锟街节硷拷锟斤拷(N:N锟斤拷锟街节憋拷示锟斤拷8*N锟斤拷锟斤拷圈状态)             锟斤拷

	UartInfo.TxData[3] =  0xFF;     	//锟斤拷圈状态锟街斤拷1
	UartInfo.TxData[4] =  0xFF;			//锟斤拷圈状态锟街斤拷2

    temp = CRC16(UartInfo.TxData,5);
	UartInfo.TxData[5] = temp/256; //校锟斤拷锟斤拷锟斤拷纸锟�
	UartInfo.TxData[6] = temp%256; //校锟斤拷锟斤拷锟斤拷纸锟�

	ModBus1_send(7);
}

void Reply_02(void)   				//锟斤拷锟斤拷散锟斤拷锟斤拷锟斤拷馗锟斤拷锟斤拷锟�
{
	unsigned short temp;
    //锟斤拷询锟斤拷前锟斤拷位状态锟斤拷
	UartInfo.TxData[0] = MyModbusID;      	//站锟斤拷
	UartInfo.TxData[1] = 0x02;      	//锟截革拷锟斤拷锟斤拷锟斤拷
	UartInfo.TxData[2] = UartInfo.RxData[5]/8; 	//锟街节硷拷锟斤拷(N:N锟斤拷锟街节憋拷示锟斤拷8*N锟斤拷锟斤拷圈状态)

	UartInfo.TxData[3] =  0xFF;     	//锟斤拷圈状态锟街斤拷1
	UartInfo.TxData[4] =  0xFF;			//锟斤拷圈状态锟街斤拷2

    temp = CRC16(UartInfo.TxData,5);
	UartInfo.TxData[5] = temp/256;     //校锟斤拷锟斤拷锟斤拷纸锟�
	UartInfo.TxData[6] = temp%256;     //校锟斤拷锟斤拷锟斤拷纸锟�
	ModBus1_send(7);
}

void Reply_03(void)			      //锟斤拷锟斤拷锟街寄达拷锟斤拷锟斤拷锟侥达拷锟斤拷1为锟借定频锟绞ｏ拷锟侥达拷锟斤拷2为母锟斤拷锟借定锟斤拷压
{
	Uint16 temp;
	Uint16 i = 0;
	Uint16 tmpData = 0;
	unsigned short TxDataCnt = 0;
	if(UartInfo.RxData[0] == MyModbusID)
	{
		UartInfo.TxData[0] = MyModbusID;           //站锟斤拷
		UartInfo.TxData[1] = 0x03;         //锟截革拷锟斤拷锟斤拷锟斤拷
		UartInfo.TxData[2] = (uint8_t)(UartInfo.RxData[4]*256+UartInfo.RxData[5])*2;//锟街节硷拷锟斤拷
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
					temp = UartInfo.RxData[5]/2;
					for(i=0;i<temp;i++)
					{
						if(i<SLAVE_MAXNUM)
						{
							UartInfo.TxData[TxDataCnt] = (uint8_t)((uartData.strainFreReal[i][0]&0xFF00)>>8);
							TxDataCnt ++;
							UartInfo.TxData[TxDataCnt] = (uint8_t)(uartData.strainFreReal[i][0]&0x00FF);
							TxDataCnt ++;
							UartInfo.TxData[TxDataCnt] = (uint8_t)((uartData.strainFreReal[i][1]&0xFF00)>>8);
							TxDataCnt ++;
							UartInfo.TxData[TxDataCnt] = (uint8_t)(uartData.strainFreReal[i][1]&0x00FF);
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
					for(i=0;i<UartInfo.RxData[5];i++)
					{
							UartInfo.TxData[TxDataCnt] = (uint8_t)((uartData.tprtureReal[i][0]&0xFF00)>>8);
							TxDataCnt ++;
							UartInfo.TxData[TxDataCnt] = (uint8_t)(uartData.tprtureReal[i][0]&0x00FF);
							TxDataCnt ++;
					}
					break;
				}
				default: break;
			}
		}
		else//倾角
		{	tmpData -= 0x1000;
			if(tmpData&0x0080)
				temp = 1;//璁＄畻璇㈤棶鐨勪粠鏈哄彿
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

		temp = CRC16(UartInfo.TxData,TxDataCnt);   //锟斤拷锟紺RC校锟斤拷锟斤拷
		UartInfo.TxData[TxDataCnt] = temp/256;TxDataCnt ++;//校锟斤拷锟斤拷锟斤拷纸锟�
		UartInfo.TxData[TxDataCnt] = temp%256;TxDataCnt ++;//校锟斤拷锟斤拷锟斤拷纸锟�
		ModBus1_send(TxDataCnt);
	}







//	UartInfo.TxData[TxDataCnt] = (SysParameterVar.MyID & 0xff00)>>8;TxDataCnt ++;//锟侥达拷锟斤拷1锟斤拷锟斤拷
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
//	UartInfo.TxData[TxDataCnt] = (tmpData & 0xff00)>>8;TxDataCnt ++;//锟侥达拷锟斤拷2锟斤拷锟斤拷
//	UartInfo.TxData[TxDataCnt] = tmpData & 0xff;TxDataCnt ++;
//
//	//3~14: 通锟斤拷1~12锟侥碉拷前压锟斤拷值
//	for(i = 0; i < MAX_CHANNEL_NUM; i ++)
//	{
//		tmpData = SysStateVar.CurrentPressure[i];
//		UartInfo.TxData[TxDataCnt] = (tmpData & 0xff00)>>8;TxDataCnt ++;//锟侥达拷锟斤拷3~14锟斤拷锟斤拷
//		UartInfo.TxData[TxDataCnt] = tmpData & 0xff;TxDataCnt ++;
//	}





}

void Reply_04(void)                      //锟斤拷锟斤拷锟斤拷拇锟斤拷锟�
{
	Uint16 temp;
	Uint16 i = 0;
	Uint16 tmpData = 0;
	unsigned short TxDataCnt = 0;
	UartInfo.TxData[0] = MyModbusID;           //站锟斤拷
	UartInfo.TxData[1] = 0x04;         //锟截革拷锟斤拷锟斤拷锟斤拷
	UartInfo.TxData[2] = (UartInfo.RxData[4]*256+UartInfo.RxData[5])*2;//锟街节硷拷锟斤拷

	TxDataCnt = 3;
//	tmpData = stcAngle.Angle[0]/32768.0*180.0*10.0 + 1800;
//	UartInfo.TxData[TxDataCnt] = (tmpData & 0xff00)>>8;TxDataCnt ++;//锟侥达拷锟斤拷1锟斤拷锟斤拷
//	UartInfo.TxData[TxDataCnt] = tmpData & 0xff;TxDataCnt ++;
//
//	tmpData = stcAngle.Angle[1]/32768.0*180.0*10.0 + 1800;
//	UartInfo.TxData[TxDataCnt] = (tmpData & 0xff00)>>8;TxDataCnt ++;//锟侥达拷锟斤拷2锟斤拷锟斤拷
//	UartInfo.TxData[TxDataCnt] = tmpData & 0xff;TxDataCnt ++;


    temp = CRC16(UartInfo.TxData,TxDataCnt);   //锟斤拷锟紺RC校锟斤拷锟斤拷
    UartInfo.TxData[TxDataCnt] = temp/256;TxDataCnt ++;//校锟斤拷锟斤拷锟斤拷纸锟�
	UartInfo.TxData[TxDataCnt] = temp%256;TxDataCnt ++;//校锟斤拷锟斤拷锟斤拷纸锟�

	ModBus1_send(TxDataCnt);

}

void Reply_05(void)            		    //写锟斤拷锟斤拷锟斤拷圈锟斤拷锟斤拷
{
	unsigned short temp;

    switch(	UartInfo.RxData[2]*256+	UartInfo.RxData[3])
    {
   	  case 0:      break;         //锟斤拷应1锟斤拷锟斤拷圈锟斤拷址锟斤拷"锟斤拷锟斤拷"锟斤拷钮
	  case 1:      break;         //锟斤拷应2锟斤拷锟斤拷圈锟斤拷址锟斤拷锟斤拷停止锟斤拷锟斤拷钮
        default:    									break;
	}

    UartInfo.TxData[0] = MyModbusID;         		    //站锟斤拷
    UartInfo.TxData[1] = 0x05;         		    //锟斤拷锟斤拷锟斤拷
    UartInfo.TxData[2] = UartInfo.RxData[2];        //锟斤拷址锟斤拷位
    UartInfo.TxData[3] = UartInfo.RxData[3];        //锟斤拷址锟斤拷位
    UartInfo.TxData[4] = UartInfo.RxData[4];        //锟斤拷锟街碉拷锟轿�
    UartInfo.TxData[5] = UartInfo.RxData[5];        //锟斤拷锟街碉拷锟轿�

	temp = CRC16(UartInfo.TxData,6);   		    //锟斤拷锟紺RC校锟斤拷锟斤拷
	UartInfo.TxData[6] = temp/256;     		    //校锟斤拷锟斤拷锟斤拷纸锟�
	UartInfo.TxData[7] = temp%256;     		    //校锟斤拷锟斤拷锟斤拷纸锟�

	ModBus1_send(8); 					    //锟斤拷锟斤拷
}

void Reply_06(void)					    //写锟斤拷锟斤拷锟侥达拷锟斤拷锟斤拷锟斤拷
{
	unsigned short temp;
	switch(	UartInfo.RxData[2]*256+	UartInfo.RxData[3])
	{
		case 0:
			 	 break; 				//锟借定频锟绞寄达拷锟斤拷
		case 1:
				 break;				//母锟竭碉拷压锟侥达拷锟斤拷
	   default:  break;
	}

    UartInfo.TxData[0] = MyModbusID;         		    //站锟斤拷
    UartInfo.TxData[1] = 0x06;         		    //锟斤拷锟斤拷锟斤拷
    UartInfo.TxData[2] = UartInfo.RxData[2];        //锟斤拷址锟斤拷位
    UartInfo.TxData[3] = UartInfo.RxData[3];        //锟斤拷址锟斤拷位
    UartInfo.TxData[4] = UartInfo.RxData[4];        //锟斤拷锟街碉拷锟轿�
    UartInfo.TxData[5] = UartInfo.RxData[5];        //锟斤拷锟街碉拷锟轿�

    temp = CRC16(UartInfo.TxData,6);   				//锟斤拷锟紺RC校锟斤拷锟斤拷
    UartInfo.TxData[6] = temp/256;     				//校锟斤拷锟斤拷锟斤拷纸锟�
    UartInfo.TxData[7] = temp%256;     				//校锟斤拷锟斤拷锟斤拷纸锟�

    ModBus1_send(8); 									//锟斤拷锟斤拷
}

/**
  * @}
  */ 

/******************* (C) COPYRIGHT YuRobo *****END OF FILE****/
