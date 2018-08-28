/*
 * ModBus.c
 *
 *  Created on: 2018��2��5��
 *      Author: Yu
 */

/* Includes ------------------------------------------------------------------*/
#include  "ModBus.h"
#include  "ModBus_user.h"
//#include "SCI.h"
/** @addtogroup DMA_FSMC
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// ȫ�ֱ�������
Uint16 Mdbs_state;
Uint16 ReceivedChar;

e_EnFlag Mdbs_EnTimerCnt = EnFlagFalse;
Uint16 Mdbs_TimerCnt = 0;
Uint16 Mdbs_EnRcvCheck = EnFlagFalse;

tUartParms UartInfo;				//Uart��ݽṹ
 //tUartParms Uart2Info;				//Uart��ݽṹ
tModbusCount MdbsCNT;

unsigned char MyModbusID = 1;//0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  ��Tim�жϻص������е���,��Ҫ0.1ms���ж�.
  * @param  None
  * @retval None
  */
void ModBus_TIM_Callback(void)
{
	if(Mdbs_EnTimerCnt == EnFlagTrue)
	{
		Mdbs_TimerCnt ++;
		if(Mdbs_TimerCnt > 35)//3.5ms
		{
			Mdbs_TimerCnt = 0;//�����
			Mdbs_EnRcvCheck = EnFlagTrue;//��־λ
			Mdbs_EnTimerCnt = EnFlagFalse;//�ض�ʱ������
		}
	}
}

/**
  * @brief  �ڴ��ڽ����жϻص������е���.
  * @param  None
  * @retval None
  */
void ModBus_Rcv_Callback(unsigned char RcvData)
{
	UartInfo.RxCurData  =  RcvData & 0xff;
	 if(UartInfo.RxTimes == 0)
	{
	    UartInfo.RxData[0] =  UartInfo.RxCurData; //����֡��ַ�ֽڣ��Ȳ������ж�
	    UartInfo.RxAddr    =  UartInfo.RxCurData; //����֡��ַ�ֽ�
	    UartInfo.RxTimes   =  1;                  //�Ѿ�����һ����ַ�ֽ�
//		    (CpuTimer1.RegsAddr)->TCR.bit.TRB = 1;    //��ʱ������װ��
//		    (CpuTimer1.RegsAddr)->TCR.bit.TSS = 0;    //������ʱ��
		Mdbs_TimerCnt = 0;//�����
		Mdbs_EnTimerCnt = EnFlagTrue;//����ʱ������
	}
      else	//�Ѿ�����֡����״̬��
	{
//	         (CpuTimer1.RegsAddr)->TCR.bit.TSS = 1; //�ȹرն�ʱ��
         UartInfo.RxData[UartInfo.RxTimes] = UartInfo.RxCurData; //���յ���ݴ������ݻ�����
		 UartInfo.RxTimes++;
	   if(UartInfo.RxTimes==2)
	   	UartInfo.RxFuncID = UartInfo.RxCurData; //����֡ID��
//		   (CpuTimer1.RegsAddr)->TCR.bit.TRB = 1;     //��ʱ������װ��
//		   (CpuTimer1.RegsAddr)->TCR.bit.TSS = 0;	    //������ÿһ���ֽ�֮������֡��ʱ
		Mdbs_TimerCnt = 0;//�����
		Mdbs_EnTimerCnt = EnFlagTrue;//����ʱ������
	}
}

/**
  * @brief  ��main�������ѭ���е���.
  * @param  None
  * @retval None
  */
void ModBusCheck(void)
{
	//For ModBus
	if(Mdbs_EnRcvCheck == EnFlagTrue)    //֡��ʱ����ʱ,�����Ѿ����ŵ��в�����һ�����֡��֡ͨ��֡����ʱ��ֿ�
	{
		Mdbs_TimerCnt = 0;//�����
		Mdbs_EnRcvCheck = EnFlagFalse;//���־λ
		Mdbs_EnTimerCnt = EnFlagFalse;//�ض�ʱ������
		//-----�����ŵ��в����֡,����֡��״̬����-----
		Mdbs_state = AnalyFrm();
		//-----------�����¼�������-------------
		MdbsCNT.CRCerr_CNT += Mdbs_state&CRC_err;
		MdbsCNT.Addrerr_CNT+= Mdbs_state&ADDR_err;
		MdbsCNT.IDerr_CNT  += Mdbs_state&FuncID_err;
		MdbsCNT.Frabrk_CNT += Mdbs_state&Frame_broken;
		MdbsCNT.Frame_CNT  += Mdbs_state&Frame_OK;    //��¼��ȷ֡��֡��
		//-----------�¼��������������--------------

		//-----------��֡����-------------
		if(Mdbs_state&Frame_OK)
		{
			Deal_OKfrm();         //������ȷ��֡,��������������״̬��Ϣ
					//�Լ���28335ѯ�ʱ�Ƶ���͵���״̬
		}
		//-----------������֮֡����Ҫ�Լ������������㴦��,��֡�ʹ���֡��Ҫ����------
		UartInfo.RxTimes  = 0;
		UartInfo.RxAddr   = 0;
		UartInfo.RxFuncID = 0;
		//-----------֡�������֮�󣬹رն�ʱ�����ȴ��¸�֡�����ֽ�-------


	}//end of if(Mdbs_EnRcvCheck == EnFlagTrue)
}
//============��λ���� T3.5��ʱ��==============
/*
void ResetTimer(void)
{
//       (CpuTimer1.RegsAddr)->TCR.bit.TSS = 0;           //�ر�t3.5������
//	 (CpuTimer1.RegsAddr)->TIM.all     = 0x00000000;  //���t3.5��ʱ��
//	 (CpuTimer1.RegsAddr)->TCR.bit.TRB = 1;            //����t3.5��ʱ��
//	 (CpuTimer1.RegsAddr)->TCR.bit.TIF = 0;           //���T2�жϱ�־
}
*/

