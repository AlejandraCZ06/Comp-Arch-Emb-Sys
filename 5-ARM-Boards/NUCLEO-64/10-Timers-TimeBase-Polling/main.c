#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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


void Timers_Init(void){
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
  volatile unsigned int dummy;
  dummy =  RCC->APB1ENR;
  dummy =  RCC->APB1ENR;

  TIM2->PSC = 84-1;
  TIM2->ARR = 500000-1;
  TIM2->CR1 |= TIM_CR1_DIR;
  TIM2->CR1 |= TIM_CR1_ARPE;
  TIM2->CNT = 0;
  TIM2->CR1 = TIM_CR1_CEN;

  TIM5->PSC = 84-1;
  TIM5->ARR = 100000-1;
  TIM5->CR1 |= TIM_CR1_DIR;
  TIM5->CR1 |= TIM_CR1_ARPE;
  TIM5->CNT = 0;
  TIM5->CR1 = TIM_CR1_CEN;

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

    /*
    if(__HAL_TIM_GET_FLAG(&htim2,TIM_FLAG_UPDATE)) {
        __HAL_TIM_CLEAR_FLAG(&htim2,TIM_FLAG_UPDATE);
        update_sw_timers(&timers);
      }*/

    if(read_pin_state(GPIOC,(1<<BUTTON_PIN))){
      //if(timers.sw_tmr1_flag){
      //  timers.sw_tmr1_flag = 0;
      if(TIM2->SR&TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
        toggle_pin_state(GPIOA,1<<LED_PIN);
      }
      
    } else {
      //if(timers.sw_tmr2_flag){
      //  timers.sw_tmr2_flag = 0;
      if(TIM5->SR&TIM_SR_UIF) {
        TIM5->SR &= ~TIM_SR_UIF;
        toggle_pin_state(GPIOA,1<<LED_PIN);
      }
    }

  }


}
