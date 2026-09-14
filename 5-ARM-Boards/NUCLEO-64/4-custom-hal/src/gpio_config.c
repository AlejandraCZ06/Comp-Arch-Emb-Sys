#include "gpio.h"

static void configurar_salida(GPIO_TypeDef *puerto, uint8_t pin)
{
    puerto->MODER &= ~(3U << (pin * 2));
    puerto->MODER |= (1U << (pin * 2));
}

static void configurar_entrada_pullup(GPIO_TypeDef *puerto, uint8_t pin)
{
    puerto->MODER &= ~(3U << (pin * 2));

    puerto->PUPDR &= ~(3U << (pin * 2));
    puerto->PUPDR |= (1U << (pin * 2));
}

void GPIO_Config(void)
{
    *RCC_AHB1ENR |= (1U << 0);
    *RCC_AHB1ENR |= (1U << 1);
    *RCC_AHB1ENR |= (1U << 2);

    configurar_entrada_pullup(GPIOC, 0);
    configurar_entrada_pullup(GPIOC, 1);
    configurar_entrada_pullup(GPIOC, 2);
    configurar_entrada_pullup(GPIOC, 3);
    configurar_entrada_pullup(GPIOC, 4);
    configurar_entrada_pullup(GPIOC, 5);
    configurar_entrada_pullup(GPIOC, 6);
    configurar_entrada_pullup(GPIOC, 7);

    configurar_salida(GPIOA, 8);
    configurar_salida(GPIOA, 9);
    configurar_salida(GPIOA, 10);

    configurar_salida(GPIOB, 3);
    configurar_salida(GPIOB, 4);
    configurar_salida(GPIOB, 5);
    configurar_salida(GPIOB, 6);
    configurar_salida(GPIOB, 8);

    configurar_salida(GPIOB, 9);
    configurar_salida(GPIOB, 10);
    configurar_salida(GPIOA, 6);
}

void escribir(GPIO_TypeDef *puerto, uint8_t pin, uint8_t valor)
{
    if (valor)
        puerto->ODR |= (1U << pin);
    else
        puerto->ODR &= ~(1U << pin);
}

uint8_t leer_dip(void)
{
    uint8_t valor;

    valor = GPIOC->IDR & 0xFF;

    valor = ~valor;

    return valor;
}

void apagar_digitos(void)
{
    escribir(GPIOB, 9, 1);
    escribir(GPIOB, 10, 1);
    escribir(GPIOA, 6, 1);
}

void mostrar_numero(uint8_t numero)
{
    if (numero == 0 || numero == 2 || numero == 3 ||
        numero == 5 || numero == 6 || numero == 7 ||
        numero == 8 || numero == 9)
        escribir(GPIOA, 8, 0);
    else
        escribir(GPIOA, 8, 1);

    if (numero == 0 || numero == 1 || numero == 2 ||
        numero == 3 || numero == 4 || numero == 7 ||
        numero == 8 || numero == 9)
        escribir(GPIOA, 9, 0);
    else
        escribir(GPIOA, 9, 1);

    if (numero == 0 || numero == 1 || numero == 3 ||
        numero == 4 || numero == 5 || numero == 6 ||
        numero == 7 || numero == 8 || numero == 9)
        escribir(GPIOA, 10, 0);
    else
        escribir(GPIOA, 10, 1);

    if (numero == 0 || numero == 2 || numero == 3 ||
        numero == 5 || numero == 6 || numero == 8 ||
        numero == 9)
        escribir(GPIOB, 3, 0);
    else
        escribir(GPIOB, 3, 1);

    if (numero == 0 || numero == 2 ||
        numero == 6 || numero == 8)
        escribir(GPIOB, 4, 0);
    else
        escribir(GPIOB, 4, 1);

    if (numero == 0 || numero == 4 || numero == 5 ||
        numero == 6 || numero == 8 || numero == 9)
        escribir(GPIOB, 5, 0);
    else
        escribir(GPIOB, 5, 1);

    if (numero == 2 || numero == 3 || numero == 4 ||
        numero == 5 || numero == 6 || numero == 8 ||
        numero == 9)
        escribir(GPIOB, 6, 0);
    else
        escribir(GPIOB, 6, 1);

    escribir(GPIOB, 8, 1);
}

void retardo(volatile uint32_t ciclos)
{
    while (ciclos--)
    {
    }
}

void HALT(void)
{
    while (1)
    {
        __asm volatile ("nop");
    }
}