/*
 * ModBus.c
 *
 *  Created on: 2018锟斤拷2锟斤拷5锟斤拷
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

// 全锟街憋拷锟斤拷锟斤拷锟斤拷
Uint16 Mdbs_state;
Uint16 ReceivedChar;

e_EnFlag Mdbs_EnTimerCnt = EnFlagFalse;
Uint16 Mdbs_TimerCnt = 0;
Uint16 Mdbs_EnRcvCheck = EnFlagFalse;

tUartParms UartInfo;				//Uart锟斤拷萁峁�
 //tUartParms Uart2Info;				//Uart锟斤拷萁峁�
tModbusCount MdbsCNT;

unsigned char MyModbusID = 1;//0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  锟斤拷Tim锟叫断回碉拷锟斤拷锟斤拷锟叫碉拷锟斤拷,锟斤拷要0.1ms锟斤拷锟叫讹拷.
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
			Mdbs_TimerCnt = 0;//锟斤拷锟斤拷锟�
			Mdbs_EnRcvCheck = EnFlagTrue;//锟斤拷志位
			Mdbs_EnTimerCnt = EnFlagFalse;//锟截讹拷时锟斤拷锟斤拷锟斤拷
		}
	}
}

/**
  * @brief  锟节达拷锟节斤拷锟斤拷锟叫断回碉拷锟斤拷锟斤拷锟叫碉拷锟斤拷.
  * @param  None
  * @retval None
  */
void ModBus_Rcv_Callback(unsigned char RcvData)
{
	UartInfo.RxCurData  =  RcvData & 0xff;
	 if(UartInfo.RxTimes == 0)
	{
	    UartInfo.RxData[0] =  UartInfo.RxCurData; //锟斤拷锟斤拷帧锟斤拷址锟街节ｏ拷锟饺诧拷锟斤拷锟斤拷锟叫讹拷
	    UartInfo.RxAddr    =  UartInfo.RxCurData; //锟斤拷锟斤拷帧锟斤拷址锟街斤拷
	    UartInfo.RxTimes   =  1;                  //锟窖撅拷锟斤拷锟斤拷一锟斤拷锟斤拷址锟街斤拷
//		    (CpuTimer1.RegsAddr)->TCR.bit.TRB = 1;    //锟斤拷时锟斤拷锟斤拷锟斤拷装锟斤拷
//		    (CpuTimer1.RegsAddr)->TCR.bit.TSS = 0;    //锟斤拷锟斤拷锟斤拷时锟斤拷
		Mdbs_TimerCnt = 0;//锟斤拷锟斤拷锟�
		Mdbs_EnTimerCnt = EnFlagTrue;//锟斤拷锟斤拷时锟斤拷锟斤拷锟斤拷
	}
      else	//锟窖撅拷锟斤拷锟斤拷帧锟斤拷锟斤拷状态锟斤拷
	{
//	         (CpuTimer1.RegsAddr)->TCR.bit.TSS = 1; //锟饺关闭讹拷时锟斤拷
         UartInfo.RxData[UartInfo.RxTimes] = UartInfo.RxCurData; //锟斤拷锟秸碉拷锟斤拷荽锟斤拷锟斤拷锟斤拷莼锟斤拷锟斤拷锟�
		 UartInfo.RxTimes++;
	   if(UartInfo.RxTimes==2)
	   	UartInfo.RxFuncID = UartInfo.RxCurData; //锟斤拷锟斤拷帧ID锟斤拷
//		   (CpuTimer1.RegsAddr)->TCR.bit.TRB = 1;     //锟斤拷时锟斤拷锟斤拷锟斤拷装锟斤拷
//		   (CpuTimer1.RegsAddr)->TCR.bit.TSS = 0;	    //锟斤拷锟斤拷锟斤拷每一锟斤拷锟街斤拷之锟斤拷锟斤拷锟斤拷帧锟斤拷时
		Mdbs_TimerCnt = 0;//锟斤拷锟斤拷锟�
		Mdbs_EnTimerCnt = EnFlagTrue;//锟斤拷锟斤拷时锟斤拷锟斤拷锟斤拷
	}
}

/**
  * @brief  锟斤拷main锟斤拷锟斤拷锟斤拷锟窖拷锟斤拷械锟斤拷锟�.
  * @param  None
  * @retval None
  */
