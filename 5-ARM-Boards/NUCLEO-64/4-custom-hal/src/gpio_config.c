#include "gpio_config.h"


/* =========================================================
   CONFIGURAR ENTRADA CON PULL-UP
   Se usa para los 9 botones externos.

   Boton sin presionar = 1
   Boton presionado    = 0
   ========================================================= */
static void configurar_entrada_pullup(volatile GPIO_TypeDef *GPIOx,
                                      uint8_t pin)
{
    GPIOx->MODER &= ~(3 << (pin * 2));

    GPIOx->PUPDR &= ~(3 << (pin * 2));
    GPIOx->PUPDR |=  (1 << (pin * 2));
}


/* =========================================================
   CONFIGURAR ENTRADA CON PULL-DOWN

   Se usa para B1 en PC13.

   B1 sin presionar = 0
   B1 presionado    = 1
   ========================================================= */
static void configurar_entrada_pulldown(volatile GPIO_TypeDef *GPIOx,
                                        uint8_t pin)
{
    GPIOx->MODER &= ~(3 << (pin * 2));

    GPIOx->PUPDR &= ~(3 << (pin * 2));
    GPIOx->PUPDR |=  (2 << (pin * 2));
}


/* =========================================================
   CONFIGURAR SALIDA
   ========================================================= */
static void configurar_salida(volatile GPIO_TypeDef *GPIOx,
                              uint8_t pin)
{
    GPIOx->MODER &= ~(3 << (pin * 2));
    GPIOx->MODER |=  (1 << (pin * 2));
}


/* =========================================================
   CONFIGURACION GENERAL GPIO
   ========================================================= */
void GPIO_Config(void)
{
    /*
     * Habilitar relojes GPIOA, GPIOB y GPIOC
     */
    RCC->AHB1ENR |= (1 << 0);
    RCC->AHB1ENR |= (1 << 1);
    RCC->AHB1ENR |= (1 << 2);

    /*
     * Pequeño acceso para asegurar habilitacion
     */
    volatile unsigned int dummy;

    dummy = RCC->AHB1ENR;
    dummy = RCC->AHB1ENR;


    /* =====================================================
       BOTONES EXTERNOS
       ===================================================== */

    /* Boton 1 -> PC0 */
    configurar_entrada_pullup(GPIOC, 0);

    /* Boton 2 -> PC1 */
    configurar_entrada_pullup(GPIOC, 1);

    /* Boton 3 -> PB8 */
    configurar_entrada_pullup(GPIOB, 8);

    /* Boton 4 -> PB13 */
    configurar_entrada_pullup(GPIOB, 13);

    /* Boton 5 -> PB14 */
    configurar_entrada_pullup(GPIOB, 14);

    /* Boton 6 -> PC5 */
    configurar_entrada_pullup(GPIOC, 5);

    /* Boton 7 -> PC6 */
    configurar_entrada_pullup(GPIOC, 6);

    /* Boton 8 -> PC7 */
    configurar_entrada_pullup(GPIOC, 7);

    /* Boton 9 -> PA10 */
    configurar_entrada_pullup(GPIOA, 10);


    /* =====================================================
       B1 DE LA NUCLEO
       PC13
       ===================================================== */

    configurar_entrada_pulldown(GPIOC, 13);


    /* =====================================================
       LEDS ROJOS
       ===================================================== */

    /* LED rojo 1 -> PA0 */
    configurar_salida(GPIOA, 0);

    /* LED rojo 2 -> PA1 */
    configurar_salida(GPIOA, 1);

    /* LED rojo 3 -> PB9 */
    configurar_salida(GPIOB, 9);

    /* LED rojo 4 -> PA8 */
    configurar_salida(GPIOA, 8);

    /* LED rojo 5 -> PA4 */
    configurar_salida(GPIOA, 4);

    /* LED rojo 6 -> PA6 */
    configurar_salida(GPIOA, 6);

    /* LED rojo 7 -> PA5 */
    configurar_salida(GPIOA, 5);

    /* LED rojo 8 -> PA7 */
    configurar_salida(GPIOA, 7);

    /* LED rojo 9 -> PB15 */
    configurar_salida(GPIOB, 15);


    /* =====================================================
       LEDS AZULES
       ===================================================== */

    /* LED azul 1 -> PB7 */
    configurar_salida(GPIOB, 7);

    /* LED azul 2 -> PB1 */
    configurar_salida(GPIOB, 1);

    /* LED azul 3 -> PB2 */
    configurar_salida(GPIOB, 2);

    /* LED azul 4 -> PB3 */
    configurar_salida(GPIOB, 3);

    /* LED azul 5 -> PB4 */
    configurar_salida(GPIOB, 4);

    /* LED azul 6 -> PB5 */
    configurar_salida(GPIOB, 5);

    /* LED azul 7 -> PB6 */
    configurar_salida(GPIOB, 6);

    /* LED azul 8 -> PC8 */
    configurar_salida(GPIOC, 8);

    /* LED azul 9 -> PA9 */
    configurar_salida(GPIOA, 9);


    /* =====================================================
       APAGAR TODOS LOS LEDS ROJOS
       ===================================================== */

    write_pin_state(GPIOA, 0, 0);
    write_pin_state(GPIOA, 1, 0);
    write_pin_state(GPIOB, 9, 0);
    write_pin_state(GPIOA, 8, 0);
    write_pin_state(GPIOA, 4, 0);
    write_pin_state(GPIOA, 6, 0);
    write_pin_state(GPIOA, 5, 0);
    write_pin_state(GPIOA, 7, 0);
    write_pin_state(GPIOB, 15, 0);


    /* =====================================================
       APAGAR TODOS LOS LEDS AZULES
       ===================================================== */

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


/* =========================================================
   LEER GPIO
   ========================================================= */
uint8_t read_pin_state(volatile GPIO_TypeDef *GPIOx,
                       uint8_t pin)
{
    if (GPIOx->IDR & (1 << pin))
        return 1;
    else
        return 0;
}


/* =========================================================
   ESCRIBIR GPIO
   ========================================================= */
void write_pin_state(volatile GPIO_TypeDef *GPIOx,
                     uint8_t pin,
                     uint8_t state)
{
    if (state)
        GPIOx->ODR |= (1 << pin);
    else
        GPIOx->ODR &= ~(1 << pin);
}


/* =========================================================
   LEER LOS 9 BOTONES DEL TABLERO

   1 = presionado
   0 = no presionado
   ========================================================= */
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


/* =========================================================
   LEER B1

   B1 presionado = 1
   B1 no presionado = 0
   ========================================================= */
uint8_t boton_start(void)
{
    return read_pin_state(GPIOC, 13) == 1;
}


/* =========================================================
   CONTROL LED ROJO
   ========================================================= */
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


/* =========================================================
   CONTROL LED AZUL
   ========================================================= */
void led_azul(uint8_t led, uint8_t estado)
{
    switch (led)
    {
        case 1:
            write_pin_state(GPIOB, 7, estado);
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
            write_pin_state(GPIOC, 8, estado);
            break;

        case 9:
            write_pin_state(GPIOA, 9, estado);
            break;

        default:
            break;
    }
}