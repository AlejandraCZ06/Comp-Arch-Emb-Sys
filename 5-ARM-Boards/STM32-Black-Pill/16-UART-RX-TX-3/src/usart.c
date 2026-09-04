
#include "usart.h"

#define UART2_PCLK_HZ (SYCLK / 2U)

void UART2_Init(void){
  WRITE_REG_FIELD(RCC->APB1ENR,RCC_APB1ENR_USART2EN,1);
  volatile unsigned int dummy;
  dummy =  RCC->APB1ENR;
  dummy =  RCC->APB1ENR;

  WRITE_REG_FIELD(USART2->CR1,USART_CR1_UE,1);
  WRITE_REG_FIELD(USART2->CR1,USART_CR1_M,0);
  WRITE_REG_FIELD(USART2->CR2,USART_CR2_STOP,0);
  USART2->BRR = (UART2_PCLK_HZ) / 115200U;
  WRITE_REG_FIELD(USART2->CR1,USART_CR1_TE,1);  
  WRITE_REG_FIELD(USART2->CR1,USART_CR1_RE,1);     
  WRITE_REG_FIELD(USART2->CR1,USART_CR1_RXNEIE,1); 
}



void uart_putc(char dat){
    while (!(READ_REG_FIELD(USART2->SR,USART_SR_TXE)));
    USART2->DR = (dat&0xff);
}

char uart_tstc(void){
    return(READ_REG_FIELD(USART2->SR,USART_SR_RXNE));
}

char uart_getc(void){
    return (char)(USART2->DR & 0xffU);
}

void uart_puts(const char* str){
    while (*str)
    {
        uart_putc(*str++);
    }
    
}