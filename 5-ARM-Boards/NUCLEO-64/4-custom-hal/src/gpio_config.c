#include "gpio_config.h"


static void configurar_entrada_pullup(volatile GPIO_TypeDef *GPIOx,
                                      uint8_t pin)
{
    GPIOx->MODER &= ~(3 << (pin * 2));
    GPIOx->PUPDR &= ~(3 << (pin * 2));
    GPIOx->PUPDR |=  (1 << (pin * 2));
}


static void configurar_entrada(volatile GPIO_TypeDef *GPIOx,
                               uint8_t pin)
{
    GPIOx->MODER &= ~(3 << (pin * 2));
    GPIOx->PUPDR &= ~(3 << (pin * 2));
}


static void configurar_salida(volatile GPIO_TypeDef *GPIOx,
                              uint8_t pin)
{
    GPIOx->MODER &= ~(3 << (pin * 2));
    GPIOx->MODER |=  (1 << (pin * 2));
}


void GPIO_Config(void)
{
    RCC->AHB1ENR |= (1 << 0);
    RCC->AHB1ENR |= (1 << 1);
    RCC->AHB1ENR |= (1 << 2);

    volatile unsigned int dummy;
    dummy = RCC->AHB1ENR;
    dummy = RCC->AHB1ENR;


    /* Botones */

    configurar_entrada_pullup(GPIOC, 0);
    configurar_entrada_pullup(GPIOC, 1);
    configurar_entrada_pullup(GPIOB, 8);
    configurar_entrada_pullup(GPIOB, 13);
    configurar_entrada_pullup(GPIOB, 14);
    configurar_entrada_pullup(GPIOC, 5);
    configurar_entrada_pullup(GPIOC, 6);
    configurar_entrada_pullup(GPIOC, 7);
    configurar_entrada_pullup(GPIOA, 10);


    /* B1 */

    configurar_entrada(GPIOC, 13);


    /* LEDs rojos */

    configurar_salida(GPIOA, 0);
    configurar_salida(GPIOA, 1);
    configurar_salida(GPIOB, 9);
    configurar_salida(GPIOA, 8);
    configurar_salida(GPIOA, 4);
    configurar_salida(GPIOA, 6);
    configurar_salida(GPIOA, 5);
    configurar_salida(GPIOA, 7);
    configurar_salida(GPIOB, 15);


    /* LEDs azules */

    configurar_salida(GPIOB, 7);
    configurar_salida(GPIOB, 1);
    configurar_salida(GPIOB, 2);
    configurar_salida(GPIOB, 3);
    configurar_salida(GPIOB, 4);
    configurar_salida(GPIOB, 5);
    configurar_salida(GPIOB, 6);
    configurar_salida(GPIOC, 8);
    configurar_salida(GPIOA, 9);


    /* Apagar LEDs */

    write_pin_state(GPIOA, 0, 0);
    write_pin_state(GPIOA, 1, 0);
    write_pin_state(GPIOB, 9, 0);
    write_pin_state(GPIOA, 8, 0);
    write_pin_state(GPIOA, 4, 0);
    write_pin_state(GPIOA, 6, 0);
    write_pin_state(GPIOA, 5, 0);
    write_pin_state(GPIOA, 7, 0);
    write_pin_state(GPIOB, 15, 0);

    write_pin_state(GPIOB, 7, 0);
    write_pin_state(GPIOB, 1, 0);
    write_pin_state(GPIOB, 2, 0);
    write_pin_state(GPIOB, 3, 0);
    write_pin_state(GPIOB, 4, 0);
    write_pin_state(GPIOB, 5, 0);
    write_pin_state(GPIOB, 6, 0);
    write_pin_state(GPIOC, 8, 0);
    write_pin_state(GPIOA, 9, 0);
}


uint8_t read_pin_state(volatile GPIO_TypeDef *GPIOx, uint8_t pin)
{
    return (GPIOx->IDR & (1 << pin)) ? 1 : 0;
}


void write_pin_state(volatile GPIO_TypeDef *GPIOx,
                     uint8_t pin,
                     uint8_t state)
{
    if (state)
        GPIOx->ODR |= (1 << pin);
    else
        GPIOx->ODR &= ~(1 << pin);
}


uint8_t boton_presionado(uint8_t boton)
{
    switch (boton)
    {
        case 1: return read_pin_state(GPIOC, 0) == 0;
        case 2: return read_pin_state(GPIOC, 1) == 0;
        case 3: return read_pin_state(GPIOB, 8) == 0;
        case 4: return read_pin_state(GPIOB, 13) == 0;
        case 5: return read_pin_state(GPIOB, 14) == 0;
        case 6: return read_pin_state(GPIOC, 5) == 0;
        case 7: return read_pin_state(GPIOC, 6) == 0;
        case 8: return read_pin_state(GPIOC, 7) == 0;
        case 9: return read_pin_state(GPIOA, 10) == 0;
        default: return 0;
    }
}


uint8_t boton_start(void)
{
    return read_pin_state(GPIOC, 13) == 1;
}


void led_rojo(uint8_t led, uint8_t estado)
{
    switch (led)
    {
        case 1: write_pin_state(GPIOA, 0, estado); break;
        case 2: write_pin_state(GPIOA, 1, estado); break;
        case 3: write_pin_state(GPIOB, 9, estado); break;
        case 4: write_pin_state(GPIOA, 8, estado); break;
        case 5: write_pin_state(GPIOA, 4, estado); break;
        case 6: write_pin_state(GPIOA, 6, estado); break;
        case 7: write_pin_state(GPIOA, 5, estado); break;
        case 8: write_pin_state(GPIOA, 7, estado); break;
        case 9: write_pin_state(GPIOB, 15, estado); break;
        default: break;
    }
}


void led_azul(uint8_t led, uint8_t estado)
{
    switch (led)
    {
        case 1: write_pin_state(GPIOB, 7, estado); break;
        case 2: write_pin_state(GPIOB, 1, estado); break;
        case 3: write_pin_state(GPIOB, 2, estado); break;
        case 4: write_pin_state(GPIOB, 3, estado); break;
        case 5: write_pin_state(GPIOB, 4, estado); break;
        case 6: write_pin_state(GPIOB, 5, estado); break;
        case 7: write_pin_state(GPIOB, 6, estado); break;
        case 8: write_pin_state(GPIOC, 8, estado); break;
        case 9: write_pin_state(GPIOA, 9, estado); break;
        default: break;
    }
}