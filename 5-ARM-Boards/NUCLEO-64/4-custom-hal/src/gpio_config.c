#include "gpio_config.h"

void GPIO_Config(void)
{
    /*
     * ==================================================
     * ACTIVAR RELOJ DE GPIOA, GPIOB Y GPIOC
     * ==================================================
     */

    RCC->AHB1ENR |= (1 << 0);   // GPIOA
    RCC->AHB1ENR |= (1 << 1);   // GPIOB
    RCC->AHB1ENR |= (1 << 2);   // GPIOC

    volatile unsigned int dummy;
    dummy = RCC->AHB1ENR;
    dummy = RCC->AHB1ENR;


    /*
     * ==================================================
     * BOTONES
     * ==================================================
     *
     * Boton 1 -> PC0 -> CN8-6
     * Boton 2 -> PC1 -> CN8-5
     * Boton 3 -> PB12 -> CN10-16
     * Boton 4 -> PB13 -> CN10-30
     * Boton 5 -> PB14 -> CN10-28
     * Boton 6 -> PC5 -> CN10-6
     * Boton 7 -> PC6 -> CN10-4
     * Boton 8 -> PC7 -> CN10-19
     * Boton 9 -> PB15 -> CN10-26
     */


    /* Boton 1 -> PC0 */
    GPIOC->MODER &= ~(3 << (0 * 2));
    GPIOC->PUPDR &= ~(3 << (0 * 2));
    GPIOC->PUPDR |=  (1 << (0 * 2));


    /* Boton 2 -> PC1 */
    GPIOC->MODER &= ~(3 << (1 * 2));
    GPIOC->PUPDR &= ~(3 << (1 * 2));
    GPIOC->PUPDR |=  (1 << (1 * 2));


    /* Boton 3 -> PB12 */
    GPIOB->MODER &= ~(3 << (12 * 2));
    GPIOB->PUPDR &= ~(3 << (12 * 2));
    GPIOB->PUPDR |=  (1 << (12 * 2));


    /* Boton 4 -> PB13 */
    GPIOB->MODER &= ~(3 << (13 * 2));
    GPIOB->PUPDR &= ~(3 << (13 * 2));
    GPIOB->PUPDR |=  (1 << (13 * 2));


    /* Boton 5 -> PB14 */
    GPIOB->MODER &= ~(3 << (14 * 2));
    GPIOB->PUPDR &= ~(3 << (14 * 2));
    GPIOB->PUPDR |=  (1 << (14 * 2));


    /* Boton 6 -> PC5 */
    GPIOC->MODER &= ~(3 << (5 * 2));
    GPIOC->PUPDR &= ~(3 << (5 * 2));
    GPIOC->PUPDR |=  (1 << (5 * 2));


    /* Boton 7 -> PC6 */
    GPIOC->MODER &= ~(3 << (6 * 2));
    GPIOC->PUPDR &= ~(3 << (6 * 2));
    GPIOC->PUPDR |=  (1 << (6 * 2));


    /* Boton 8 -> PC7 */
    GPIOC->MODER &= ~(3 << (7 * 2));
    GPIOC->PUPDR &= ~(3 << (7 * 2));
    GPIOC->PUPDR |=  (1 << (7 * 2));


    /* Boton 9 -> PB15 */
    GPIOB->MODER &= ~(3 << (15 * 2));
    GPIOB->PUPDR &= ~(3 << (15 * 2));
    GPIOB->PUPDR |=  (1 << (15 * 2));


    /*
     * ==================================================
     * LEDS ROJOS
     * ==================================================
     *
     * Rojo 1 -> PA0 -> CN8-1
     * Rojo 2 -> PA1 -> CN8-2
     * Rojo 3 -> PB10 -> CN9-7
     * Rojo 4 -> PA8 -> CN9-8
     * Rojo 5 -> PA4 -> CN8-3
     * Rojo 6 -> PA6 -> CN5-5
     * Rojo 7 -> PA5 -> CN5-6
     * Rojo 8 -> PA7 -> CN5-4
     * Rojo 9 -> PC9 -> CN10-1
     *
     * GPIO -> resistencia -> LED -> GND
     *
     * 1 = encendido
     * 0 = apagado
     */


    /* LED rojo 1 -> PA0 */
    GPIOA->MODER &= ~(3 << (0 * 2));
    GPIOA->MODER |=  (1 << (0 * 2));


    /* LED rojo 2 -> PA1 */
    GPIOA->MODER &= ~(3 << (1 * 2));
    GPIOA->MODER |=  (1 << (1 * 2));


    /* LED rojo 3 -> PB10 */
    GPIOB->MODER &= ~(3 << (10 * 2));
    GPIOB->MODER |=  (1 << (10 * 2));


    /* LED rojo 4 -> PA8 */
    GPIOA->MODER &= ~(3 << (8 * 2));
    GPIOA->MODER |=  (1 << (8 * 2));


    /* LED rojo 5 -> PA4 */
    GPIOA->MODER &= ~(3 << (4 * 2));
    GPIOA->MODER |=  (1 << (4 * 2));


    /* LED rojo 6 -> PA6 */
    GPIOA->MODER &= ~(3 << (6 * 2));
    GPIOA->MODER |=  (1 << (6 * 2));


    /* LED rojo 7 -> PA5 */
    GPIOA->MODER &= ~(3 << (5 * 2));
    GPIOA->MODER |=  (1 << (5 * 2));


    /* LED rojo 8 -> PA7 */
    GPIOA->MODER &= ~(3 << (7 * 2));
    GPIOA->MODER |=  (1 << (7 * 2));


    /* LED rojo 9 -> PC9 */
    GPIOC->MODER &= ~(3 << (9 * 2));
    GPIOC->MODER |=  (1 << (9 * 2));


    /*
     * ==================================================
     * APAGAR TODOS LOS LEDS AL INICIO
     * ==================================================
     */

    write_pin_state(GPIOA, 0, 0);
    write_pin_state(GPIOA, 1, 0);
    write_pin_state(GPIOB, 10, 0);
    write_pin_state(GPIOA, 8, 0);
    write_pin_state(GPIOA, 4, 0);
    write_pin_state(GPIOA, 6, 0);
    write_pin_state(GPIOA, 5, 0);
    write_pin_state(GPIOA, 7, 0);
    write_pin_state(GPIOC, 9, 0);
}


/*
 * ==================================================
 * LEER GPIO
 * ==================================================
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
 * ==================================================
 * ESCRIBIR GPIO
 * ==================================================
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
 * ==================================================
 * BOTON PRESIONADO
 * ==================================================
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
            return read_pin_state(GPIOB, 12) == 0;

        case 4:
            return read_pin_state(GPIOB, 13) == 0;

        case 5:
            return read_pin_state(GPIOB, 14) == 0;

        case 6:
            return read_pin_state(GPIOC, 5) == 0;

        case 7:
            return read_pin_state(GPIOC, 6) == 0;

        case 8:
            return read_pin_state(GPIOC, 7) == 0;

        case 9:
            return read_pin_state(GPIOB, 15) == 0;

        default:
            return 0;
    }
}


/*
 * ==================================================
 * CONTROL DE LED ROJO
 * ==================================================
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
            write_pin_state(GPIOC, 9, estado);
            break;

        default:
            break;
    }
}