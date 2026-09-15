#ifndef __GPIO_CONFIG_H__
#define __GPIO_CONFIG_H__

#include "stdint.h"

typedef struct
{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIO_TypeDef;

#define PERIPH_BASE  0x40000000U
#define AHB1_BASE    (PERIPH_BASE + 0x20000U)
#define GPIOA_BASE   (AHB1_BASE + 0x0000U)
#define GPIOB_BASE   (AHB1_BASE + 0x0400U)
#define RCC_BASE     (AHB1_BASE + 0x3800U)

#define GPIOA ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB ((GPIO_TypeDef *) GPIOB_BASE)

#define RCC_AHB1ENR ((volatile uint32_t *)(RCC_BASE + 0x30U))

void GPIO_Config(void);
void escribir(GPIO_TypeDef *puerto, uint8_t pin, uint8_t valor);
void apagar_digitos(void);
void mostrar_numero(uint8_t numero);
void mostrar_3_digitos_250ms(uint16_t numero);
void HALT(void);

#endif