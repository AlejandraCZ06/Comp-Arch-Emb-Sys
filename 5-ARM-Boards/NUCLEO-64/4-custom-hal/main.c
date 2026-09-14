#include "gpio.h"

volatile uint8_t hundreds;
volatile uint8_t tens;
volatile uint8_t units;

int main(void)
{
    uint8_t entrada;

    GPIO_Config();

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