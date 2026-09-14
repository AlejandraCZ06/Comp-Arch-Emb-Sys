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
     * Boton 1 -> PC0
     * Boton 2 -> PC1
     * Boton 3 -> PB8   (CN10-3)
     * Boton 4 -> PB13
     * Boton 5 -> PB14
     * Boton 6 -> PC5
     * Boton 7 -> PC6
     * Boton 8 -> PC7
     * Boton 9 -> PB10  (Pulsador 9 en PB10)
     */

    /* Boton 1 -> PC0 */
    GPIOC->MODER &= ~(3 << (0 * 2));
    GPIOC->PUPDR &= ~(3 << (0 * 2));
    GPIOC->PUPDR |=  (1 << (0 * 2));

    /* Boton 2 -> PC1 */
    GPIOC->MODER &= ~(3 << (1 * 2));
    GPIOC->PUPDR &= ~(3 << (1 * 2));
    GPIOC->PUPDR |=  (1 << (1 * 2));

    /* Boton 3 -> PB8 */
    GPIOB->MODER &= ~(3 << (8 * 2));
    GPIOB->PUPDR &= ~(3 << (8 * 2));
    GPIOB->PUPDR |=  (1 << (8 * 2));

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

    /* Boton 9 -> PB10 */
    GPIOB->MODER &= ~(3 << (10 * 2));
    GPIOB->PUPDR &= ~(3 << (10 * 2));
    GPIOB->PUPDR |=  (1 << (10 * 2));


    /*
     * ==================================================
     * LEDS ROJOS
     * ==================================================
     *
     * Rojo 1 -> PA0
     * Rojo 2 -> PA1
     * Rojo 3 -> PB9   (LED 3 en PB9)
     * Rojo 4 -> PA8
     * Rojo 5 -> PA4
     * Rojo 6 -> PA6
     * Rojo 7 -> PA5
     * Rojo 8 -> PA7
     * Rojo 9 -> PC8   (LED 9 movido a PC8 para liberar PC9)
     */

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

    /* LED rojo 9 -> PC8 */
    GPIOC->MODER &= ~(3 << (8 * 2));
    GPIOC->MODER |=  (1 << (8 * 2));


    /*
     * ==================================================
     * APAGAR TODOS LOS LEDS AL INICIO
     * ==================================================
     */

    write_pin_state(GPIOA, 0, 0);
    write_pin_state(GPIOA, 1, 0);
    write_pin_state(GPIOB, 9, 0);   // Apagar LED 3 en PB9
    write_pin_state(GPIOA, 8, 0);
    write_pin_state(GPIOA, 4, 0);
    write_pin_state(GPIOA, 6, 0);
    write_pin_state(GPIOA, 5, 0);
    write_pin_state(GPIOA, 7, 0);
    write_pin_state(GPIOC, 8, 0);   // Apagar LED 9 en PC8
}


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

uint8_t boton_presionado(uint8_t boton)
{
    switch (boton)
    {
        case 1:
            return read_pin_state(GPIOC, 0) == 0;
        case 2:
            return read_pin_state(GPIOC, 1) == 0;
        case 3:
            return read_pin_state(GPIOB, 8) == 0;   // Botón 3 en PB8
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
            return read_pin_state(GPIOB, 10) == 0;  // Botón 9 en PB10
        default:
            return 0;
    }
}

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
            write_pin_state(GPIOB, 9, estado);  // LED 3 en PB9
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
            write_pin_state(GPIOC, 8, estado);  // LED 9 en PC8
            break;
        default:
            break;
    }
}