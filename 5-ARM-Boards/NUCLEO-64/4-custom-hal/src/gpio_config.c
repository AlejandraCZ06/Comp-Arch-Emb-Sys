#include "gpio_config.h"


void GPIO_Config(void)
{
    /* ==================================================
     * ACTIVAR RELOJ DE GPIOA, GPIOB Y GPIOC
     * ================================================== */

    RCC->AHB1ENR |= (1 << 0);   // GPIOA
    RCC->AHB1ENR |= (1 << 1);   // GPIOB
    RCC->AHB1ENR |= (1 << 2);   // GPIOC

    volatile unsigned int dummy;

    dummy = RCC->AHB1ENR;
    dummy = RCC->AHB1ENR;


    /* ==================================================
     * BOTONES
     *
     * Botón 1 -> PC0
     * Botón 2 -> PC1
     * Botón 3 -> PB8
     * Botón 4 -> PB13
     * Botón 5 -> PB14
     * Botón 6 -> PC5
     * Botón 7 -> PC6
     * Botón 8 -> PC7
     * Botón 9 -> PA10
     *
     * GPIO -> BOTÓN -> GND
     *
     * Pull-up interno:
     * 1 = sin presionar
     * 0 = presionado
     * ================================================== */


    /* Botón 1 -> PC0 */
    GPIOC->MODER &= ~(3 << (0 * 2));
    GPIOC->PUPDR &= ~(3 << (0 * 2));
    GPIOC->PUPDR |=  (1 << (0 * 2));


    /* Botón 2 -> PC1 */
    GPIOC->MODER &= ~(3 << (1 * 2));
    GPIOC->PUPDR &= ~(3 << (1 * 2));
    GPIOC->PUPDR |=  (1 << (1 * 2));


    /* Botón 3 -> PB8 */
    GPIOB->MODER &= ~(3 << (8 * 2));
    GPIOB->PUPDR &= ~(3 << (8 * 2));
    GPIOB->PUPDR |=  (1 << (8 * 2));


    /* Botón 4 -> PB13 */
    GPIOB->MODER &= ~(3 << (13 * 2));
    GPIOB->PUPDR &= ~(3 << (13 * 2));
    GPIOB->PUPDR |=  (1 << (13 * 2));


    /* Botón 5 -> PB14 */
    GPIOB->MODER &= ~(3 << (14 * 2));
    GPIOB->PUPDR &= ~(3 << (14 * 2));
    GPIOB->PUPDR |=  (1 << (14 * 2));


    /* Botón 6 -> PC5 */
    GPIOC->MODER &= ~(3 << (5 * 2));
    GPIOC->PUPDR &= ~(3 << (5 * 2));
    GPIOC->PUPDR |=  (1 << (5 * 2));


    /* Botón 7 -> PC6 */
    GPIOC->MODER &= ~(3 << (6 * 2));
    GPIOC->PUPDR &= ~(3 << (6 * 2));
    GPIOC->PUPDR |=  (1 << (6 * 2));


    /* Botón 8 -> PC7 */
    GPIOC->MODER &= ~(3 << (7 * 2));
    GPIOC->PUPDR &= ~(3 << (7 * 2));
    GPIOC->PUPDR |=  (1 << (7 * 2));


    /* Botón 9 -> PA10 */
    GPIOA->MODER &= ~(3 << (10 * 2));
    GPIOA->PUPDR &= ~(3 << (10 * 2));
    GPIOA->PUPDR |=  (1 << (10 * 2));


    /* ==================================================
     * LEDS ROJOS
     *
     * Rojo 1 -> PA0
     * Rojo 2 -> PA1
     * Rojo 3 -> PB9
     * Rojo 4 -> PA8
     * Rojo 5 -> PA4
     * Rojo 6 -> PA6
     * Rojo 7 -> PA5
     * Rojo 8 -> PA7
     * Rojo 9 -> PB15
     *
     * GPIO -> resistencia -> LED -> GND
     *
     * 1 = encendido
     * 0 = apagado
     * ================================================== */


    /* LED rojo 1 -> PA0 */
    GPIOA->MODER &= ~(3 << (0 * 2));
    GPIOA->MODER |=  (1 << (0 * 2));


    /* LED rojo 2 -> PA1 */
    GPIOA->MODER &= ~(3 << (1 * 2));
    GPIOA->MODER |=  (1 << (1 * 2));


    /* LED rojo 3 -> PB9 */
    GPIOB->MODER &= ~(3 << (9 * 2));
    GPIOB->MODER |=  (1 << (9 * 2));


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


    /* LED rojo 9 -> PB15 */
    GPIOB->MODER &= ~(3 << (15 * 2));
    GPIOB->MODER |=  (1 << (15 * 2));


    /* ==================================================
     * LEDS AZULES
     *
     * Azul 1 -> CN8-4  -> PA4
     * Azul 2 -> CN10-24 -> PB1
     * Azul 3 -> CN10-22 -> PB2
     * Azul 4 -> CN10-31 -> PB3
     * Azul 5 -> CN10-27 -> PB4
     * Azul 6 -> CN10-29 -> PB5
     * Azul 7 -> CN10-17 -> PB6
     * Azul 8 -> CN10-21 -> PB7
     * Azul 9 -> CN9-1 -> PA3
     *
     * GPIO -> resistencia -> LED -> GND
     *
     * 1 = encendido
     * 0 = apagado
     * ================================================== */


    /* LED azul 1 -> PA4 */
    GPIOA->MODER &= ~(3 << (4 * 2));
    GPIOA->MODER |=  (1 << (4 * 2));


    /* LED azul 2 -> PB1 */
    GPIOB->MODER &= ~(3 << (1 * 2));
    GPIOB->MODER |=  (1 << (1 * 2));


    /* LED azul 3 -> PB2 */
    GPIOB->MODER &= ~(3 << (2 * 2));
    GPIOB->MODER |=  (1 << (2 * 2));


    /* LED azul 4 -> PB3 */
    GPIOB->MODER &= ~(3 << (3 * 2));
    GPIOB->MODER |=  (1 << (3 * 2));


    /* LED azul 5 -> PB4 */
    GPIOB->MODER &= ~(3 << (4 * 2));
    GPIOB->MODER |=  (1 << (4 * 2));


    /* LED azul 6 -> PB5 */
    GPIOB->MODER &= ~(3 << (5 * 2));
    GPIOB->MODER |=  (1 << (5 * 2));


    /* LED azul 7 -> PB6 */
    GPIOB->MODER &= ~(3 << (6 * 2));
    GPIOB->MODER |=  (1 << (6 * 2));


    /* LED azul 8 -> PB7 */
    GPIOB->MODER &= ~(3 << (7 * 2));
    GPIOB->MODER |=  (1 << (7 * 2));


    /* LED azul 9 -> PA3 */
    GPIOA->MODER &= ~(3 << (3 * 2));
    GPIOA->MODER |=  (1 << (3 * 2));


    /* ==================================================
     * APAGAR TODOS LOS LEDS AL INICIO
     * ================================================== */

    /* ROJOS */
    write_pin_state(GPIOA, 0, 0);
    write_pin_state(GPIOA, 1, 0);
    write_pin_state(GPIOB, 9, 0);
    write_pin_state(GPIOA, 8, 0);
    write_pin_state(GPIOA, 4, 0);
    write_pin_state(GPIOA, 6, 0);
    write_pin_state(GPIOA, 5, 0);
    write_pin_state(GPIOA, 7, 0);
    write_pin_state(GPIOB, 15, 0);

    /* AZULES */
    write_pin_state(GPIOA, 4, 0);
    write_pin_state(GPIOB, 1, 0);
    write_pin_state(GPIOB, 2, 0);
    write_pin_state(GPIOB, 3, 0);
    write_pin_state(GPIOB, 4, 0);
    write_pin_state(GPIOB, 5, 0);
    write_pin_state(GPIOB, 6, 0);
    write_pin_state(GPIOB, 7, 0);
    write_pin_state(GPIOA, 3, 0);
}