void ModBusCheck(void)
{
	//For ModBus
	if(Mdbs_EnRcvCheck == EnFlagTrue)    //帧锟斤拷时锟斤拷锟斤拷时,锟斤拷锟斤拷锟窖撅拷锟斤拷锟脚碉拷锟叫诧拷锟斤拷锟斤拷一锟斤拷锟斤拷锟街★拷锟街⊥拷锟街★拷锟斤拷锟绞憋拷锟街匡拷
	{
		Mdbs_TimerCnt = 0;//锟斤拷锟斤拷锟�
		Mdbs_EnRcvCheck = EnFlagFalse;//锟斤拷锟街疚�
		Mdbs_EnTimerCnt = EnFlagFalse;//锟截讹拷时锟斤拷锟斤拷锟斤拷
		//-----锟斤拷锟斤拷锟脚碉拷锟叫诧拷锟斤拷锟街�,锟斤拷锟斤拷帧锟斤拷状态锟斤拷锟斤拷-----
		Mdbs_state = AnalyFrm();
		//-----------锟斤拷锟斤拷锟铰硷拷锟斤拷锟斤拷锟斤拷-------------
		MdbsCNT.CRCerr_CNT += Mdbs_state&CRC_err;
		MdbsCNT.Addrerr_CNT+= Mdbs_state&ADDR_err;
		MdbsCNT.IDerr_CNT  += Mdbs_state&FuncID_err;
		MdbsCNT.Frabrk_CNT += Mdbs_state&Frame_broken;
		MdbsCNT.Frame_CNT  += Mdbs_state&Frame_OK;    //锟斤拷录锟斤拷确帧锟斤拷帧锟斤拷
		//-----------锟铰硷拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�--------------

		//-----------锟斤拷帧锟斤拷锟斤拷-------------
		if(Mdbs_state&Frame_OK)
		{
			Deal_OKfrm();         //锟斤拷锟斤拷锟斤拷确锟斤拷帧,锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷状态锟斤拷息
					//锟皆硷拷锟斤拷28335询锟绞憋拷频锟斤拷锟酵碉拷锟斤拷状态
		}
		//-----------锟斤拷锟斤拷锟斤拷帧之锟斤拷锟斤拷要锟皆硷拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟姐处锟斤拷,锟斤拷帧锟酵达拷锟斤拷帧锟斤拷要锟斤拷锟斤拷------
		UartInfo.RxTimes  = 0;
		UartInfo.RxAddr   = 0;
		UartInfo.RxFuncID = 0;
		//-----------帧锟斤拷锟斤拷锟斤拷锟街拷螅乇斩锟绞憋拷锟斤拷锟斤拷却锟斤拷赂锟街★拷锟斤拷锟斤拷纸锟�-------


	}//end of if(Mdbs_EnRcvCheck == EnFlagTrue)
}
//============锟斤拷位锟斤拷锟斤拷 T3.5锟斤拷时锟斤拷==============
/*
void ResetTimer(void)
{
//       (CpuTimer1.RegsAddr)->TCR.bit.TSS = 0;           //锟截憋拷t3.5锟斤拷锟斤拷锟斤拷
//	 (CpuTimer1.RegsAddr)->TIM.all     = 0x00000000;  //锟斤拷锟絫3.5锟斤拷时锟斤拷
//	 (CpuTimer1.RegsAddr)->TCR.bit.TRB = 1;            //锟斤拷锟斤拷t3.5锟斤拷时锟斤拷
//	 (CpuTimer1.RegsAddr)->TCR.bit.TIF = 0;           //锟斤拷锟絋2锟叫断憋拷志
}
*/