//===========�ر�T3.5��ʱ��====================//
/*
void CloseTimer(void)
{

//    (CpuTimer1.RegsAddr)->TCR.bit.TSS = 0;           //�ر�t3.5������
//    (CpuTimer1.RegsAddr)->TIM.all     = 0x00000000;  //���t3.5��ʱ��
//    (CpuTimer1.RegsAddr)->TCR.bit.TIF = 0;           //���T2�жϱ�־
}
*/
//================������������֡=====================
unsigned int AnalyFrm(void)
{
    Uint16  Mdbs;
    unsigned short CRC_cacul;
    Mdbs = 0;
  	if(UartInfo.RxAddr!=MyModbusID)		 			       //�ж�վ��
 			Mdbs =  Mdbs|ADDR_err;
   	if(UartInfo.RxTimes>256)						       //�ж�֡��
		    Mdbs =  Mdbs|Length_err;
      if(UartInfo.RxFuncID>127)   						   //�жϹ�����
   		  	Mdbs =  Mdbs|FuncID_err;
    	 	else      										//��������ȷ���������ж���������֡
		{
			switch(UartInfo.RxFuncID)
			{
				case 0x01:
				case 0x02:
				case 0x03:
				case 0x04:	if(UartInfo.RxTimes!= 8)  Mdbs =  Mdbs|Frame_broken;  break;
				case 0x05:							    						  break;
				case 0x06:							    						  break;
				case 0x0F:							    						  break;
				case 0x10:							    						  break;
				default:							    						  break;
			}
		}

    if(Mdbs == 0)								 //���ǰ��Ĵ���û�У���˶Լ���У����
	{
	    CRC_cacul = CRC16(UartInfo.RxData,UartInfo.RxTimes-2);  //����CRCУ����
		if((UartInfo.RxData[UartInfo.RxTimes-2]<<8|UartInfo.RxData[UartInfo.RxTimes-1])== CRC_cacul)
			Mdbs = Frame_OK;			        //CRCУ�����λ��ǰ����λ�ں�,��Ϊ����ȷ�ϵ���ȷ֡
		else
			Mdbs = Mdbs|CRC_err;
	}
	return 	Mdbs;								  //����Modbus�Ĺ���״̬�������
}



//==============CRCУ������ɺ���=================
//����  У�������ָ�룬У����ݳ���
//����ֵ�����У��ֵ�ͽ��յ���CRCֵ��ͬ����Ϊ��ģ�����Ϊ��
//
unsigned short CRC16(unsigned char *puchMsg,unsigned int usDataLen)                   //                  ��
{
	unsigned char uchCRCHi = 0xFF ; /* ��CRC�ֽڳ�ʼ��  */
	unsigned char uchCRCLo = 0xFF ; /* ��CRC �ֽڳ�ʼ�� */
	unsigned uIndex ;				/* CRCѭ���е�����  */
	while (usDataLen--) 			/* ������Ϣ������   */
	{
		uIndex = uchCRCHi ^ *puchMsg++ ; /* ����CRC */
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex] ;
	}
	return (uchCRCHi << 8 | uchCRCLo) ;

}

//===========�õ���ȷ��֡�󣬽���֡��Ϣ����==============
void Deal_OKfrm(void)
{
    switch(UartInfo.RxFuncID)
 	{
		case 0x01:  Reply_01();	 	 break;    //����Ȧ����(��д����)
		case 0x02:  Reply_02(); 	 break;    //����ɢ������(ֻ������)
		case 0x03:  Reply_03(); 	 break;    //�����ּĴ���(��д����)
		case 0x04:  Reply_04();  	 break;    //������Ĵ���(ֻ������)
		case 0x05:  Reply_05(); 	 break;    //д������Ȧ
		case 0x06:  Reply_06();  	 break;    //д�������ּĴ���
		//case 0x0F:  Reply_0F();  	 break;    //д�����Ȧ
		//case 0x10:  Reply_10();  	 break;    //д������ּĴ���
		default:	  				 break;
	}
}





/////////////////////modbus END//////////////////////////////////////


/**
  * @}
  */ 

/******************* (C) COPYRIGHT YuRobo *****END OF FILE****/
