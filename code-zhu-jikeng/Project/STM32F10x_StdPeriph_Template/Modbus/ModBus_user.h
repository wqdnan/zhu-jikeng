/**
******************************************************************************
* @file    ModBus_user.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MODBUS_USER_H_
#define MODBUS_USER_H_
/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Reply_01(void);     	//��������Ȧ����(0x�豸����)��  �ɶ���д
void Reply_02(void);     	//��������ɢ������(1x�豸����)��ֻ�ܶ�
void Reply_03(void);     	//���������ּĴ���(3x�豸����)
void Reply_04(void);     	//����������Ĵ���(4x�豸����)
void Reply_05(void);     	//����д������Ȧ����
void Reply_06(void);     	//����д�������ּĴ�������



#endif /* MODBUS_USER_H_ */
/******************* (C) COPYRIGHT YuRobo *****END OF FILE****/