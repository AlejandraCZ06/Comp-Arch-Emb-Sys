#include "gpio_config.h"

void configurar_salida(GPIO_TypeDef *puerto, uint8_t pin)
{
    puerto->MODER &= ~(3U << (pin * 2));
    puerto->MODER |= (1U << (pin * 2));
}

void configurar_entrada_pullup(GPIO_TypeDef *puerto, uint8_t pin)
{
    puerto->MODER &= ~(3U << (pin * 2));
    puerto->PUPDR &= ~(3U << (pin * 2));
    puerto->PUPDR |= (1U << (pin * 2));
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
    switch (numero)
    {
        case 0:
            escribir(GPIOA, 8, 0);
            escribir(GPIOA, 9, 0);
            escribir(GPIOA, 10, 0);
            escribir(GPIOB, 3, 0);
            escribir(GPIOB, 4, 0);
            escribir(GPIOB, 5, 0);
            escribir(GPIOB, 6, 1);
            break;

        case 1:
            escribir(GPIOA, 8, 1);
            escribir(GPIOA, 9, 0);
            escribir(GPIOA, 10, 0);
            escribir(GPIOB, 3, 1);
            escribir(GPIOB, 4, 1);
            escribir(GPIOB, 5, 1);
            escribir(GPIOB, 6, 1);
            break;

        case 2:
            escribir(GPIOA, 8, 0);
            escribir(GPIOA, 9, 0);
            escribir(GPIOA, 10, 1);
            escribir(GPIOB, 3, 0);
            escribir(GPIOB, 4, 0);
            escribir(GPIOB, 5, 1);
            escribir(GPIOB, 6, 0);
            break;

        case 3:
            escribir(GPIOA, 8, 0);
            escribir(GPIOA, 9, 0);
            escribir(GPIOA, 10, 0);
            escribir(GPIOB, 3, 0);
            escribir(GPIOB, 4, 1);
            escribir(GPIOB, 5, 1);
            escribir(GPIOB, 6, 0);
            break;

        case 4:
            escribir(GPIOA, 8, 1);
            escribir(GPIOA, 9, 0);
            escribir(GPIOA, 10, 0);
            escribir(GPIOB, 3, 1);
            escribir(GPIOB, 4, 1);
            escribir(GPIOB, 5, 0);
            escribir(GPIOB, 6, 0);
            break;

        case 5:
            escribir(GPIOA, 8, 0);
            escribir(GPIOA, 9, 1);
            escribir(GPIOA, 10, 0);
            escribir(GPIOB, 3, 0);
            escribir(GPIOB, 4, 1);
            escribir(GPIOB, 5, 0);
            escribir(GPIOB, 6, 0);
            break;

        case 6:
            escribir(GPIOA, 8, 0);
            escribir(GPIOA, 9, 1);
            escribir(GPIOA, 10, 0);
            escribir(GPIOB, 3, 0);
            escribir(GPIOB, 4, 0);
            escribir(GPIOB, 5, 0);
            escribir(GPIOB, 6, 0);
            break;

        case 7:
            escribir(GPIOA, 8, 0);
            escribir(GPIOA, 9, 0);
            escribir(GPIOA, 10, 0);
            escribir(GPIOB, 3, 1);
            escribir(GPIOB, 4, 1);
            escribir(GPIOB, 5, 1);
            escribir(GPIOB, 6, 1);
            break;

        case 8:
            escribir(GPIOA, 8, 0);
            escribir(GPIOA, 9, 0);
            escribir(GPIOA, 10, 0);
            escribir(GPIOB, 3, 0);
            escribir(GPIOB, 4, 0);
            escribir(GPIOB, 5, 0);
            escribir(GPIOB, 6, 0);
            break;

        case 9:
            escribir(GPIOA, 8, 0);
            escribir(GPIOA, 9, 0);
            escribir(GPIOA, 10, 0);
            escribir(GPIOB, 3, 0);
            escribir(GPIOB, 4, 1);
            escribir(GPIOB, 5, 0);
            escribir(GPIOB, 6, 0);
            break;

        default:
            break;
    }
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