#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "inc/gpio_config.h"
#include "inc/system_config.h"
#include "inc/gpio_config.h"

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


void update_sw_timers(SW_Timers* timer){
  timer->sw_tmr1_count++;
  timer->sw_tmr2_count++;
  if(timer->sw_tmr1_count==timer->sw_tmr1_period){
    timer->sw_tmr1_count=0;
    timer->sw_tmr1_flag=1;
  }
  if(timer->sw_tmr2_count==timer->sw_tmr2_period){
    timer->sw_tmr2_count=0;
    timer->sw_tmr2_flag=1;
  }
}

void TIM2_IRQHandler(void){
  
  TIM2->SR &= ~TIM_SR_UIF;       // clear the TIM2 flag 
  if (read_pin_state(GPIOC,1<<BUTTON_PIN)){
    toggle_pin_state(GPIOA,1<<LED_PIN);
  }

}

void TIM5_IRQHandler(void){
  TIM5->SR &= ~TIM_SR_UIF;      // clear the TIM5 flag 
  if (!read_pin_state(GPIOC,1<<BUTTON_PIN)){
    toggle_pin_state(GPIOA,1<<LED_PIN);
  }
}

void Timers_Init(void){
  /*Enable the clock to the timers 2 and 5*/

  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
  volatile unsigned int dummy;
  dummy =  RCC->APB1ENR;
  dummy =  RCC->APB1ENR;

  /* Timer2 config as base timer*/
  TIM2->PSC = 84-1;             // Prescale to 1MHz
  TIM2->ARR = 500000-1;         // Timeout at 500ms
  TIM2->CR1 |= TIM_CR1_DIR;     // Counter Up
  TIM2->CR1 |= TIM_CR1_ARPE;    // Autoreload
  TIM2->CNT = 0;                // restart the counter
  TIM2->CR1 = TIM_CR1_CEN;      // Enable the timer
  TIM2->DIER |= TIM_DIER_UIE;   // Enable the timer to fire and Interrupt

  /* Timer5 config as base timer*/
  TIM5->PSC = 84-1;
  TIM5->ARR = 100000-1;
  TIM5->CR1 |= TIM_CR1_DIR;
  TIM5->CR1 |= TIM_CR1_ARPE;
  TIM5->CNT = 0;
  TIM5->CR1 = TIM_CR1_CEN;
  TIM5->DIER |= TIM_DIER_UIE;

  NVIC_EnableIRQ(TIM2_IRQn);  // Enable the TIM2 IRQ on the NVIC
  NVIC_EnableIRQ(TIM5_IRQn);  // Enable the TIM5 IRQ on the NVIC
}

int main()
{

  clock_config();
   
  /*Enable the CLK to the GPIOA and GPIOC, this needs to be done before the configuration opf the GPIO*/
  
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
  GPIO_Config(GPIOC,GPIO_Init);

  timers.sw_tmr1_period = 500;
  timers.sw_tmr2_period = 100;
  Timers_Init();
  
  while(1)
  {
    /*This example now works using interrupts only, so the main is fully available to run enything else*/
  }


}
