#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "inc/system_config.h"
#include "inc/stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal.h"

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

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim5;


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

int main()
{

  HAL_Init(); //Initialices the HAL
  HAL_SuspendTick(); //Disable SysTick, to avoid firing SysTick IRQ
  clock_config();

  __HAL_RCC_TIM2_CLK_ENABLE();
  __HAL_RCC_TIM5_CLK_ENABLE();
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 83;
  htim2.Init.Period = 500000; //1ms
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 83;
  htim5.Init.Period = 100000;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  HAL_TIM_Base_Init(&htim2);
  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_Base_Init(&htim5);
  HAL_TIM_Base_Start(&htim5);
  
  /*Enable the CLK to the GPIOA and GPIOC, this needs to be done before the configuration opf the GPIO*/
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  /*Create the GPIO config structure*/
  GPIO_InitTypeDef GPIO_InitStruct;
  /*Config the GPIOA*/
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  timers.sw_tmr1_period = 500;
  timers.sw_tmr2_period = 100;
  
  while(1)
  {

    /*
    if(__HAL_TIM_GET_FLAG(&htim2,TIM_FLAG_UPDATE)) {
        __HAL_TIM_CLEAR_FLAG(&htim2,TIM_FLAG_UPDATE);
        update_sw_timers(&timers);
      }*/

    if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)){
      //if(timers.sw_tmr1_flag){
      //  timers.sw_tmr1_flag = 0;
      if(__HAL_TIM_GET_FLAG(&htim2,TIM_FLAG_UPDATE)) {
        __HAL_TIM_CLEAR_FLAG(&htim2,TIM_FLAG_UPDATE);
        HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
      }
      
    } else {
      //if(timers.sw_tmr2_flag){
      //  timers.sw_tmr2_flag = 0;
      if(__HAL_TIM_GET_FLAG(&htim5,TIM_FLAG_UPDATE)) {
        __HAL_TIM_CLEAR_FLAG(&htim5,TIM_FLAG_UPDATE);
        HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
      }
    }

  }


}