//===========锟截憋拷T3.5锟斤拷时锟斤拷====================//
/*
void CloseTimer(void)
{

//    (CpuTimer1.RegsAddr)->TCR.bit.TSS = 0;           //锟截憋拷t3.5锟斤拷锟斤拷锟斤拷
//    (CpuTimer1.RegsAddr)->TIM.all     = 0x00000000;  //锟斤拷锟絫3.5锟斤拷时锟斤拷
//    (CpuTimer1.RegsAddr)->TCR.bit.TIF = 0;           //锟斤拷锟絋2锟叫断憋拷志
}
*/
//================锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷帧=====================
unsigned int AnalyFrm(void)
{
    Uint16  Mdbs;
    unsigned short CRC_cacul;
    Mdbs = 0;
  	if(UartInfo.RxAddr!=MyModbusID)		 			       //锟叫讹拷站锟斤拷
 			Mdbs =  Mdbs|ADDR_err;
   	if(UartInfo.RxTimes>256)						       //锟叫讹拷帧锟斤拷
		    Mdbs =  Mdbs|Length_err;
      if(UartInfo.RxFuncID>127)   						   //锟叫断癸拷锟斤拷锟斤拷
   		  	Mdbs =  Mdbs|FuncID_err;
    	 	else      										//锟斤拷锟斤拷锟斤拷锟斤拷确锟斤拷锟斤拷锟斤拷锟斤拷锟叫讹拷锟斤拷锟斤拷锟斤拷锟斤拷帧
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

    if(Mdbs == 0)								 //锟斤拷锟角帮拷锟侥达拷锟斤拷没锟叫ｏ拷锟斤拷硕约锟斤拷锟叫ｏ拷锟斤拷锟�
	{
	    CRC_cacul = CRC16(UartInfo.RxData,UartInfo.RxTimes-2);  //锟斤拷锟斤拷CRC校锟斤拷锟斤拷
		if((UartInfo.RxData[UartInfo.RxTimes-2]<<8|UartInfo.RxData[UartInfo.RxTimes-1])== CRC_cacul)
			Mdbs = Frame_OK;			        //CRC校锟斤拷锟斤拷锟轿伙拷锟角帮拷锟斤拷锟轿伙拷诤锟�,锟斤拷为锟斤拷锟斤拷确锟较碉拷锟斤拷确帧
		else
			Mdbs = Mdbs|CRC_err;
	}
	return 	Mdbs;								  //锟斤拷锟斤拷Modbus锟侥癸拷锟斤拷状态锟斤拷锟斤拷锟斤拷锟�
}



//==============CRC校锟斤拷锟斤拷锟斤拷珊锟斤拷锟�=================
//锟斤拷锟斤拷  校锟斤拷锟斤拷锟斤拷锟街革拷耄ｏ拷锟斤拷锟捷筹拷锟斤拷
//锟斤拷锟斤拷值锟斤拷锟斤拷锟叫ｏ拷锟街碉拷徒锟斤拷盏锟斤拷锟紺RC值锟斤拷同锟斤拷锟斤拷为锟斤拷模锟斤拷锟斤拷锟轿拷锟�
//
unsigned short CRC16(unsigned char *puchMsg,unsigned int usDataLen)                   //                  锟斤拷
{
	unsigned char uchCRCHi = 0xFF ; /* 锟斤拷CRC锟街节筹拷始锟斤拷  */
	unsigned char uchCRCLo = 0xFF ; /* 锟斤拷CRC 锟街节筹拷始锟斤拷 */
	unsigned uIndex ;				/* CRC循锟斤拷锟叫碉拷锟斤拷锟斤拷  */
	while (usDataLen--) 			/* 锟斤拷锟斤拷锟斤拷息锟斤拷锟斤拷锟斤拷   */
	{
		uIndex = uchCRCHi ^ *puchMsg++ ; /* 锟斤拷锟斤拷CRC */
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex] ;
	}
	return (uchCRCHi << 8 | uchCRCLo) ;

}

//===========锟矫碉拷锟斤拷确锟斤拷帧锟襟，斤拷锟斤拷帧锟斤拷息锟斤拷锟斤拷==============
void Deal_OKfrm(void)
{
    switch(UartInfo.RxFuncID)
 	{
		case 0x01:  Reply_01();	 	 break;    //锟斤拷锟斤拷圈锟斤拷锟斤拷(锟斤拷写锟斤拷锟斤拷)
		case 0x02:  Reply_02(); 	 break;    //锟斤拷锟斤拷散锟斤拷锟斤拷锟斤拷(只锟斤拷锟斤拷锟斤拷)
		case 0x03:  Reply_03(); 	 break;    //锟斤拷锟斤拷锟街寄达拷锟斤拷(锟斤拷写锟斤拷锟斤拷)
		case 0x04:  Reply_04();  	 break;    //锟斤拷锟斤拷锟斤拷拇锟斤拷锟�(只锟斤拷锟斤拷锟斤拷)
		case 0x05:  Reply_05(); 	 break;    //写锟斤拷锟斤拷锟斤拷圈
		case 0x06:  Reply_06();  	 break;    //写锟斤拷锟斤拷锟斤拷锟街寄达拷锟斤拷
		//case 0x0F:  Reply_0F();  	 break;    //写锟斤拷锟斤拷锟饺�
		//case 0x10:  Reply_10();  	 break;    //写锟斤拷锟斤拷锟斤拷旨拇锟斤拷锟�
		default:	  				 break;
	}
}





/////////////////////modbus END//////////////////////////////////////


/**
  * @}
  */ 

/******************* (C) COPYRIGHT YuRobo *****END OF FILE****/
