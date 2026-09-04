#include "gpio_config.h"




void GPIO_Config(volatile GPIO_TypeDef *GPIOx, GPIO_InitTypeDef Config)
{

  GPIOx->MODER |= (Config.Mode);
  GPIOx->OSPEEDR |= (Config.Speed);
  GPIOx->PUPDR |= (Config.Pull);
  if (Config.Pin < 0x00000100 ){
      GPIOx->AFR[0] |= (Config.Alternate);
  } else {
      GPIOx->AFR[1] |= (Config.Alternate);
  }
}

uint32_t read_pin_state(volatile GPIO_TypeDef *GPIOx, uint32_t pin){
  return (GPIOx->IDR & (pin)) ? 1 : 0;
}

void write_pin_state(volatile GPIO_TypeDef *GPIOx, uint32_t pin, uint32_t state){
  if(state){
    GPIOx->ODR |= (pin);
  } else {
    GPIOx->ODR &= ~(pin);
  }
}

void toggle_pin_state(volatile GPIO_TypeDef *GPIOx, uint32_t pin){
    GPIOx->ODR ^= pin;
}