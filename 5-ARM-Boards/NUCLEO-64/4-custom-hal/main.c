#include "gpio_config.h"

int main(void)
{
    GPIO_Config();

    while (1)
    {
        /*
         * BOTON 1 -> LED 1
         */
        if (boton_presionado(1))
        {
            led_rojo(1, 1);
        }


        /*
         * BOTON 2 -> LED 2
         */
        if (boton_presionado(2))
        {
            led_rojo(2, 1);
        }


        /*
         * BOTON 3 -> LED 3
         */
        if (boton_presionado(3))
        {
            led_rojo(3, 1);
        }


        /*
         * BOTON 4 -> LED 4
         */
        if (boton_presionado(4))
        {
            led_rojo(4, 1);
        }


        /*
         * BOTON 5 -> LED 5
         */
        if (boton_presionado(5))
        {
            led_rojo(5, 1);
        }


        /*
         * BOTON 6 -> LED 6
         */
        if (boton_presionado(6))
        {
            led_rojo(6, 1);
        }


        /*
         * BOTON 7 -> LED 7
         */
        if (boton_presionado(7))
        {
            led_rojo(7, 1);
        }


        /*
         * BOTON 8 -> LED 8
         */
        if (boton_presionado(8))
        {
            led_rojo(8, 1);
        }


        /*
         * BOTON 9 -> LED 9
         */
        if (boton_presionado(9))
        {
            led_rojo(9, 1);
        }
    }

    return 0;
}