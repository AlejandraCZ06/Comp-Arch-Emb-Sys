#include "gpio_config.h"


/*
 * Configuración de GPIO
 *
 * PA0 - PA8  -> LEDs rojos
 * PB0 - PB8  -> LEDs verdes
 * PC0 - PC8  -> Botones
 *
 * En esta primera prueba solamente
 * utilizamos PA0-PA8 y PC0-PC8.
 */


void GPIO_Config(void)
{
    uint8_t i;


    /* =====================================
       ACTIVAR RELOJ DE GPIOA, GPIOB Y GPIOC
       ===================================== */

    RCC->AHB1ENR |= (1 << 0);   // GPIOA
    RCC->AHB1ENR |= (1 << 1);   // GPIOB
    RCC->AHB1ENR |= (1 << 2);   // GPIOC


    /* =====================================
       PA0 - PA8
       LEDs ROJOS
       Configurar como salida
       ===================================== */

    for (i = 0; i < 9; i++)
    {
        GPIOA->MODER &= ~(3 << (i * 2));
        GPIOA->MODER |=  (1 << (i * 2));

        /* LED inicialmente apagado */

        GPIOA->ODR &= ~(1 << i);
    }


    /* =====================================
       PB0 - PB8
       LEDs VERDES
       Configurar como salida
       ===================================== */

    for (i = 0; i < 9; i++)
    {
        GPIOB->MODER &= ~(3 << (i * 2));
        GPIOB->MODER |=  (1 << (i * 2));

        /* LED inicialmente apagado */

        GPIOB->ODR &= ~(1 << i);
    }


    /* =====================================
       PC0 - PC8
       BOTONES
       Entrada con PULL-UP
       ===================================== */

    for (i = 0; i < 9; i++)
    {
        /* Configurar como entrada */

        GPIOC->MODER &= ~(3 << (i * 2));


        /* Activar pull-up */

        GPIOC->PUPDR &= ~(3 << (i * 2));
        GPIOC->PUPDR |=  (1 << (i * 2));
    }
}


/*
 * Leer el estado de un pin
 *
 * Devuelve:
 * 0 -> pin en LOW
 * 1 -> pin en HIGH
 */

uint8_t read_pin_state(volatile GPIO_TypeDef *GPIOx, uint8_t pin)
{
    if (GPIOx->IDR & (1 << pin))
    {
        return 1;
    }

    return 0;
}


/*
 * Escribir el estado de un pin
 *
 * state = 0 -> LOW
 * state = 1 -> HIGH
 */

void write_pin_state(volatile GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t state)
{
    if (state == 1)
    {
        GPIOx->ODR |= (1 << pin);
    }
    else
    {
        GPIOx->ODR &= ~(1 << pin);
    }
}


/*
 * Leer botón
 *
 * Los botones tienen PULL-UP interno:
 *
 * Sin presionar -> 1
 * Presionado    -> 0
 *
 * Esta función devuelve:
 *
 * 1 -> botón presionado
 * 0 -> botón no presionado
 */

uint8_t boton_presionado(uint8_t posicion)
{
    if (read_pin_state(GPIOC, posicion) == 0)
    {
        return 1;
    }

    return 0;
}


/*
 * Controlar LED rojo
 *
 * posicion:
 * 0 -> LED rojo 1
 * 1 -> LED rojo 2
 * ...
 * 8 -> LED rojo 9
 */

void led_rojo(uint8_t posicion, uint8_t estado)
{
    write_pin_state(GPIOA, posicion, estado);
}