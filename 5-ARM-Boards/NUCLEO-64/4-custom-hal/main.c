#include "gpio_config.h"

volatile uint8_t hundreds;
volatile uint8_t tens;
volatile uint8_t units;

int main(void)
{
    uint8_t entrada;

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

    while (1)
    {
        entrada = leer_dip();

        hundreds = entrada / 100;
        tens = (entrada / 10) % 10;
        units = entrada % 10;

        apagar_digitos();
        mostrar_numero(hundreds);
        escribir(GPIOB, 9, 0);
        retardo(1000);

        apagar_digitos();
        mostrar_numero(tens);
        escribir(GPIOB, 10, 0);
        retardo(1000);

        apagar_digitos();
        mostrar_numero(units);
        escribir(GPIOA, 6, 0);
        retardo(1000);
    }

    HALT();

    return 0;
}