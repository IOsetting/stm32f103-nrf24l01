#include <stdio.h>
#include "usart.h"

struct __FILE { int handle; };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}
  USART_SendData(USARTx, ch);
  return(ch);
}

int fgetc(FILE *f) {
  char ch;
  while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET){}
  ch = USART_ReceiveData(USARTx);
  return((int)ch);
}

int ferror(FILE *f) {
  return EOF;
}

void _ttywrch(int ch) {
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
  USART_SendData(USARTx, ch);
}

void _sys_exit(int return_code) {
  while (1); /* endless loop */
}

void RCC_Configuration(void)
{
  if (USARTx == USART1) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  } else if (USARTx == USART2) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  } else if (USARTx == USART3) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  }
}

/**
* USART1(PA9, PA10)
* USART2(PA2, PA3)
* USART3(PB10, PB11)
*/
void UsartGPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if (USARTx == USART1) {
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
  } else if (USARTx == USART2) {
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
  } else if (USARTx == USART3) {
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
}

void USART_Configuration(void)
{
  RCC_Configuration();

  USART_InitTypeDef USART_InitStruct;
  USART_InitStruct.USART_BaudRate = 115200;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

  USART_Init(USARTx, &USART_InitStruct);
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
  USART_Cmd(USARTx, ENABLE);

  UsartGPIO_Configuration();
}


/**************************
* USART with CTS/RTS
*
* USART1 - TX:PA9,  RX:PA10, CTS:PA11, RTS:PA12, CK:PA8 
* USART2 - TX:PA2,  RX:PA3,  CTS:PA0,  RTS:PA1,  CK:PA4
* USART3 - TX:PB10, RX:PB11, CTS:PB13, RTS:PB14, CK:PB12
*
***************************/
void UsartGPIO_CTRT_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if (USARTx == USART1) {
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_12;/*GPIO_Pin_1 | GPIO_Pin_2;*/
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;/*GPIO_Pin_0 | GPIO_Pin_3;*/
    GPIO_Init(GPIOA, &GPIO_InitStruct);

  } else if (USARTx == USART2) {
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

  } else if (USARTx == USART3) {
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_14;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

  }
  
}

void USART_CTRT_Configuartion(void)
{
  RCC_Configuration();

  USART_InitTypeDef USART_InitStruct;
  USART_InitStruct.USART_BaudRate = 115200;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

  USART_Init(USARTx, &USART_InitStruct);
  USART_Cmd(USARTx, ENABLE);
  UsartGPIO_CTRT_Configuration();
}

