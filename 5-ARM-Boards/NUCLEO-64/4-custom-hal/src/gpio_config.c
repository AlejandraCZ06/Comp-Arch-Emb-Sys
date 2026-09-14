#include "gpio_config.h"

void GPIO_Config(void)
{
    /* Activar reloj de GPIOA, GPIOB y GPIOC */
    RCC->AHB1ENR |= (1 << 0);   // GPIOA
    RCC->AHB1ENR |= (1 << 1);   // GPIOB
    RCC->AHB1ENR |= (1 << 2);   // GPIOC

    volatile unsigned int dummy;
    dummy = RCC->AHB1ENR;
    dummy = RCC->AHB1ENR;

    /*
     * BOTONES
     *
     * PC0-PC8 como entradas
     */
    GPIOC->MODER_Bits.MODER0 = 0;
    GPIOC->MODER_Bits.MODER1 = 0;
    GPIOC->MODER_Bits.MODER2 = 0;
    GPIOC->MODER_Bits.MODER3 = 0;
    GPIOC->MODER_Bits.MODER4 = 0;
    GPIOC->MODER_Bits.MODER5 = 0;
    GPIOC->MODER_Bits.MODER6 = 0;
    GPIOC->MODER_Bits.MODER7 = 0;
    GPIOC->MODER_Bits.MODER8 = 0;

    /*
     * Pull-up interno para los botones
     *
     * 01 = pull-up
     */
    GPIOC->PUPDR_Bits.PUPDR0 = 1;
    GPIOC->PUPDR_Bits.PUPDR1 = 1;
    GPIOC->PUPDR_Bits.PUPDR2 = 1;
    GPIOC->PUPDR_Bits.PUPDR3 = 1;
    GPIOC->PUPDR_Bits.PUPDR4 = 1;
    GPIOC->PUPDR_Bits.PUPDR5 = 1;
    GPIOC->PUPDR_Bits.PUPDR6 = 1;
    GPIOC->PUPDR_Bits.PUPDR7 = 1;
    GPIOC->PUPDR_Bits.PUPDR8 = 1;


    /*
     * LED ROJO
     *
     * R1  = PA0
     * R2  = PA1
     * R3  = PB10
     * R4  = PA8
     * R5  = PA4
     * R6  = PA6
     * R7  = PA5
     * R8  = PA7
     * R9  = PA3
     */

    GPIOA->MODER_Bits.MODER0 = 1;
    GPIOA->MODER_Bits.MODER1 = 1;
    GPIOA->MODER_Bits.MODER3 = 1;
    GPIOA->MODER_Bits.MODER4 = 1;
    GPIOA->MODER_Bits.MODER5 = 1;
    GPIOA->MODER_Bits.MODER6 = 1;
    GPIOA->MODER_Bits.MODER7 = 1;
    GPIOA->MODER_Bits.MODER8 = 1;

    GPIOB->MODER_Bits.MODER10 = 1;
}


/*
 * Leer el estado de un GPIO
 */
uint8_t read_pin_state(volatile GPIO_TypeDef *GPIOx, uint8_t pin)
{
    if (GPIOx->IDR & (1 << pin))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/*
 * Escribir en un GPIO
 */
void write_pin_state(volatile GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t state)
{
    if (state)
    {
        GPIOx->ODR |= (1 << pin);
    }
    else
    {
        GPIOx->ODR &= ~(1 << pin);
    }
}


/*
 * Saber si un botón está presionado
 *
 * Los botones tienen pull-up:
 *
 * Sin presionar = 1
 * Presionado    = 0
 */
uint8_t boton_presionado(uint8_t boton)
{
    switch (boton)
    {
        case 1:
            return read_pin_state(GPIOC, 0) == 0;

        case 2:
            return read_pin_state(GPIOC, 1) == 0;

        case 3:
            return read_pin_state(GPIOC, 2) == 0;

        case 4:
            return read_pin_state(GPIOC, 3) == 0;

        case 5:
            return read_pin_state(GPIOC, 4) == 0;

        case 6:
            return read_pin_state(GPIOC, 5) == 0;

        case 7:
            return read_pin_state(GPIOC, 6) == 0;

        case 8:
            return read_pin_state(GPIOC, 7) == 0;

        case 9:
            return read_pin_state(GPIOC, 8) == 0;

        default:
            return 0;
    }
}


/*
 * Encender o apagar LED rojo
 */
void led_rojo(uint8_t led, uint8_t estado)
{
    switch (led)
    {
        case 1:
            write_pin_state(GPIOA, 0, estado);
            break;

        case 2:
            write_pin_state(GPIOA, 1, estado);
            break;

        case 3:
            write_pin_state(GPIOB, 10, estado);
            break;

        case 4:
            write_pin_state(GPIOA, 8, estado);
            break;

        case 5:
            write_pin_state(GPIOA, 4, estado);
            break;

        case 6:
            write_pin_state(GPIOA, 6, estado);
            break;

        case 7:
            write_pin_state(GPIOA, 5, estado);
            break;

        case 8:
            write_pin_state(GPIOA, 7, estado);
            break;

        case 9:
            write_pin_state(GPIOA, 3, estado);
            break;

        default:
            break;
    }
}