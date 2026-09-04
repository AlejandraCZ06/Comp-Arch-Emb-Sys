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



void update_sw_timers(volatile SW_Timers* timer){
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

/*The SysTick_Handler, was already defined as weak during the crt0.s init file, so when we define it here, 
the Vector table is updated whit the new address where the function is allocated, so that when and interrupt happen
the vector table knows where to find the SysTick_Handler*/
void SysTick_Handler(void){
  update_sw_timers(&timers);
}

volatile uint32_t period=0, duty=0;
float freq=0.0, dutyper=0.0;

void TIM5_IRQHandler(void){
  if(TIM5->SR&TIM_SR_CC1IF){
    period = TIM5->CCR1;
    if(period!=0) {
      duty = TIM5->CCR2;
    }
    TIM5->SR&=~TIM_SR_CC1IF;   
  }

}

void TIM2_PWM_10KHz_Init(void){
  /*Enable the clock to the timers 2 and 5*/
  
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  volatile unsigned int dummy;
  dummy =  RCC->APB1ENR;
  dummy =  RCC->APB1ENR;

  /* Timer2 config as base timer*/
  TIM2->PSC = 0;             // Prescale to 1MHz
  TIM2->ARR = 4200-1;         // Timeout at 500ms
  TIM2->CCR1 = 2100-1;
  TIM2->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE; 
  TIM2->CCER |= TIM_CCER_CC1E;
  TIM2->CR1 |= TIM_CR1_DIR;     // Counter Up
  TIM2->CR1 |= TIM_CR1_ARPE;    // Autoreload
  TIM2->CNT = 0;                // restart the counter
  TIM2->CR1 = TIM_CR1_CEN;      // Enable the timer

}

void TIM5_IC_Init(void){
  
  RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
  volatile unsigned int dummy;
  dummy =  RCC->APB1ENR;
  dummy =  RCC->APB1ENR;

  /* Timer2 config as base timer*/
  TIM5->PSC = 0;             // Prescale to 42MHz
  TIM5->ARR = 0xffffffff;         // maximum reload value
 
  TIM5->CCMR1 |= (1 << TIM_CCMR1_CC1S_Pos);  // enable Input capture CH1 on TI1
  TIM5->CCER  &= ~((1<<TIM_CCER_CC1P_Pos) | (1<<TIM_CCER_CC1NP_Pos)); // Capture configured on rising edge

  TIM5->CCMR1 |= (2 << TIM_CCMR1_CC2S_Pos); // Enable Input capture CH2 on same TI1
  TIM5->CCER  |= ((1<<TIM_CCER_CC2P_Pos) | ((0<<TIM_CCER_CC2NP_Pos))); // Enable capture on falling edge 
  
  TIM5->SMCR |= (5<<TIM_SMCR_TS_Pos); // TI1FP1 selected
  TIM5->SMCR |= (4<<TIM_SMCR_SMS_Pos); //Reset the Timer on every rising capture event

  TIM5->CR1 &= ~TIM_CR1_DIR;     // Counter Up
  TIM5->CR1 |= TIM_CR1_ARPE;    // Autoreload, this does not 
  //TIM5->CNT = 0;                // restart the counter
  TIM5->CCER |= (1<<TIM_CCER_CC1E_Pos) | (1<<TIM_CCER_CC2E_Pos); // Enable cpature CC1 and CC2
  TIM5->DIER |= (1<<TIM_DIER_CC1IE_Pos); // Enable CC1 interrupt (only on rising edge)
  TIM5->CR1 |= TIM_CR1_CEN;      // Enable the timer

  NVIC_EnableIRQ(TIM5_IRQn); // Enable the TIM5 IRQ

}

int main()
{

  clock_config();
  SysTick_Init(1000);
  SysTick_enable_IrQ(1);
   
  timers.sw_tmr1_period = 10;
  /*Enable the CLK to the GPIOA and GPIOC, this needs to be done before the configuration opf the GPIO*/
  
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable GPIOA clock in RCC_AHB1ENR register (bit 0)
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Enable GPIOC clock in RCC_AHB1ENR register (bit 2)
  // do two dummy reads after enabling the peripheral clock, as per the errata
  volatile unsigned int dummy;
  dummy = (RCC->AHB1ENR);
  dummy = (RCC->AHB1ENR);
  
  GPIO_InitTypeDef GPIO_Init; 
  GPIO_Init.Pin = (1<<LED_PIN);
  GPIO_Init.Mode = (2 << GPIO_MODER_MODER5_Pos);
  GPIO_Init.Speed = (3 << GPIO_OSPEEDR_OSPEED5_Pos);
  GPIO_Init.Alternate = (1<<GPIO_AFRL_AFSEL5_Pos);
  GPIO_Config(GPIOA,GPIO_Init);

  GPIO_Init.Pin = (1<<0);
  GPIO_Init.Mode = (2 << GPIO_MODER_MODER0_Pos);
  GPIO_Init.Alternate = (2<<GPIO_AFRL_AFSEL0_Pos);
  GPIO_Config(GPIOA,GPIO_Init);

  GPIO_Init.Pin = (1<<BUTTON_PIN);
  GPIO_Init.Mode = (0 << GPIO_MODER_MODER13_Pos);
  GPIO_Config(GPIOC,GPIO_Init);

  TIM2_PWM_10KHz_Init();
  TIM5_IC_Init();

  while(1)
  {
      freq = 42000000.0 / (float)period; 
      dutyper = ((float) duty / (float) period)*100;
  }


}
