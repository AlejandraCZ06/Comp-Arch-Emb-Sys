#include <stdatomic.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "inc/gpio_config.h"
#include "inc/spi.h"
#include "inc/system_config.h"
#include "inc/gpio_config.h"
#include "inc/usart.h"
#include "max7219.h"

#define LED_PIN 5 // Pin 5 corresponds to the on-board LED on the NUCLEO-64 board
#define BUTTON_PIN 13 // Pin 13 corresponds to the on-board button on the NUCLEO-64 board
/*Declaration of two timers made by software updated every 1ms*/


/* Must be delcared volatile as the timer can update asynchounosly*/
volatile SW_Timers timers;
volatile uint8_t uart_rx_flag=0;
volatile uint8_t spi_rx_flag=0;
uint8_t spi_master[1]="";
uint8_t spi_slave[1] ="";
uint8_t spi_slave_rcv[1]="";
uint8_t i=1;


/*The SysTick_Handler, was already defined as weak during the crt0.s init file, so when we define it here, 
the Vector table is updated whit the new address where the function is allocated, so that when and interrupt happen
the vector table knows where to find the SysTick_Handler*/
void SysTick_Handler(void){
  update_sw_timers(&timers);
}


void USART2_IRQHandler(void){
  if(USART2->SR&USART_SR_RXNE){
    uart_rx_flag = 1;
  }
}


void GPIOConfig(void){

  WRITE_REG_FIELD(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN, 1);
  // do two dummy reads after enabling the peripheral clock, as per the errata
  volatile unsigned int dummy;
  dummy = (RCC->AHB1ENR);
  dummy = (RCC->AHB1ENR);
  
  /*UART2 TX/RX Pins*/
  WRITE_REG_FIELD(GPIOA->MODER,GPIO_MODER_MODER2,2);
  WRITE_REG_FIELD(GPIOA->AFR[0],GPIO_AFRL_AFRL2,7);
  WRITE_REG_FIELD(GPIOA->MODER,GPIO_MODER_MODER3,2);
  WRITE_REG_FIELD(GPIOA->AFR[0],GPIO_AFRL_AFRL3,7);


}

const uint64_t IMAGES[16] = {
  0xff000001010000ff, 0xff000003030000ff, 0xff000006060000ff,
  0xff00000c0c0000ff, 0xff000018180000ff, 0xff000030300000ff,
  0xff000060600000ff, 0xff0000c0c00000ff, 0xff000080800000ff,
  0xff0000c0c00000ff, 0xff000060600000ff, 0xff000018180000ff,
  0xff00000c0c0000ff, 0xff000006060000ff, 0xff000003030000ff,
  0xff000001010000ff
};

uint8_t data[8] ={
  0xAA,
  0x55,
  0xAA,
  0x55,
  0xAA,
  0x55,
  0xAA,
  0x55
};

int main()
{

  clock_config();
  SysTick_Init(1000);
  SysTick_enable_IrQ(1);
   
  timers.sw_tmr1_period = 100;
  timers.sw_tmr2_period = 1000;
  /*Enable the CLK to the GPIOA and GPIOC, this needs to be done before the configuration opf the GPIO*/
  

  GPIOConfig();

  WRITE_REG_FIELD(RCC->APB1ENR,RCC_APB1ENR_USART2EN,1);
  volatile unsigned int dummy;
  dummy =  RCC->APB1ENR;
  dummy =  RCC->APB1ENR;

  UART_Init(USART2);
  UART_RX_IRQ_EN(USART2,1);
  NVIC_EnableIRQ(USART2_IRQn);
  
  max7219_config();

  max7219_print(data);

  uint8_t idx=0;

  while(1)
  {
    if(timers.sw_tmr1_flag){
      timers.sw_tmr1_flag = 0;
      max7219_print((uint8_t *)&IMAGES[idx]);
      idx++;
      if(idx>15){
        idx = 0;
      }
    }
  }


}
