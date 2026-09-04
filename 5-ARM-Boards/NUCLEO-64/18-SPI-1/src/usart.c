
#include "usart.h"

#define UART_PCLK_HZ (SYCLK / 2U)

// Define the UARTx in your main file
// #define UARTx USART2

void UART_Init(USART_TypeDef* USART){

  WRITE_REG_FIELD(USART->CR1,USART_CR1_UE,1);
  WRITE_REG_FIELD(USART->CR1,USART_CR1_M,0);
  WRITE_REG_FIELD(USART->CR2,USART_CR2_STOP,0);
  USART2->BRR = (UART_PCLK_HZ) / 115200U;
  WRITE_REG_FIELD(USART->CR1,USART_CR1_TE,1);  
  WRITE_REG_FIELD(USART->CR1,USART_CR1_RE,1);     
}


void UART_RX_IRQ_EN(USART_TypeDef* USART,uint8_t enable){
    WRITE_REG_FIELD(USART->CR1,USART_CR1_RXNEIE,enable); 
}


void uart_putc(USART_TypeDef* USART,char dat){
    while (!(READ_REG_FIELD(USART->SR,USART_SR_TXE)));
    USART2->DR = (dat&0xff);
}

char uart_tstc(USART_TypeDef* USART){
    return(READ_REG_FIELD(USART->SR,USART_SR_RXNE));
}

char uart_getc(USART_TypeDef* USART){
    return (char)(USART->DR & 0xffU);
}

void uart_puts(USART_TypeDef* USART,const char* str){
    while (*str)
    {
        uart_putc(USART,*str++);
    }
    
}