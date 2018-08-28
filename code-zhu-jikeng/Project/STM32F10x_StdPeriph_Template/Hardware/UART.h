#ifndef __UART_H
#define __UART_H

#include "stm32f10x.h"
#include "main.h"

#define MASTER_UART              USART3
#define SLAVE_UART               USART2
#define WIFI_UART                UART4
#define TOP_UART                 USART1
//#define USART1   与上位机进行通信的

#define USART1_GPIO              GPIOA
#define USART1_CLK               RCC_APB2Periph_USART1
#define USART1_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USART1_RxPin             GPIO_Pin_10
#define USART1_TxPin             GPIO_Pin_9
#define RXBUF1_LENGTH			 120
#define TXBUF1_LENGTH			 120
#define UP485_pin                GPIO_Pin_11

#define RX_DE_UP()  GPIO_SetBits(USART1_GPIO,UP485_pin)
#define TX_DE_UP()  GPIO_ResetBits(USART1_GPIO,UP485_pin)

//#define USART2  主机和从机进行通信

#define USART2_GPIO              GPIOA
#define USART2_CLK               RCC_APB1Periph_USART2
#define USART2_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USART2_RxPin             GPIO_Pin_3
#define USART2_TxPin             GPIO_Pin_2
#define RXBUF2_LENGTH			 120
#define TXBUF2_LENGTH			 120
#define DE485_pin                GPIO_Pin_4

#define RX_DE1()  GPIO_SetBits(USART2_GPIO,DE485_pin)
#define TX_DE1()  GPIO_ResetBits(USART2_GPIO,DE485_pin)

//#define  USART3                DSP主机和STM32主机通信交互
#define USART3_GPIO              GPIOB
#define USART3_CLK               RCC_APB1Periph_USART3
#define USART3_GPIO_CLK          RCC_APB2Periph_GPIOB
#define USART3_RxPin             GPIO_Pin_11
#define USART3_TxPin             GPIO_Pin_10
#define RXBUF3_LENGTH			 120
#define TXBUF3_LENGTH			 120

//#define  USART4               stm32主机和wifi通信交互
#define UART4_GPIO              GPIOC
#define UART4_CLK               RCC_APB1Periph_UART4
#define UART4_GPIO_CLK          RCC_APB2Periph_GPIOC
#define UART4_RxPin              GPIO_Pin_11
#define UART4_TxPin              GPIO_Pin_10
#define RXBUF4_LENGTH			 120
#define TXBUF4_LENGTH			 120

//#define  USART5
#define UART5_GPIO_T            GPIOB
#define UART5_GPIO_R            GPIOD
#define UART5_CLK               RCC_APB1Periph_UART5
#define UART5_GPIO_CLK_T        RCC_APB2Periph_GPIOB
#define UART5_GPIO_CLK_R        RCC_APB2Periph_GPIOD
#define UART5_RxPin              GPIO_Pin_2
#define UART5_TxPin              GPIO_Pin_12
#define RXBUF5_LENGTH			 120
#define TXBUF5_LENGTH			 120

#define FRAME_START              0xAA
#define FRAME_END                0xBB

extern char rxBuf1[RXBUF1_LENGTH];
extern char rxBuf2[RXBUF2_LENGTH];
extern char rxBuf3[RXBUF3_LENGTH];

extern char txBuf1[TXBUF1_LENGTH];
extern char txBuf2[TXBUF2_LENGTH];
extern char txBuf3[TXBUF3_LENGTH];

extern uint8_t rxLength1;
extern uint8_t rxLength2;
extern uint8_t rxLength3;

extern e_state rx1Flag;
extern e_state rx2Flag;
extern e_state rx3Flag;


void UART1Init(void);
void UART2Init(void);
void UART3Init(void);
void UART4Init(void);
void UART5Init(void);

void SendStr(USART_TypeDef* USARTx,uint8_t *str,uint8_t length);

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);



#endif
