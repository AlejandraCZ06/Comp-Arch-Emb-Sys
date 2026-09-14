#include "gpio_config.h"

int main(void)
{
    GPIO_Config();

    while (1)
    {
        if (boton_presionado(1))
            led_rojo(1, 1);
        else
            led_rojo(1, 0);

        if (boton_presionado(2))
            led_rojo(2, 1);
        else
            led_rojo(2, 0);

        if (boton_presionado(3))
            led_rojo(3, 1);
        else
            led_rojo(3, 0);

        if (boton_presionado(4))
            led_rojo(4, 1);
        else
            led_rojo(4, 0);

        if (boton_presionado(5))
            led_rojo(5, 1);
        else
            led_rojo(5, 0);

        if (boton_presionado(6))
            led_rojo(6, 1);
        else
            led_rojo(6, 0);

        if (boton_presionado(7))
            led_rojo(7, 1);
        else
            led_rojo(7, 0);

        if (boton_presionado(8))
            led_rojo(8, 1);
        else
            led_rojo(8, 0);

        if (boton_presionado(9))
            led_rojo(9, 1);
        else
            led_rojo(9, 0);
    }

    return 0;
}