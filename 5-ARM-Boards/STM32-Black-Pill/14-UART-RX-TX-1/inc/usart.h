#ifndef _USART_H_
#define _USART_H_

#include "system_config.h"

void UART2_Init(void);



void uart_putc(char dat);
char uart_getc(void);

void uart_puts(const char* str);



#endif 