#include "gpio_config.h"

void GPIO_Config(void)
{
    /* Activar reloj de GPIOA, GPIOB y GPIOC */
    RCC->AHB1ENR |= (1 << 0);
    RCC->AHB1ENR |= (1 << 1);
    RCC->AHB1ENR |= (1 << 2);

    volatile unsigned int dummy;
    dummy = RCC->AHB1ENR;
    dummy = RCC->AHB1ENR;


    /*
     * =========================
     * BOTONES
     * =========================
     *
     * Boton 1 -> PC0
     * Boton 2 -> PC1
     * Boton 3 -> PC2
     * Boton 4 -> PC3
     * Boton 5 -> PC4
     * Boton 6 -> PC5
     * Boton 7 -> PC6
     * Boton 8 -> PC7
     * Boton 9 -> PC8
     *
     * MODER = 00 -> entrada
     */

    GPIOC->MODER &= ~(3 << (0 * 2));
    GPIOC->MODER &= ~(3 << (1 * 2));
    GPIOC->MODER &= ~(3 << (2 * 2));
    GPIOC->MODER &= ~(3 << (3 * 2));
    GPIOC->MODER &= ~(3 << (4 * 2));
    GPIOC->MODER &= ~(3 << (5 * 2));
    GPIOC->MODER &= ~(3 << (6 * 2));
    GPIOC->MODER &= ~(3 << (7 * 2));
    GPIOC->MODER &= ~(3 << (8 * 2));


    /*
     * Pull-up interno
     *
     * PUPDR:
     * 00 = sin pull
     * 01 = pull-up
     * 10 = pull-down
     *
     * Cada pin utiliza 2 bits.
     */

    GPIOC->PUPDR |= (1 << (0 * 2));
    GPIOC->PUPDR |= (1 << (1 * 2));
    GPIOC->PUPDR |= (1 << (2 * 2));
    GPIOC->PUPDR |= (1 << (3 * 2));
    GPIOC->PUPDR |= (1 << (4 * 2));
    GPIOC->PUPDR |= (1 << (5 * 2));
    GPIOC->PUPDR |= (1 << (6 * 2));
    GPIOC->PUPDR |= (1 << (7 * 2));
    GPIOC->PUPDR |= (1 << (8 * 2));


    /*
     * =========================
     * LEDS ROJOS
     * =========================
     *
     * Rojo 1 -> PA0
     * Rojo 2 -> PA1
     * Rojo 3 -> PB10
     * Rojo 4 -> PA8
     * Rojo 5 -> PA4
     * Rojo 6 -> PA6
     * Rojo 7 -> PA5
     * Rojo 8 -> PA7
     * Rojo 9 -> PA3
     */

    GPIOA->MODER |= (1 << (0 * 2));
    GPIOA->MODER |= (1 << (1 * 2));
    GPIOA->MODER |= (1 << (3 * 2));
    GPIOA->MODER |= (1 << (4 * 2));
    GPIOA->MODER |= (1 << (5 * 2));
    GPIOA->MODER |= (1 << (6 * 2));
    GPIOA->MODER |= (1 << (7 * 2));
    GPIOA->MODER |= (1 << (8 * 2));

    GPIOB->MODER |= (1 << (10 * 2));


    /*
     * Apagar todos los LEDs rojos al comenzar
     */

    write_pin_state(GPIOA, 0, 0);
    write_pin_state(GPIOA, 1, 0);
    write_pin_state(GPIOB, 10, 0);
    write_pin_state(GPIOA, 8, 0);
    write_pin_state(GPIOA, 4, 0);
    write_pin_state(GPIOA, 6, 0);
    write_pin_state(GPIOA, 5, 0);
    write_pin_state(GPIOA, 7, 0);
    write_pin_state(GPIOA, 3, 0);
}


/*
 * Leer estado de un pin
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
 * Escribir estado de un pin
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
 * Leer los botones
 *
 * Como usamos pull-up:
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
 * Controlar los LEDs rojos
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