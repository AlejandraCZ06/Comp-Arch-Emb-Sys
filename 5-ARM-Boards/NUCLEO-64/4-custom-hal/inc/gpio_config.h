#ifndef __GPIO_CONFIG_H__
#define __GPIO_CONFIG_H__

#include "stm32f401.h"
#include "stdint.h"

/* Configuración */
void GPIO_Config(void);

/* Lectura y escritura GPIO */
uint8_t read_pin_state(volatile GPIO_TypeDef *GPIOx, uint8_t pin);
void write_pin_state(volatile GPIO_TypeDef *GPIOx,
                     uint8_t pin,
                     uint8_t state);

/* Botones */
uint8_t boton_presionado(uint8_t boton);

/* LEDs rojos */
void led_rojo(uint8_t led, uint8_t estado);

/* LEDs azules */
void led_azul(uint8_t led, uint8_t estado);

#endif