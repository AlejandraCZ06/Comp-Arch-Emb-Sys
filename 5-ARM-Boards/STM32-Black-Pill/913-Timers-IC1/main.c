#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "inc/system_config.h"
#include "inc/stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal.h"

TIM_Base_InitTypeDef htim2;

int main()
{

  HAL_Init(); //Initialices the HAL
  HAL_SuspendTick(); //Disable SysTick, to avoid firing SysTick IRQ
  clock_config();

  /*This configures the  MCO1 (PA8) using 3.2MHz=16MHz/5*/
  RCC->CFGR |= (RCC_CFGR_MCO1PRE_Msk);  // div5
  RCC->CFGR &= ~(RCC_CFGR_MCO1_Msk); // HSI

  /*This configures the  MCO1 (PA8) using 3.2MHz=16MHz/5 using the LL HAL*/
  //__HAL_RCC_MCO1_CONFIG(RCC_MCO1SOURCE_HSI,RCC_MCODIV_5);

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  /*Create the GPIO config structure*/
  GPIO_InitTypeDef GPIO_InitStruct;
  /*Config the GPIOA*/
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  


  
  while(1)
  {




  }


}
