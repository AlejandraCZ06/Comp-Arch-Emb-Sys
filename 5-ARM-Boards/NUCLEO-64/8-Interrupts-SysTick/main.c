#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "inc/gpio_config.h"
#include "system_config.h"


#define LED_PIN 5 // Pin 5 corresponds to the on-board LED on the NUCLEO-64 board
#define BUTTON_PIN 13 // Pin 13 corresponds to the on-board button on the NUCLEO-64 board

/*Declaration of two timers made by software updated every 1ms*/
typedef struct{
  unsigned int sw_tmr1_count;
  unsigned int sw_tmr2_count;
  unsigned int sw_tmr1_period;
  unsigned int sw_tmr2_period;
  unsigned int sw_tmr1_flag;
  unsigned int sw_tmr2_flag;
} SW_Timers;

/* Must be delcared volatile as the timer can update asynchounosly*/
volatile SW_Timers timers;

/*The SysTick_Handler, was already defined as weak during the crt0.s init file, so when we define it here, 
the Vector table is updated whit the new address where the function is allocated, so that when and interrupt happen
the vector table knows where to find the SysTick_Handler*/
void SysTick_Handler(void){
  timers.sw_tmr1_count++;
  timers.sw_tmr2_count++;
  if(timers.sw_tmr1_count==timers.sw_tmr1_period){
    timers.sw_tmr1_count=0;
    timers.sw_tmr1_flag=1;
  }
  if(timers.sw_tmr2_count==timers.sw_tmr2_period){
    timers.sw_tmr2_count=0;
    timers.sw_tmr2_flag=1;
  }
}

int main()
{
  /*Initialize the timeout period of the SW timers*/
  timers.sw_tmr1_period=500;
  timers.sw_tmr2_period=250;

  clock_config();

  /*This is a custom SysTick IRQ config*/
  SysTick_Init(1000);
  SysTick_enable_IrQ(1);

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable GPIOA clock in RCC_AHB1ENR register (bit 0)
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Enable GPIOC clock in RCC_AHB1ENR register (bit 2)
  // do two dummy reads after enabling the peripheral clock, as per the errata
  volatile unsigned int dummy;
  dummy = (RCC->AHB1ENR);
  dummy = (RCC->AHB1ENR);
  
  GPIO_InitTypeDef GPIO_Init; 
  GPIO_Init.Pin = (1<<LED_PIN);
  GPIO_Init.Mode = (1 << GPIO_MODER_MODER5_Pos);
  GPIO_Init.Speed = (3 << GPIO_OSPEEDR_OSPEED5_Pos);
  GPIO_Config(GPIOA,GPIO_Init);

  GPIO_Init.Pin = (1<<BUTTON_PIN);
  GPIO_Init.Mode = (0 << GPIO_MODER_MODER13_Pos);
  GPIO_Init.Pull = (1 << GPIO_PUPDR_PUPD13_Pos);
  GPIO_Config(GPIOC,GPIO_Init);


  while(1)
  {
    /*Checks the status of the Button*/
    if (read_pin_state(GPIOC,(1<<BUTTON_PIN))){
      if(timers.sw_tmr1_flag){
        /*Checks whether the timer reached 500ms, and toggle the LED*/
        timers.sw_tmr1_flag=0;
        toggle_pin_state(GPIOA,(1<<LED_PIN));
      }
    } else {
      if(timers.sw_tmr2_flag){
         /*Checks whether the timer reached 250ms, and toggle the LED*/
        timers.sw_tmr2_flag=0;
        toggle_pin_state(GPIOA,(1<<LED_PIN));
      }
    }
  }


}
