#ifndef __AT32_USART_H__
#define __AT32_USART_H__

#include "at32f415.h"


void BSP_UsartInit(void);

void USART1_TimeOutCounter(void);
void USART2_TimeOutCounter(void);
void USART3_TimeOutCounter(void);
void UART4_TimeOutCounter(void);
void UART5_TimeOutCounter(void);
uint16_t usart_sendData(uint8_t chUsartNum, uint8_t *pchSendData, uint16_t hwLength);
uint16_t usart_receiveData(uint8_t chUsartNum, uint8_t *pchReceiveData);

uint16_t bsp_UsartSendData(uint8_t *pchSendData,uint16_t hwLength);
uint16_t bsp_UsartReceiveData(uint8_t *pchReceiveData);
#endif