/* ==================================================
 * LEER GPIO
 * ================================================== */

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


/* ==================================================
 * ESCRIBIR GPIO
 * ================================================== */

void write_pin_state(volatile GPIO_TypeDef *GPIOx,
                     uint8_t pin,
                     uint8_t state)
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


/* ==================================================
 * BOTON PRESIONADO
 * ================================================== */

uint8_t boton_presionado(uint8_t boton)
{
    switch (boton)
    {
        case 1:
            return read_pin_state(GPIOC, 0) == 0;

        case 2:
            return read_pin_state(GPIOC, 1) == 0;

        case 3:
            return read_pin_state(GPIOB, 8) == 0;

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
            return read_pin_state(GPIOA, 10) == 0;

        default:
            return 0;
    }
}


/* ==================================================
 * CONTROL DE LED ROJO
 * ================================================== */

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
            write_pin_state(GPIOB, 9, estado);
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
            write_pin_state(GPIOB, 15, estado);
            break;

        default:
            break;
    }
}


/* ==================================================
 * CONTROL DE LED AZUL
 * ================================================== */

void led_azul(uint8_t led, uint8_t estado)
{
    switch (led)
    {
        case 1:
            write_pin_state(GPIOA, 4, estado);
            break;

        case 2:
            write_pin_state(GPIOB, 1, estado);
            break;

        case 3:
            write_pin_state(GPIOB, 2, estado);
            break;

        case 4:
            write_pin_state(GPIOB, 3, estado);
            break;

        case 5:
            write_pin_state(GPIOB, 4, estado);
            break;

        case 6:
            write_pin_state(GPIOB, 5, estado);
            break;

        case 7:
            write_pin_state(GPIOB, 6, estado);
            break;

        case 8:
            write_pin_state(GPIOB, 7, estado);
            break;

        case 9:
            write_pin_state(GPIOA, 3, estado);
            break;

        default:
            break;
    }
}