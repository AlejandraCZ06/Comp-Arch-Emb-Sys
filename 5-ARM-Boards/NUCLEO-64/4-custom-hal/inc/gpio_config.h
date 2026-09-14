#ifndef __GPIO_CONFIG_H__
#define __GPIO_CONFIG_H__

#include "stm32f401.h"
#include "stdint.h"


void GPIO_Config(void);

uint8_t read_pin_state(volatile GPIO_TypeDef *GPIOx, uint8_t pin);

void write_pin_state(volatile GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t state);

uint8_t boton_presionado(uint8_t posicion);

void led_rojo(uint8_t posicion, uint8_t estado);


#endif