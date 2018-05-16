#include "SPI.h"

/**
******************************************************************************
* @file    SPI.c
* @author  wqdnan
* @version V3.5.0
* @date    28-12-2017
* @brief   初始化SPI，使能SPI收发，定义SPI接收函数和发送函数
******************************************************************************
* @attention
*
*
*
******************************************************************************
*/
/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BufferSize  32

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SPI_InitTypeDef  SPI_InitStructure;
uint16_t SPI1_Buffer_Tx[BufferSize] = {0x0102, 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C,
                                  0x0D0E, 0x0F10, 0x1112, 0x1314, 0x1516, 0x1718,
                                  0x191A, 0x1B1C, 0x1D1E, 0x1F20, 0x2122, 0x2324,
                                  0x2526, 0x2728, 0x292A, 0x2B2C, 0x2D2E, 0x2F30,
                                  0x3132, 0x3334, 0x3536, 0x3738, 0x393A, 0x3B3C,
                                  0x3D3E, 0x3F40};
uint8_t SPI_MASTER_Buffer_Tx[BufferSize] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
                                            0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C,
                                            0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12,
                                            0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
                                            0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E,
                                            0x1F, 0x20};
uint16_t SPI2_Buffer_Tx[BufferSize] = {0x5152, 0x5354, 0x5556, 0x5758, 0x595A, 0x5B5C,
                                  0x5D5E, 0x5F60, 0x6162, 0x6364, 0x6566, 0x6768,
                                  0x696A, 0x6B6C, 0x6D6E, 0x6F70, 0x7172, 0x7374,
                                  0x7576, 0x7778, 0x797A, 0x7B7C, 0x7D7E, 0x7F80,
                                  0x8182, 0x8384, 0x8586, 0x8788, 0x898A, 0x8B8C,
                                  0x8D8E, 0x8F90};
uint8_t SPI1_Buffer_Rx[BufferSize], SPI2_Buffer_Rx[BufferSize];
uint32_t TxIdx = 0, RxIdx = 0;
uint16_t CRC1Value = 0, CRC2Value = 0;


/**
  * @brief  SPI初始化，SPI1作为主机，SPI2作为从机，采用双向直接发送发送.
  * @param  None
  * @retval None
  */
void SPI1Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	/* PCLK2 = HCLK/2 */
	RCC_PCLK2Config(RCC_HCLK_Div2);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | SPI_MASTER_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(SPI_SLAVE_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SPI_MASTER_PIN_SCK | SPI_MASTER_PIN_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_MASTER_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SPI_MASTER_PIN_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI_MASTER_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SPI_SLAVE_PIN_SCK | SPI_SLAVE_PIN_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI_SLAVE_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SPI_SLAVE_PIN_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_SLAVE_GPIO, &GPIO_InitStructure);

	/* SPI1 configuration ------------------------------------------------------*/
	 SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	 SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	 SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	 SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	 SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	 SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	 SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	 SPI_InitStructure.SPI_CRCPolynomial = 7;
	 SPI_Init(SPI_MASTER, &SPI_InitStructure);
	 /* SPI2 configuration ------------------------------------------------------*/
	 SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	 SPI_Init(SPI_SLAVE, &SPI_InitStructure);

	 SPI_Cmd(SPI_MASTER, ENABLE);
	 SPI_Cmd(SPI_SLAVE, ENABLE);

	 /* Transfer procedure */
	 while (TxIdx < BufferSize - 1)
	 {
	   /* Wait for SPI1 Tx buffer empty */
	   while (SPI_I2S_GetFlagStatus(SPI_MASTER, SPI_I2S_FLAG_TXE) == RESET);
	   /* Send SPI2 data */
	   SPI_I2S_SendData(SPI_SLAVE, SPI2_Buffer_Tx[TxIdx]);
	   /* Send SPI1 data */
	   SPI_I2S_SendData(SPI_MASTER, SPI1_Buffer_Tx[TxIdx++]);
	   /* Wait for SPI2 data reception */
	   while (SPI_I2S_GetFlagStatus(SPI_SLAVE, SPI_I2S_FLAG_RXNE) == RESET);
	   /* Read SPI2 received data */
	   SPI2_Buffer_Rx[RxIdx] = SPI_I2S_ReceiveData(SPI_SLAVE);
	   /* Wait for SPI1 data reception */
	   while (SPI_I2S_GetFlagStatus(SPI_MASTER, SPI_I2S_FLAG_RXNE) == RESET);
	   /* Read SPI1 received data */
	   SPI1_Buffer_Rx[RxIdx++] = SPI_I2S_ReceiveData(SPI_MASTER);
	 }

}

/**
  * @brief  SPI初始化，SPI1作为主机，SPI2作为从机，功能为SPI1发送，SPI2中断接收.
  * @param  None
  * @retval None
  */
void SPI2Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/* PCLK2 = HCLK/2 */
	RCC_PCLK2Config(RCC_HCLK_Div2);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | SPI_MASTER_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(SPI_SLAVE_CLK, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = SPI_SLAVE_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI_MASTER_PIN_SCK | SPI_MASTER_PIN_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_MASTER_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SPI_MASTER_PIN_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI_MASTER_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SPI_SLAVE_PIN_SCK | SPI_SLAVE_PIN_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI_SLAVE_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SPI_SLAVE_PIN_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_SLAVE_GPIO, &GPIO_InitStructure);

	/* SPI1 configuration ------------------------------------------------------*/
	 SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	 SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	 SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	 SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	 SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	 SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	 SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	 SPI_InitStructure.SPI_CRCPolynomial = 7;
	 SPI_Init(SPI_MASTER, &SPI_InitStructure);
	 /* SPI2 configuration ------------------------------------------------------*/
	 SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	 SPI_Init(SPI_SLAVE, &SPI_InitStructure);

	 SPI_I2S_ITConfig(SPI_SLAVE, SPI_I2S_IT_RXNE, ENABLE);

	 SPI_Cmd(SPI_MASTER, ENABLE);
	 SPI_Cmd(SPI_SLAVE, ENABLE);
}

void SPI2_IRQHandler(void)
{
   /* Store SPI_SLAVE received data */
	if(SPI_I2S_GetITStatus(SPI_SLAVE, SPI_I2S_IT_RXNE) == SET) 
	{
		SPI2_Buffer_Rx[RxIdx] = SPI_I2S_ReceiveData(SPI_SLAVE);
		if(RxIdx<(BufferSize-1))
			RxIdx ++;
  }
}
