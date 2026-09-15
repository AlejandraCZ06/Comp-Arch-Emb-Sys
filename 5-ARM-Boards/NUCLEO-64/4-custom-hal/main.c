#include "gpio_config.h"

int main(void)
{
    uint16_t anterior = 0;
    uint16_t actual = 1;
    uint16_t siguiente;

    GPIO_Config();

    while (1)
    {
        mostrar_3_digitos_250ms(actual);

        siguiente = anterior + actual;

        anterior = actual;
        actual = siguiente;

        if (actual > 987)
        {
            anterior = 0;
            actual = 1;
        }
    }

    HALT();

    return 0;
}