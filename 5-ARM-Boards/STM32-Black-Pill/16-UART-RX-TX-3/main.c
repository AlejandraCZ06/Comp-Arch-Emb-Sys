#include <stdatomic.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "inc/gpio_config.h"
#include "inc/system_config.h"
#include "inc/gpio_config.h"
#include "inc/usart.h"

#define LED_PIN 5 // Pin 5 corresponds to the on-board LED on the NUCLEO-64 board
#define BUTTON_PIN 13 // Pin 13 corresponds to the on-board button on the NUCLEO-64 board

/* Must be delcared volatile as the timer can update asynchounosly*/
volatile SW_Timers timers;

/*The SysTick_Handler, was already defined as weak during the crt0.s init file, so when we define it here, 
the Vector table is updated whit the new address where the function is allocated, so that when and interrupt happen
the vector table knows where to find the SysTick_Handler*/
void SysTick_Handler(void){
  update_sw_timers(&timers);
}

void USART2_IRQHandler(void){
  /*The pending IRQ flag for the uart is automatically cleared after reading the recieved data*/
  uart_putc(uart_getc());
}

int main()
{

  clock_config();
  SysTick_Init(1000);
  SysTick_enable_IrQ(1);
   
  timers.sw_tmr1_period = 1000;
  timers.sw_tmr2_period = 1000;
  /*Enable the CLK to the GPIOA and GPIOC, this needs to be done before the configuration opf the GPIO*/
  
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable GPIOA clock in RCC_AHB1ENR register (bit 0)

  // do two dummy reads after enabling the peripheral clock, as per the errata
  volatile unsigned int dummy;
  dummy = (RCC->AHB1ENR);
  dummy = (RCC->AHB1ENR);
  
  /*Uart Pins Configurations to use the ST-LINK Vitual Serial Port*/
  GPIO_InitTypeDef GPIO_Init; 
  GPIO_Init.Pin = (1<<2);
  GPIO_Init.Mode = (2 << GPIO_MODER_MODER2_Pos);
  GPIO_Init.Alternate = (7<<GPIO_AFRL_AFSEL2_Pos);
  GPIO_Config(GPIOA,GPIO_Init);

  GPIO_Init.Pin = (1<<3);
  GPIO_Init.Mode = (2 << GPIO_MODER_MODER3_Pos);
  GPIO_Init.Alternate = (7<<GPIO_AFRL_AFSEL3_Pos);
  GPIO_Config(GPIOA,GPIO_Init);

  UART2_Init();
  NVIC_EnableIRQ(USART2_IRQn);


  while(1)
  {
    if(timers.sw_tmr1_flag){
      timers.sw_tmr1_flag=0;
      uart_puts("Waiting ....\r\n");
    }

  }


}
