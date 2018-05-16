#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"


#define SPI_MASTER                   SPI1
#define SPI_MASTER_CLK               RCC_APB2Periph_SPI1
#define SPI_MASTER_GPIO              GPIOA
#define SPI_MASTER_GPIO_CLK          RCC_APB2Periph_GPIOA
#define SPI_MASTER_PIN_NSS           GPIO_Pin_4
#define SPI_MASTER_PIN_SCK           GPIO_Pin_5
#define SPI_MASTER_PIN_MISO          GPIO_Pin_6
#define SPI_MASTER_PIN_MOSI          GPIO_Pin_7
#define SPI_MASTER_IRQn              SPI1_IRQn

#define SPI_SLAVE                    SPI2
#define SPI_SLAVE_CLK                RCC_APB1Periph_SPI2
#define SPI_SLAVE_GPIO               GPIOB
#define SPI_SLAVE_GPIO_CLK           RCC_APB2Periph_GPIOB
#define SPI_SLAVE_PIN_NSS            GPIO_Pin_12
#define SPI_SLAVE_PIN_SCK            GPIO_Pin_13
#define SPI_SLAVE_PIN_MISO           GPIO_Pin_14
#define SPI_SLAVE_PIN_MOSI           GPIO_Pin_15
#define SPI_SLAVE_DMA                DMA1
#define SPI_SLAVE_DMA_CLK            RCC_AHBPeriph_DMA1
#define SPI_SLAVE_Rx_DMA_Channel     DMA1_Channel4
#define SPI_SLAVE_Rx_DMA_FLAG        DMA1_FLAG_TC4
#define SPI_SLAVE_Tx_DMA_Channel     DMA1_Channel5
#define SPI_SLAVE_Tx_DMA_FLAG        DMA1_FLAG_TC5
#define SPI_SLAVE_DR_Base            0x4000380C
#define SPI_SLAVE_IRQn               SPI2_IRQn

void SPI1Init(void);
void SPI2Init(void);

#endif
